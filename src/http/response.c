#include "response.h"

#include <string.h>

static void
set_json_content_type(SoupServerMessage *msg)
{
  soup_message_headers_set_content_type(
      soup_server_message_get_response_headers(msg), "application/json", NULL);
}

void
http_respond_json_take(SoupServerMessage *msg, guint status, char *body)
{
  set_json_content_type(msg);

  soup_message_body_append(
      soup_server_message_get_response_body(msg), SOUP_MEMORY_TAKE, body, strlen(body));

  soup_server_message_set_status(msg, status, NULL);
}

void
http_respond_json_static(SoupServerMessage *msg, guint status, const char *body)
{
  set_json_content_type(msg);

  soup_message_body_append(
      soup_server_message_get_response_body(msg), SOUP_MEMORY_STATIC, body, strlen(body));

  soup_server_message_set_status(msg, status, NULL);
}
