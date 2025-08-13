test_c_darray = executable('test_c_darray', 'test_C_DArray.c', dependencies: [cmocka_dep], link_with: [lib, test_lib], include_directories: [incl_dirs])
test('ds/C_DArray', test_c_darray)

test_c_list = executable('test_c_list', 'test_C_List.c', dependencies: [cmocka_dep], link_with: [lib, test_lib], include_directories: [incl_dirs])
test('ds/C_List', test_c_list)

test_c_array = executable('test_c_array', 'test_C_Array.c', dependencies: [cmocka_dep], link_with: [lib, test_lib], include_directories: [incl_dirs])
test('ds/C_Array', test_c_array)
