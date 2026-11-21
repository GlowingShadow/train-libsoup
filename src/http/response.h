#pragma once

#include <libsoup/soup.h>

void http_respond_json_take(SoupServerMessage *msg, guint status, char *body);
void http_respond_json_static(SoupServerMessage *msg, guint status, const char *body);
