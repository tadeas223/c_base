test_mem = executable('test_mem', 'test_mem.c', dependencies: cmocka_dep, link_with: [lib, test_lib], include_directories: [incl_dirs, test_incl_dirs])
test('mem', test_mem)

test_strings = executable('test_strings', 'test_strings.c', dependencies: cmocka_dep, link_with: [lib, test_lib], include_directories: [incl_dirs, test_incl_dirs])
test('strings', test_mem)
