#include <libsoup/soup.h>
#include <string.h>

#define SOUP_STATUS_OK_200                SOUP_STATUS_OK
#define SOUP_STATUS_BAD_REQUEST_400       SOUP_STATUS_BAD_REQUEST
#define SOUP_STATUS_NOT_FOUND_404         SOUP_STATUS_NOT_FOUND
#define SOUP_STATUS_NOT_IMPLEMENTED_501   SOUP_STATUS_NOT_IMPLEMENTED

typedef struct {
  gboolean state_a;
  gboolean state_b;
  gboolean state_c;
  gboolean state_d;
} AppState;

/* GET  curl http://localhost:8080/api/hello */
static void
hello_handler(SoupServer        *server,
              SoupServerMessage *msg,
              const char        *path,
              GHashTable        *query,
              gpointer           user_data)
{
  if (soup_server_message_get_method(msg) != SOUP_METHOD_GET) {
    soup_server_message_set_status(msg, SOUP_STATUS_NOT_IMPLEMENTED_501, NULL);
    return;
  }

  static const char body[] = "{\"message\": \"Hello from libsoup!\"}";

  soup_message_headers_set_content_type(
      soup_server_message_get_response_headers(msg), "application/json", NULL);

  soup_message_body_append(
      soup_server_message_get_response_body(msg), SOUP_MEMORY_STATIC, body, strlen(body));

  soup_server_message_set_status(msg, SOUP_STATUS_OK_200, NULL);
}

static void
send_state_json(SoupServerMessage *msg, gboolean state)
{
  char *body = g_strdup_printf("{\"state\": %s}", state ? "true" : "false");

  soup_message_headers_set_content_type(
      soup_server_message_get_response_headers(msg), "application/json", NULL);

  soup_message_body_append(
      soup_server_message_get_response_body(msg), SOUP_MEMORY_TAKE, body, strlen(body));

  soup_server_message_set_status(msg, SOUP_STATUS_OK_200, NULL);
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
static void
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
    send_state_json(msg, *state);
    return;
  }

  if (method == SOUP_METHOD_POST) {
    const char *value = query ? g_hash_table_lookup(query, "value") : NULL;

    if (g_strcmp0(value, "true") != 0 && g_strcmp0(value, "false") != 0) {
      static const char body[] = "{\"error\": \"expected ?value=true|false\"}";

      soup_message_headers_set_content_type(
          soup_server_message_get_response_headers(msg), "application/json", NULL);

      soup_message_body_append(
          soup_server_message_get_response_body(msg), SOUP_MEMORY_STATIC, body, strlen(body));

      soup_server_message_set_status(msg, SOUP_STATUS_BAD_REQUEST_400, NULL);
      return;
    }

    *state = g_strcmp0(value, "true") == 0;
    send_state_json(msg, *state);
    return;
  }

  soup_server_message_set_status(msg, SOUP_STATUS_NOT_IMPLEMENTED_501, NULL);
}

int
main(int argc, char **argv)
{
  g_autoptr(GError) error = NULL;
  g_autoptr(SoupServer) server = NULL;
  g_autoptr(GMainLoop) loop = NULL;
  AppState app_state = { TRUE, TRUE, FALSE, FALSE };

  server = soup_server_new("server-header", "libsoup-training/0.1", NULL);
  soup_server_add_handler(server, "/api/hello", hello_handler, NULL, NULL);
  soup_server_add_handler(server, "/api/state/a", state_handler, &app_state, NULL);
  soup_server_add_handler(server, "/api/state/b", state_handler, &app_state, NULL);
  soup_server_add_handler(server, "/api/state/c", state_handler, &app_state, NULL);
  soup_server_add_handler(server, "/api/state/d", state_handler, &app_state, NULL);

  if (!soup_server_listen_local(server, 8080, 0, &error)) {
    g_printerr("Failed to start server: %s\n", error->message);
    return 1;
  }

  g_print("Listening on http://localhost:8080/api/hello\n");

  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);

  return 0;
}
