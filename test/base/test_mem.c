#include "base/types.h"
#include "os/os_mem.h"
#include <stdio.h>
#include <stdlib.h>

/* make cmocka work for c89 */
#define inline

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "base/mem.h"


/****************************************
 * Memory Operations
****************************************/

static void test_m_align_forward() {
    assert_int_equal(m_align_forward(10, 16), 16);
    assert_int_equal(m_align_forward(14, 16), 16);
    assert_int_equal(m_align_forward(1, 16), 16);
    assert_int_equal(m_align_forward(17, 16), 32);
    assert_int_equal(m_align_forward(20, 16), 32);
    
    assert_int_equal(m_align_forward(10, 64), 64);
    assert_int_equal(m_align_forward(14, 64), 64);
    assert_int_equal(m_align_forward(68, 64), 128);
    assert_int_equal(m_align_forward(145, 64), 192);
    assert_int_equal(m_align_forward(243, 64), 256);
}

static void test_m_align_backward() {
    assert_int_equal(m_align_backward(10, 16), 0);
    assert_int_equal(m_align_backward(14, 16), 0);
    assert_int_equal(m_align_backward(1, 16), 0);
    assert_int_equal(m_align_backward(17, 16), 16);
    assert_int_equal(m_align_backward(20, 16), 16);
    
    assert_int_equal(m_align_backward(10, 64), 0);
    assert_int_equal(m_align_backward(14, 64), 0);
    assert_int_equal(m_align_backward(68, 64), 64);
    assert_int_equal(m_align_backward(145, 64), 128);
    assert_int_equal(m_align_backward(243, 64), 192);
}

static void test_m_copy() {
    char arr1[] = "hello";
    char arr2[] = "world";

    m_copy(arr1, arr2, 6);
    
    assert_string_equal(arr1, "world");
    assert_string_equal(arr1, arr2);
    
    /* reverse the copy order */
    char arr3[] = "hello";
    char arr4[] = "world";
    m_copy(arr4, arr3, 6);
    
    assert_string_equal(arr3, "hello");
    assert_string_equal(arr3, arr4);
}

static const struct CMUnitTest grp_memory_operations[] = {
    cmocka_unit_test(test_m_align_forward),
    cmocka_unit_test(test_m_align_backward),
    cmocka_unit_test(test_m_copy),
};


/****************************************
 * Memory Base 
****************************************/

static void test_m_memory_base_set_get_default() {
    int test = 0;
    /* Test the poiter idk */
    m_memory_base_set_default((void*)&test);
    assert_ptr_equal(m_memory_base_get_default(), &test);
}

static const struct CMUnitTest grp_memory_base[] = {
    cmocka_unit_test(test_m_memory_base_set_get_default),
};

/****************************************
 * 64-bit Arena - initializers
****************************************/

static void test_m_arena_begin() {
    m_memory_base_set_default(os_memory_base());

    m_Arena arena;
    m_arena_begin(&arena);
    {
        assert_ptr_equal(arena.base, os_memory_base());
        assert_int_equal(arena.cap, M_ARENA_DEFAULT_RESERVE);
        assert_int_equal(arena.pos, 0);
        assert_int_equal(arena.commit_pos, 0);
    }
    m_arena_end(&arena);
}

static void test_m_arena_begin_reserve() {
    m_memory_base_set_default(os_memory_base());

    m_Arena arena;
    m_arena_begin_reserve(&arena, 1024);
    {
        assert_ptr_equal(arena.base, os_memory_base());
        assert_int_equal(arena.cap, 1024);
        assert_int_equal(arena.pos, 0);
        assert_int_equal(arena.commit_pos, 0);
    } 
    m_arena_end(&arena);
}

static void test_m_arena_begin_reserve_base() {
    m_Arena arena;
    m_arena_begin_reserve_base(&arena, os_memory_base(), 1024);
    {
        assert_ptr_equal(arena.base, os_memory_base());
        assert_int_equal(arena.cap, 1024);
        assert_int_equal(arena.pos, 0);
        assert_int_equal(arena.commit_pos, 0);
        
    }
    m_arena_end(&arena);
}
static const struct CMUnitTest grp_arena_initializers[] = {
    cmocka_unit_test(test_m_arena_begin),
    cmocka_unit_test(test_m_arena_begin_reserve),
    cmocka_unit_test(test_m_arena_begin_reserve_base),
};

/****************************************
 * 64-bit Arena - destructors
****************************************/

static void test_m_arena_end() {
    /* 
     * This cannot be tested.
     * This function does not change anything
    */ 
}

static void test_m_arena_reset() {
    m_Arena arena;
    m_arena_begin_reserve_base(&arena, os_memory_base(), 1024);
    m_arena_reset(&arena);
    assert_ptr_equal(arena.base, os_memory_base());
    assert_int_equal(arena.cap, 1024);
    assert_int_equal(arena.pos, 0);
    assert_int_equal(arena.commit_pos, 0);
}

static const struct CMUnitTest grp_arena_destructors[] = {
    cmocka_unit_test(test_m_arena_end),
    cmocka_unit_test(test_m_arena_reset),
};


/****************************************
 * 64-bit Arena - allocations
****************************************/

static void test_m_arena_alloc() {
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        u64 size = m_align_forward(4, M_DEFAULT_ALIGN);
        u64 size2 = m_align_forward(M_ARENA_COMMIT_BLOCK, M_DEFAULT_ALIGN);
        u64 size3 = m_align_forward(M_ARENA_COMMIT_BLOCK * 2, M_DEFAULT_ALIGN);
        
        void* ptr = m_arena_alloc(&arena, 4);
        assert_ptr_equal(arena.memory, ptr);
        assert_ptr_equal(arena.pos, size);
        assert_ptr_equal(arena.commit_pos, M_ARENA_COMMIT_BLOCK);

        void* ptr2 = m_arena_alloc(&arena, M_ARENA_COMMIT_BLOCK);
        assert_ptr_equal(arena.memory + size, ptr2);
        assert_ptr_equal(arena.pos,  size + size2);
        
        assert_ptr_equal(arena.commit_pos, M_ARENA_COMMIT_BLOCK * 2);
   
        void* ptr3 = m_arena_alloc(&arena, M_ARENA_COMMIT_BLOCK * 2);
        assert_ptr_equal(arena.memory + size + size2, ptr3);
        assert_ptr_equal(arena.pos,  size + size2 + size3);
        assert_ptr_equal(arena.commit_pos, M_ARENA_COMMIT_BLOCK * 4);

        void* ptr4 = m_arena_alloc(&arena, M_ARENA_DEFAULT_RESERVE);
        assert_null(ptr4);
    }
    m_arena_end(&arena);
}

static void test_m_arena_dealloc() {
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        m_arena_alloc(&arena, 4);
        m_arena_dealloc(&arena, 4);
        assert_ptr_equal(arena.pos, 0);
        assert_ptr_equal(arena.commit_pos, 0);

        m_arena_alloc(&arena, M_ARENA_COMMIT_BLOCK * 2);
        m_arena_dealloc(&arena, M_ARENA_COMMIT_BLOCK); 
        assert_ptr_equal(arena.pos,  0);
        assert_ptr_equal(arena.commit_pos, 0);
    
        m_arena_alloc(&arena, 4);
        m_arena_dealloc(&arena, Kilobytes(1));
        assert_ptr_equal(arena.pos, 0);
        assert_ptr_equal(arena.commit_pos, 0);
    }
    m_arena_end(&arena);
}

static void test_m_arena_dealloc_to() {
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        m_arena_alloc(&arena, 4);
        m_arena_dealloc_to(&arena, 4); 
        assert_ptr_equal(arena.pos, 0);
        assert_ptr_equal(arena.commit_pos, 0);

        m_arena_alloc(&arena, Kilobytes(5));
        m_arena_dealloc_to(&arena, Kilobytes(1)); 
        assert_ptr_equal(arena.pos,  Kilobytes(1));
        assert_ptr_equal(arena.commit_pos, M_ARENA_COMMIT_BLOCK);
    }
    m_arena_end(&arena);
}

static const struct CMUnitTest grp_arena_allocations[] = {
    cmocka_unit_test(test_m_arena_alloc),
    cmocka_unit_test(test_m_arena_dealloc),
    cmocka_unit_test(test_m_arena_dealloc_to),
};

/****************************************
 * Temporary arenas
****************************************/

static void test_m_temp_begin() {
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        m_Temp temp;
        m_temp_begin(&arena, &temp);
        {
            assert_true(temp.arena == &arena);
            assert_true(temp.pos == temp.pos);
        }
        m_temp_end(&temp);
    }
    m_arena_end(&arena);
}

static void test_m_temp_end() {
    
    m_Arena arena;
    m_arena_begin_base(&arena, os_memory_base());
    {
        u64 pos = arena.pos;

        m_Temp temp;
        m_temp_begin(&arena, &temp);
        {
            /* allocate stuff */
            m_arena_alloc(&arena, Kilobytes(5)); 
        }
        m_temp_end(&temp);

        assert_int_equal(arena.pos, pos);        
    }
    m_arena_end(&arena);
}

static const struct CMUnitTest grp_temp_arena[] = {
    cmocka_unit_test(test_m_temp_begin),
    cmocka_unit_test(test_m_temp_end),
};

/****************************************
 * 64-bit Pool - initializers
****************************************/

static void test_m_pool_begin() {
    m_memory_base_set_default(os_memory_base());
    
    m_Pool pool;
    m_pool_begin(&pool, sizeof(u64));
    {
        assert_int_equal(pool.data_size, sizeof(u64));
        assert_int_equal(pool.cap, M_POOL_DEFAULT_RESERVE); 
        assert_int_equal(pool.base, os_memory_base()); 
        assert_int_equal(pool.commit_pos, 0); 
        assert_null(pool.head); 
    }
    m_pool_end(&pool);
}

static void test_m_pool_begin_base() {
    m_Pool pool;
    m_pool_begin_base(&pool, os_memory_base(), sizeof(u64));
    {
        assert_int_equal(pool.data_size, sizeof(u64));
        assert_int_equal(pool.cap, M_POOL_DEFAULT_RESERVE); 
        assert_int_equal(pool.base, os_memory_base()); 
        assert_int_equal(pool.commit_pos, 0); 
        assert_null(pool.head); 
    }
    m_pool_end(&pool);
}

static void test_m_pool_begin_base_reserve() {
    m_Pool pool;
    m_pool_begin_reserve_base(&pool, os_memory_base(), Kilobytes(1), sizeof(u64));
    {
        assert_int_equal(pool.data_size, sizeof(u64));
        assert_int_equal(pool.cap, Kilobytes(1)); 
        assert_int_equal(pool.base, os_memory_base()); 
        assert_int_equal(pool.commit_pos, 0); 
        assert_null(pool.head); 
    }
    m_pool_end(&pool);
}

static const struct CMUnitTest grp_pool_initializers[] = {
    cmocka_unit_test(test_m_pool_begin),
    cmocka_unit_test(test_m_pool_begin_base),
    cmocka_unit_test(test_m_pool_begin_base_reserve),
};

/****************************************
 * 64-bit Pool - destructors
****************************************/

static void test_m_pool_end() {
    /*
     * This function does not change anything.
     * Therfore it cannot be tested.
     */
}

static void test_m_pool_reset() {
    m_Pool pool;
    m_pool_begin_base(&pool, os_memory_base(), sizeof(u64));
    {
        /* Do nothing here. */
    }
    m_pool_reset(&pool);
    {
        assert_int_equal(pool.data_size, sizeof(u64));
        assert_int_equal(pool.cap, M_POOL_DEFAULT_RESERVE); 
        assert_int_equal(pool.base, os_memory_base()); 
        assert_int_equal(pool.commit_pos, 0); 
    }
    m_pool_end(&pool);
}

static const struct CMUnitTest grp_pool_destructors[] = {
    cmocka_unit_test(test_m_pool_end),
    cmocka_unit_test(test_m_pool_reset),
};

/****************************************
 * 64-bit Pool - allcations
****************************************/

static void test_m_pool_alloc() {
    m_Pool pool;
    m_pool_begin_base(&pool, os_memory_base(), sizeof(u16));
    {
        u16 *ptr1 = m_pool_alloc(&pool);
        *ptr1 = 5; 
        assert_ptr_equal(pool.commit_pos, M_POOL_COMMIT_BLOCK * pool.data_size);
    
        u8 i;
        for(i = 0; i < M_POOL_COMMIT_BLOCK; i++) {
            u16 *ptr2 = m_pool_alloc(&pool);
            *ptr2 = 5;
        }
        assert_ptr_equal(pool.commit_pos, M_POOL_COMMIT_BLOCK * 2 * pool.data_size);
    }
    m_pool_end(&pool);
}

static void test_m_pool_dealloc() {
    m_Pool pool;
    m_pool_begin_base(&pool, os_memory_base(), sizeof(u16));
    {

        u16 *ptr1 = m_pool_alloc(&pool);
        void* head = pool.head;
        m_pool_dealloc(&pool, ptr1);
        assert_ptr_not_equal(head, ptr1);

        /* dealloc all */
        u16 *ptrs[M_POOL_COMMIT_BLOCK];
        u8 i;
        for(i = 0; i < M_POOL_COMMIT_BLOCK; i++) {
            ptrs[i] = m_pool_alloc(&pool);
            *ptrs[i] = 5; 
        }

        for(i = 0; i < M_POOL_COMMIT_BLOCK; i++) {
            m_pool_dealloc(&pool, ptrs[i]);
        }

        m_pool_dealloc(&pool, ptr1);
    }
    m_pool_end(&pool);
}

static void test_m_pool_dealloc_count() {
    m_Pool pool;
    m_pool_begin_base(&pool, os_memory_base(), sizeof(u16));
    {

        u16 *ptr1 = m_pool_alloc(&pool);
        void* head = pool.head;
        m_pool_dealloc_count(&pool, ptr1, 1);
        assert_ptr_not_equal(head, ptr1);

        /* dealloc all */
        u8 i;
        for(i = 0; i < M_POOL_COMMIT_BLOCK; i++) {
            u16 *ptr2 = m_pool_alloc(&pool);
            *ptr2 = 5; 
        }

        m_pool_dealloc_count(&pool, ptr1, M_POOL_COMMIT_BLOCK + 1);
    }
    m_pool_end(&pool);
}

static const struct CMUnitTest grp_pool_allocations[] = {
    cmocka_unit_test(test_m_pool_alloc),
    cmocka_unit_test(test_m_pool_dealloc),
    cmocka_unit_test(test_m_pool_dealloc_count),
};



int main(int argc, char **argv) {
    if(argc > 1) {
        switch (argv[1][0]) {
            case '0': 
                return cmocka_run_group_tests(grp_memory_operations, NULL, NULL);
            case '1': 
                return cmocka_run_group_tests(grp_memory_base, NULL, NULL);
            case '2': 
                return cmocka_run_group_tests(grp_arena_initializers, NULL, NULL);
            case '3': 
                return cmocka_run_group_tests(grp_arena_destructors, NULL, NULL);
            case '4': 
                return cmocka_run_group_tests(grp_arena_allocations, NULL, NULL);
            case '5': 
                return cmocka_run_group_tests(grp_temp_arena, NULL, NULL);
            case '6': 
                return cmocka_run_group_tests(grp_pool_initializers, NULL, NULL);
            case '7': 
                return cmocka_run_group_tests(grp_pool_destructors, NULL, NULL);
            case '8': 
                return cmocka_run_group_tests(grp_pool_allocations, NULL, NULL);
            default:
                printf("Missing group");
                exit(1);
        }
    } else {
        printf("Missing argument");
        exit(1);
    }
    return 0;
}
