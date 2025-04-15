cmocka_dep = dependency('cmocka', required: false)

test_incl_dirs = include_directories('.')

test_lib_sources = files(
  'test_helpers.c'
)

test_lib = library(
  'c_base_test',
  test_lib_sources,
  link_with: lib,
  include_directories: [incl_dirs, test_incl_dirs]
)

subdir('base')
subdir('os')
subdir('ds')
