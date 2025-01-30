project(
  'base',
  'c',
  default_options: ['c_std=c89', 'c_args=-Wall', 'c_args=-Werror'],
)
headers_subdir = 'c_base'

include_dirs = include_directories('include', 'src')
sources = []

subdir('src')

library('c_base', sources, include_directories: include_dirs, install: true)

install_subdir('include', install_dir: 'include/c_base/')
install_data('c_base.pc', install_dir: 'lib/pkgconfig/')
