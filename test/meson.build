cmocka_dep = dependency('cmocka', required: true)
test_lib = library('test_lib', 'test_helpers.c', include_directories: incl_dirs, dependencies: cmocka_dep, link_with: lib)

subdir('ds')
