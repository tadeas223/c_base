project('c_base', 'c',
  meson_version: '>= 1.1',
  default_options: ['c_std=c99', 'c_args=-Wall'])

name = meson.project_name()
incl_dirs = include_directories('src', 'include/' + name)

sources = []

subdir('src')

# testing executable
# will be removed
# sources += files('main.c')
# executable('main', sources, include_directories: incl_dirs)

lib = library('c_base', sources, include_directories: incl_dirs, install: true)

install_subdir('include', install_dir: get_option('prefix'))

pkgconfig = import('pkgconfig')

pkgconfig.generate(lib,
  description: 'Base library for C projects',
  version: '0.0.1',
  name: name,
  subdirs: name,
  libraries: lib,
)
