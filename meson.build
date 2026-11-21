project('libsoup-training', 'c',
  version : '0.1',
  default_options : ['warning_level=2', 'c_std=c11'])

glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
soup_dep = dependency('libsoup-3.0')
json_dep = dependency('json-glib-1.0')

executable('api-server',
  'src/main.c',
  'src/http/response.c',
  'src/handlers/hello.c',
  'src/handlers/echo.c',
  'src/handlers/items.c',
  'src/handlers/state.c',
  dependencies : [glib_dep, gobject_dep, soup_dep, json_dep],
  install : true)
