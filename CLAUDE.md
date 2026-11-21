# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A minimal C + Meson + libsoup3 HTTP API server, used as a practice starting point for HTTP API
development in C. The entire source is `.devcontainer/src/main.c`, built via
`.devcontainer/meson.build`. There is no application code outside the `.devcontainer/` directory.

## Commands

Run from `.devcontainer/` (that's where `meson.build` and `src/` live):

```sh
cd .devcontainer
meson setup build        # first time only
meson compile -C build
./build/api-server
```

Test the running server (default port 8080, auto-forwarded in the devcontainer):

```sh
curl http://localhost:8080/api/hello
# {"message": "Hello from libsoup!"}
```

Reconfigure after changing `meson.build` (e.g. new dependency or source file):

```sh
meson setup --reconfigure build
```

There is no test suite, linter, or CI config in this repo currently.

## Environment

Designed to run inside the devcontainer (`.devcontainer/.devcontainer/`), which installs
`libsoup-3.0-dev`, `libglib2.0-dev`, `libjson-glib-dev`, meson, ninja, and clang-format on a
Debian bookworm base. If working outside the devcontainer, these dev packages must be installed
manually for `meson setup` to find `glib-2.0`, `gobject-2.0`, and `libsoup-3.0` via pkg-config.

## Architecture

- `SoupServer` is created in `main()` and routes are registered with
  `soup_server_add_handler(server, "<path>", handler_fn, user_data, destroy_notify)` — one handler
  function per path prefix. Soup does not have a router; path/method matching inside each handler
  (e.g. checking `soup_server_message_get_method`) is the convention used here, as seen in
  `hello_handler`.
- Handlers follow the `SoupServerCallback` signature: `(SoupServer *, SoupServerMessage *, const
  char *path, GHashTable *query, gpointer user_data)`. Query string params arrive pre-parsed in the
  `query` hash table.
- Responses are built by setting headers via `soup_server_message_get_response_headers`, appending
  body bytes via `soup_message_body_append` on `soup_server_message_get_response_body`, and
  finishing with `soup_server_message_set_status`.
- The server runs on a `GMainLoop`; libsoup is GLib/GObject-based, so autoptr cleanup
  (`g_autoptr`) is used for `SoupServer`, `GMainLoop`, and `GError`.
- `libjson-glib` is installed and available for JSON parsing/building but not yet used in
  `main.c` — current responses are hand-written JSON string literals.

## Suggested exercise direction

The project's README lists concrete next exercises (adding routes like `/api/echo`, POST body
handling with `soup_server_message_get_request_body` + libjson-glib, path-based routing e.g.
`/api/items/<id>`, proper error status codes, and splitting handlers into separate files wired
into `meson.build`). When asked to extend this project without more specific direction, these are
the intended next steps.
