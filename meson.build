project('c_base', 'c', default_options: ['c_std=c99', 'c_args= -Wall -Wextra -Werror'])

add_project_arguments('-pedantic',language: 'c')

incl_dirs = include_directories('include/c_base', 'src')
sources = []

subdir('src')

# lib 
lib = library(
  'c_base',
  sources,
  include_directories: incl_dirs,
  install: true
)

# pkg config
pkgconfig = configuration_data()
pkgconfig.set('prefix', get_option('prefix'))
pkgconfig.set('libdir', get_option('libdir'))
pkgconfig.set('includedir', get_option('includedir'))
pkgconfig.set('version', meson.project_version())

configure_file(
  input: 'c_base.pc.in',
  output: 'c_base.pc',
  configuration: pkgconfig,
  install_dir: get_option('libdir') / 'pkgconfig'
)

# headers
install_subdir('include/c_base', install_dir: 'include')

executable('main', 'src/main.c', link_with: lib, include_directories: incl_dirs)

