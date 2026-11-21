# libsoup training

A minimal C + Meson + libsoup3 API server, meant as a starting point for practicing HTTP API development in C.

## Running

Open this folder in VS Code and "Reopen in Container" (or `devcontainer up`), then:

```sh
meson setup build
meson compile -C build
./build/api-server
```

In another terminal:

```sh
curl http://localhost:8080/api/hello
# {"message": "Hello from libsoup!"}
```

Port 8080 is auto-forwarded by the devcontainer.

## Next exercises

- Add a second route, e.g. `/api/echo`, that reflects query params back as JSON.
- Add a `POST` handler that reads the request body (`soup_server_message_get_request_body`) and parses JSON with `libjson-glib`.
- Add simple path-based routing, e.g. `/api/items/<id>`, by parsing `path` in the handler.
- Return proper error responses (404, 400) for bad input instead of always 200.
- Split handlers into their own `.c`/`.h` files as the API grows, and wire them into `meson.build`.
