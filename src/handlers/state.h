#pragma once

#include <libsoup/soup.h>

typedef struct {
  gboolean state_a;
  gboolean state_b;
  gboolean state_c;
  gboolean state_d;
} AppState;

void state_handler(SoupServer        *server,
                   SoupServerMessage *msg,
                   const char        *path,
                   GHashTable        *query,
                   gpointer           user_data);
