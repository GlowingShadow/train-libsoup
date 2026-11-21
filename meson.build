project('libsoup-training', 'c',
  version : '0.1',
  default_options : ['warning_level=2', 'c_std=c11'])

glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
soup_dep = dependency('libsoup-3.0')

executable('api-server', 'src/main.c',
  dependencies : [glib_dep, gobject_dep, soup_dep],
  install : true)
