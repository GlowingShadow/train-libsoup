#include <libsoup/soup.h>

#include "handlers/echo.h"
#include "handlers/hello.h"
#include "handlers/items.h"
#include "handlers/state.h"

int
main(int argc, char **argv)
{
  g_autoptr(GError) error = NULL;
  g_autoptr(SoupServer) server = NULL;
  g_autoptr(GMainLoop) loop = NULL;
  g_autoptr(GHashTable) items = NULL;
  AppState app_state = { FALSE, FALSE, FALSE, FALSE };

  items = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

  server = soup_server_new("server-header", "libsoup-training/0.1", NULL);
  soup_server_add_handler(server, "/api/hello", hello_handler, NULL, NULL);
  soup_server_add_handler(server, "/api/echo", echo_handler, NULL, NULL);
  soup_server_add_handler(server, ITEMS_PATH_PREFIX, items_handler, items, NULL);
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
