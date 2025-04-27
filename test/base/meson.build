test_mem = executable('test_mem', 'test_mem.c', dependencies: cmocka_dep, link_with: [lib, test_lib], include_directories: [incl_dirs, test_incl_dirs])
test('mem', test_mem)

test_strings = executable('test_strings', 'test_strings.c', dependencies: cmocka_dep, link_with: [lib, test_lib], include_directories: [incl_dirs, test_incl_dirs])
test('strings', test_strings)

test_errors = executable('test_errors', 'test_errors.c', dependencies: cmocka_dep, link_with: [lib, test_lib], include_directories: [incl_dirs, test_incl_dirs])
test('errors', test_errors)

test_varargs = executable('test_varargs', 'test_varargs.c', dependencies: cmocka_dep, link_with: [lib, test_lib], include_directories: [incl_dirs, test_incl_dirs])
test('varargs', test_varargs)

test_types = executable('test_types', 'test_types.c', dependencies: cmocka_dep, link_with: [lib, test_lib], include_directories: [incl_dirs, test_incl_dirs])
test('types', test_types)
