/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include <check.h>

#include "lagavulin/block.h"


/*-----------------------------------------------------------------------
 * Helper functions
 */

#define DESCRIBE_TEST \
    fprintf(stderr, "--- %s\n", __func__);

#define test_stmt(block, typ, union_branch, expected, fmt) \
    do { \
        struct lgv_state  state; \
        lgv_state_init(&gc, &state); \
        state.ret = lgv_block_new_halt(&gc); \
        lgv_block_execute(&gc, block, &state); \
        typ  actual = lgv_stack_get(&state.stack, -1, union_branch); \
        fail_unless(actual == expected, \
                    "Unexpected result: got " fmt ", expected " fmt, \
                    actual, expected); \
        lgv_state_done(&gc, &state); \
    } while (0)

#define test_expr(block, typ, union_branch, expected, fmt) \
    do { \
        struct lgv_block  *b_ret = lgv_block_new_return(&gc); \
        lgv_block_set_next(&gc, block, b_ret); \
        test_stmt(block, typ, union_branch, expected, fmt); \
    } while (0)


/*-----------------------------------------------------------------------
 * Constants
 */

#define test_constant(typ_id, typ, union_branch, value, name, fmt) \
    START_TEST(test_constant_##typ_id##_##name) \
    { \
        DESCRIBE_TEST; \
        struct cork_alloc  *alloc = cork_allocator_new_debug(); \
        struct cork_gc  gc; \
        cork_gc_init(&gc, alloc); \
        struct lgv_block  *b0 = lgv_block_new_constant_##typ_id(&gc, value); \
        test_expr(b0, typ, union_branch, value, fmt); \
        cork_gc_decref(&gc, b0); \
        cork_gc_done(&gc); \
        cork_allocator_free(alloc); \
    } \
    END_TEST

test_constant(bool, bool, b, false, false, "%d");
test_constant(bool, bool, b, true, true, "%d");

test_constant(int, int, si, 0, 0, "%d");
test_constant(int, int, si, 1, 1, "%d");
test_constant(int, int, si, -1, _1, "%d");
test_constant(int, int, si, 100, 100, "%d");
test_constant(int, int, si, -100, _100, "%d");
test_constant(int, int, si, INT_MIN, INT_MIN, "%d");
test_constant(int, int, si, INT_MAX, INT_MAX, "%d");


/*-----------------------------------------------------------------------
 * If statement
 */

START_TEST(test_if_true)
{
    DESCRIBE_TEST;
    struct cork_alloc  *alloc = cork_allocator_new_debug();
    struct cork_gc  gc;
    cork_gc_init(&gc, alloc);

    struct lgv_block  *bc = lgv_block_new_constant_bool(&gc, true);
    struct lgv_block  *bt = lgv_block_new_constant_int(&gc, 12);
    struct lgv_block  *bf = lgv_block_new_constant_int(&gc, 43);
    struct lgv_block  *b0 = lgv_block_new_if(&gc, bc, bt, bf);
    test_expr(b0, int, si, 12, "%d");

    cork_gc_decref(&gc, b0);
    cork_gc_done(&gc);
    cork_allocator_free(alloc);
}
END_TEST

START_TEST(test_if_false)
{
    DESCRIBE_TEST;
    struct cork_alloc  *alloc = cork_allocator_new_debug();
    struct cork_gc  gc;
    cork_gc_init(&gc, alloc);

    struct lgv_block  *bc = lgv_block_new_constant_bool(&gc, false);
    struct lgv_block  *bt = lgv_block_new_constant_int(&gc, 12);
    struct lgv_block  *bf = lgv_block_new_constant_int(&gc, 43);
    struct lgv_block  *b0 = lgv_block_new_if(&gc, bc, bt, bf);
    test_expr(b0, int, si, 43, "%d");

    cork_gc_decref(&gc, b0);
    cork_gc_done(&gc);
    cork_allocator_free(alloc);
}
END_TEST


/*-----------------------------------------------------------------------
 * Testing harness
 */

Suite *
test_suite()
{
    Suite  *s = suite_create("blocks");

    TCase  *tc_constants = tcase_create("constants");
    tcase_add_test(tc_constants, test_constant_bool_false);
    tcase_add_test(tc_constants, test_constant_bool_true);
    tcase_add_test(tc_constants, test_constant_int_0);
    tcase_add_test(tc_constants, test_constant_int_1);
    tcase_add_test(tc_constants, test_constant_int__1);
    tcase_add_test(tc_constants, test_constant_int_100);
    tcase_add_test(tc_constants, test_constant_int__100);
    tcase_add_test(tc_constants, test_constant_int_INT_MIN);
    tcase_add_test(tc_constants, test_constant_int_INT_MAX);
    suite_add_tcase(s, tc_constants);

    TCase  *tc_if = tcase_create("if");
    tcase_add_test(tc_if, test_if_true);
    tcase_add_test(tc_if, test_if_false);
    suite_add_tcase(s, tc_if);

    return s;
}


int
main(int argc, const char **argv)
{
    int  number_failed;
    Suite  *suite = test_suite();
    SRunner  *runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (number_failed == 0)? EXIT_SUCCESS: EXIT_FAILURE;
}

