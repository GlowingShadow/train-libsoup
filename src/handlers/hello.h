#pragma once

#include <libsoup/soup.h>

void hello_handler(SoupServer        *server,
                   SoupServerMessage *msg,
                   const char        *path,
                   GHashTable        *query,
                   gpointer           user_data);
