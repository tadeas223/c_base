project('c_base', 'c', default_options: ['c_std=c99', 'c_args=-Wall'])

incl_dirs = include_directories('src', 'include')

sources = []

subdir('src')


lib = shared_library('c_base', sources, include_directories: incl_dirs, install: true)

pkgconfig = import('pkgconfig')

pkgconfig.generate(lib,
  version: '0.0.1',
  name: 'c_base',
  description: 'Base library for C projects'
)

install_subdir('include', install_dir: join_paths(get_option('prefix'), 'include/c_base'))


executable('main', sources, include_directories: incl_dirs)
