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

After changing `meson.build` (new source file or dependency), reconfigure:

```sh
meson setup --reconfigure build
```

## Formatting

```sh
./scripts/format.sh
```

Runs `clang-format` over everything in `src/`. Rules live in `.clang-format` at the repo root.

## Routes

| Method | Path                | Description                                              |
|--------|---------------------|------------------------------------------------------------|
| GET    | `/api/hello`        | Static hello-world JSON message.                           |
| GET    | `/api/echo`         | Reflects query params back as `{"echo": {...}}`.            |
| GET    | `/api/items/<id>`   | Looks up a stored item; `404` if it doesn't exist.          |
| POST   | `/api/items/<id>`   | Stores `<id>` with a JSON body `{"value": "..."}`; `400` on bad/missing JSON. |
| GET    | `/api/state/<a-d>`  | Reads one of four in-memory booleans.                       |
| POST   | `/api/state/<a-d>`  | Sets it via `?value=true\|false`; `400` on anything else.   |

Unsupported methods return `501`; unknown `/api/state/*` or missing `/api/items` ids return `404`/`400` as noted above.

## Source layout

```
src/
├── main.c              # wires handlers onto the SoupServer
├── http/
│   ├── response.{c,h}  # shared JSON response helpers
│   └── status.h        # SOUP_STATUS_*_NNN aliases (code visible in the name)
└── handlers/
    ├── hello.{c,h}
    ├── echo.{c,h}
    ├── items.{c,h}
    └── state.{c,h}
```

## Next exercises

- Add a `DELETE /api/items/<id>` handler.
- Add request logging (method, path, status) around every handler.
- Swap the in-memory `items`/`state` storage for something persisted (SQLite via `libsqlite3`, or a flat file).
- Add a `meson test` suite that spins up the server and curls it.
- Add authentication (e.g. a static bearer token checked via request headers).
