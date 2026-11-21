#include "hello.h"

#include "../http/response.h"
#include "../http/status.h"

/* GET  curl http://localhost:8080/api/hello */
void
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

  http_respond_json_static(msg, SOUP_STATUS_OK_200, "{\"message\": \"Hello from libsoup!\"}");
}
