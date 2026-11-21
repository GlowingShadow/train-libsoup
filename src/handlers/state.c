#include "state.h"

#include "../http/response.h"
#include "../http/status.h"

static char *
state_json(gboolean state)
{
  return g_strdup_printf("{\"state\": %s}", state ? "true" : "false");
}

static gboolean *
state_field(AppState *app_state, const char *path)
{
  if (g_strcmp0(path, "/api/state/a") == 0)
    return &app_state->state_a;
  if (g_strcmp0(path, "/api/state/b") == 0)
    return &app_state->state_b;
  if (g_strcmp0(path, "/api/state/c") == 0)
    return &app_state->state_c;
  if (g_strcmp0(path, "/api/state/d") == 0)
    return &app_state->state_d;
  return NULL;
}

/* GET  curl http://localhost:8080/api/state/a
 * POST curl -X POST "http://localhost:8080/api/state/a?value=true"
 */
void
state_handler(SoupServer        *server,
              SoupServerMessage *msg,
              const char        *path,
              GHashTable        *query,
              gpointer           user_data)
{
  AppState   *app_state = (AppState *)user_data;
  gboolean   *state = state_field(app_state, path);
  const char *method = soup_server_message_get_method(msg);

  if (!state) {
    soup_server_message_set_status(msg, SOUP_STATUS_NOT_FOUND_404, NULL);
    return;
  }

  if (method == SOUP_METHOD_GET) {
    http_respond_json_take(msg, SOUP_STATUS_OK_200, state_json(*state));
    return;
  }

  if (method == SOUP_METHOD_POST) {
    const char *value = query ? g_hash_table_lookup(query, "value") : NULL;

    if (g_strcmp0(value, "true") != 0 && g_strcmp0(value, "false") != 0) {
      http_respond_json_static(
          msg, SOUP_STATUS_BAD_REQUEST_400, "{\"error\": \"expected ?value=true|false\"}");
      return;
    }

    *state = g_strcmp0(value, "true") == 0;
    http_respond_json_take(msg, SOUP_STATUS_OK_200, state_json(*state));
    return;
  }

  soup_server_message_set_status(msg, SOUP_STATUS_NOT_IMPLEMENTED_501, NULL);
}
