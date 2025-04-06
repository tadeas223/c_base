project('c_base', 'c', default_options: ['c_std=c99', 'c_args=-Wall', 'includedir=include'])

name = meson.project_name()
incl_dirs = include_directories('src', 'include')

sources = []

subdir('src')

# testing executable
# will be removed
executable('main', sources, include_directories: incl_dirs)

lib = shared_library('c_base', sources, include_directories: incl_dirs, install: true)

pkgconfig = import('pkgconfig')

pkgconfig.generate(lib,
  version: '0.0.1',
  name: 'c_base',
  description: 'Base library for C projects'
)
