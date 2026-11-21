#pragma once

#include <libsoup/soup.h>

void echo_handler(SoupServer        *server,
                  SoupServerMessage *msg,
                  const char        *path,
                  GHashTable        *query,
                  gpointer           user_data);
