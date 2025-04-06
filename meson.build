project('c_base', 'c', default_options: ['c_std=c99', 'c_args=-Wall', 'includedir=include'])

name = meson.project_name()
incl_dirs = include_directories('src', 'include')

sources = []

subdir('src')
subdir('include')

# testing executable
# will be removed
# sources += files('main.c')
# executable('main', sources, include_directories: incl_dirs)

shared_lib = shared_library('c_base', sources, include_directories: incl_dirs, install: true)
static_lib = static_library('c_base', sources, include_directories: incl_dirs)

pkgconfig = import('pkgconfig')

pkgconfig.generate(shared_lib,
  version: '0.0.1',
  name: 'c_base',
  libraries: shared_lib,
  description: 'Base library for C projects',
)

pkgconfig.generate(static_lib,
  version: '0.0.1',
  name: 'c_base',
  libraries: static_lib,
  description: 'Base library for C projects'
)
