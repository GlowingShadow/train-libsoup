#pragma once

#include <libsoup/soup.h>

#define ITEMS_PATH_PREFIX "/api/items"

/* user_data must be a GHashTable* (gchar* -> gchar*), e.g.
 * g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free) */
void items_handler(SoupServer        *server,
                   SoupServerMessage *msg,
                   const char        *path,
                   GHashTable        *query,
                   gpointer           user_data);
