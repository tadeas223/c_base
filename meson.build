project(
  'base',
  'c',
  default_options: ['c_std=c89', 'c_args=-Wall', 'c_args=-Werror'],
)
headers_subdir = 'c_base'

include_dirs = include_directories('include', 'src')
sources = []

subdir('src')

lib = library('c_base', sources, include_directories: include_dirs, install: true)

executable('main', sources, link_with: lib, include_directories: include_dirs)
install_subdir('include', install_dir: 'include/c_base/', strip_directory: true)
install_data('c_base.pc', install_dir: 'lib/pkgconfig/')
