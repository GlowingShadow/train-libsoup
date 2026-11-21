#include "echo.h"

#include "../http/response.h"
#include "../http/status.h"

#include <json-glib/json-glib.h>

/* GET  curl "http://localhost:8080/api/echo?foo=bar&baz=qux" */
void
echo_handler(SoupServer        *server,
             SoupServerMessage *msg,
             const char        *path,
             GHashTable        *query,
             gpointer           user_data)
{
  if (soup_server_message_get_method(msg) != SOUP_METHOD_GET) {
    soup_server_message_set_status(msg, SOUP_STATUS_NOT_IMPLEMENTED_501, NULL);
    return;
  }

  g_autoptr(JsonBuilder) builder = json_builder_new();

  json_builder_begin_object(builder);
  json_builder_set_member_name(builder, "echo");
  json_builder_begin_object(builder);

  if (query) {
    GHashTableIter iter;
    gpointer       key, value;

    g_hash_table_iter_init(&iter, query);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
      json_builder_set_member_name(builder, (const char *)key);
      json_builder_add_string_value(builder, (const char *)value);
    }
  }

  json_builder_end_object(builder);
  json_builder_end_object(builder);

  g_autoptr(JsonNode) root = json_builder_get_root(builder);
  g_autoptr(JsonGenerator) generator = json_generator_new();

  json_generator_set_root(generator, root);
  http_respond_json_take(msg, SOUP_STATUS_OK_200, json_generator_to_data(generator, NULL));
}
