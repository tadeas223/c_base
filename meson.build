project('c_base', 'c',
  meson_version: '>= 1.1',
  default_options: ['c_std=c99', 'c_args=-Wall'])

name = meson.project_name()
incl_dirs = include_directories('src', 'include')

sources = []

subdir('src')
subdir('include')

# testing executable
# will be removed
# sources += files('main.c')
# executable('main', sources, include_directories: incl_dirs)

lib = library('c_base', sources, include_directories: incl_dirs, install: true)

pkgconfig = import('pkgconfig')

pkgconfig.generate(lib,
  version: '0.0.1',
  name: 'c_base',
  libraries: lib,
  description: 'Base library for C projects',
)
