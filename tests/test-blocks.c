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

#define test_result(block, typ, expected, fmt) \
    do { \
        void  *dest = NULL; \
        struct lgv_block  *result = \
            lgv_block_new_collect(alloc, &dest); \
        lgv_block_set_next(block, result); \
        lgv_block_execute(block, result); \
        typ  actual = *((typ *) dest); \
        fail_unless(actual == expected, \
                    "Unexpected result: got " fmt ", expected " fmt, \
                    actual, expected); \
    } while (0)


/*-----------------------------------------------------------------------
 * Constants
 */

#define test_constant(typ_id, typ, value, name, fmt) \
    START_TEST(test_constant_##typ_id##_##name) \
    { \
        DESCRIBE_TEST; \
        cork_allocator_t  *alloc = cork_allocator_new_debug(); \
        struct lgv_block  *b0 = lgv_block_new_constant_##typ_id(alloc, value); \
        test_result(b0, typ, value, fmt); \
        cork_allocator_free(alloc); \
    } \
    END_TEST

test_constant(bool, bool, false, false, "%d");
test_constant(bool, bool, true, true, "%d");

test_constant(int, int, 0, 0, "%d");
test_constant(int, int, 1, 1, "%d");
test_constant(int, int, -1, _1, "%d");
test_constant(int, int, 100, 100, "%d");
test_constant(int, int, -100, _100, "%d");
test_constant(int, int, INT_MIN, INT_MIN, "%d");
test_constant(int, int, INT_MAX, INT_MAX, "%d");


/*-----------------------------------------------------------------------
 * If statement
 */

START_TEST(test_if_true)
{
    DESCRIBE_TEST;
    cork_allocator_t  *alloc = cork_allocator_new_debug();
    struct lgv_block  *bc = lgv_block_new_constant_bool(alloc, true);
    struct lgv_block  *bt = lgv_block_new_constant_int(alloc, 12);
    struct lgv_block  *bf = lgv_block_new_constant_int(alloc, 43);
    struct lgv_block  *b0 = lgv_block_new_if(alloc, bc, bt, bf);
    test_result(b0, int, 12, "%d");
    cork_allocator_free(alloc);
}
END_TEST

START_TEST(test_if_false)
{
    DESCRIBE_TEST;
    cork_allocator_t  *alloc = cork_allocator_new_debug();
    struct lgv_block  *bc = lgv_block_new_constant_bool(alloc, false);
    struct lgv_block  *bt = lgv_block_new_constant_int(alloc, 12);
    struct lgv_block  *bf = lgv_block_new_constant_int(alloc, 43);
    struct lgv_block  *b0 = lgv_block_new_if(alloc, bc, bt, bf);
    test_result(b0, int, 43, "%d");
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

