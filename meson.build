project('c_base', 'c', default_options: ['c_std=c99', 'c_args= -Wall -Wextra -Werror'])

add_project_arguments('-pedantic',language: 'c')

incl_dirs = include_directories('include/c_base', 'src')
sources = []

subdir('src')

lib = library('c_base', sources, include_directories: incl_dirs)
executable('main', 'src/main.c', link_with: lib, include_directories: incl_dirs)

