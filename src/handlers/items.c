#include "items.h"

#include "../http/response.h"
#include "../http/status.h"

#include <json-glib/json-glib.h>
#include <string.h>

static char *
item_json(const char *id, const char *value)
{
  g_autoptr(JsonBuilder) builder = json_builder_new();

  json_builder_begin_object(builder);
  json_builder_set_member_name(builder, "id");
  json_builder_add_string_value(builder, id);
  json_builder_set_member_name(builder, "value");
  json_builder_add_string_value(builder, value);
  json_builder_end_object(builder);

  g_autoptr(JsonNode) root = json_builder_get_root(builder);
  g_autoptr(JsonGenerator) generator = json_generator_new();

  json_generator_set_root(generator, root);
  return json_generator_to_data(generator, NULL);
}

static const char *
json_object_get_string_or_null(JsonObject *object, const char *member)
{
  JsonNode *node;

  if (!json_object_has_member(object, member))
    return NULL;

  node = json_object_get_member(object, member);

  if (!JSON_NODE_HOLDS_VALUE(node) || json_node_get_value_type(node) != G_TYPE_STRING)
    return NULL;

  return json_node_get_string(node);
}

/* GET  curl http://localhost:8080/api/items/42
 * POST curl -X POST -d '{"value": "hello"}' http://localhost:8080/api/items/42
 */
void
items_handler(SoupServer        *server,
              SoupServerMessage *msg,
              const char        *path,
              GHashTable        *query,
              gpointer           user_data)
{
  GHashTable *items = (GHashTable *)user_data;
  const char *rest = path + strlen(ITEMS_PATH_PREFIX);
  const char *method = soup_server_message_get_method(msg);

  if (rest[0] != '/' || rest[1] == '\0') {
    http_respond_json_static(msg, SOUP_STATUS_BAD_REQUEST_400, "{\"error\": \"missing item id\"}");
    return;
  }

  const char *id = rest + 1;

  if (method == SOUP_METHOD_GET) {
    const char *value = g_hash_table_lookup(items, id);

    if (!value) {
      http_respond_json_static(msg, SOUP_STATUS_NOT_FOUND_404, "{\"error\": \"item not found\"}");
      return;
    }

    http_respond_json_take(msg, SOUP_STATUS_OK_200, item_json(id, value));
    return;
  }

  if (method == SOUP_METHOD_POST) {
    SoupMessageBody *request_body = soup_server_message_get_request_body(msg);
    g_autoptr(JsonParser) parser = json_parser_new();
    g_autoptr(GError) error = NULL;

    if (!json_parser_load_from_data(parser, request_body->data, request_body->length, &error)) {
      http_respond_json_static(
          msg, SOUP_STATUS_BAD_REQUEST_400, "{\"error\": \"invalid JSON body\"}");
      return;
    }

    JsonNode *root = json_parser_get_root(parser);

    if (!JSON_NODE_HOLDS_OBJECT(root)) {
      http_respond_json_static(
          msg, SOUP_STATUS_BAD_REQUEST_400, "{\"error\": \"expected a JSON object\"}");
      return;
    }

    const char *value = json_object_get_string_or_null(json_node_get_object(root), "value");

    if (!value) {
      http_respond_json_static(
          msg, SOUP_STATUS_BAD_REQUEST_400, "{\"error\": \"expected a string 'value' field\"}");
      return;
    }

    g_hash_table_insert(items, g_strdup(id), g_strdup(value));
    http_respond_json_take(msg, SOUP_STATUS_OK_200, item_json(id, value));
    return;
  }

  soup_server_message_set_status(msg, SOUP_STATUS_NOT_IMPLEMENTED_501, NULL);
}
