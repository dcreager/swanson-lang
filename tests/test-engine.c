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

#include "swanson/engine.h"
#include "swanson/state.h"
#include "swanson/swanson0.h"
#include "lagavulin/engine.h"

#include "helpers.h"


/*-----------------------------------------------------------------------
 * Helper functions
 */

#define test_expr(expr, typ, union_branch, expected, fmt) \
    do { \
        struct lgv_state  *state = lgv_engine_state(s->engine); \
        fail_if_error(swan_execute(s, expr)); \
        typ  actual = \
            lgv_stack_get(&state->stack, state->stack.top, -1, union_branch); \
        fail_unless(actual == expected, \
                    "Unexpected result: got " fmt ", expected " fmt, \
                    actual, expected); \
    } while (0)


/*-----------------------------------------------------------------------
 * Constants
 */

#define test_constant(typ_id, typ, union_branch, value, name, fmt) \
    START_TEST(test_constant_##typ_id##_##name) \
    { \
        DESCRIBE_TEST; \
        DECLARE_LGV; \
        struct swan_scope  *kernel; \
        struct swan_string  *str; \
        struct swan_obj  *obj; \
        struct swan_macro  *macro; \
        struct swan_expression  *expr; \
        fail_if_error(kernel = swan_create_kernel(s)); \
        fail_if_error(str = swan_string_new(s, #value, sizeof(#value))); \
        fail_if_error(obj = swan_scope_get(s, kernel, #typ ":int-literal")); \
        fail_unless(swan_is_macro(obj)); \
        macro = swan_obj_macro(obj); \
        fail_if_error(expr = swan_macro_execute \
                      (s, macro, 1, swan_string_obj(str))); \
        test_expr(expr, typ, union_branch, value, fmt); \
        cork_gc_decref(expr); \
        cork_gc_decref(str); \
        cork_gc_decref(kernel); \
        CLEANUP_LGV; \
    } \
    END_TEST

test_constant(int, int, si, 0, 0, "%d");
test_constant(int, int, si, 1, 1, "%d");
test_constant(int, int, si, -1, _1, "%d");
test_constant(int, int, si, 100, 100, "%d");
test_constant(int, int, si, -100, _100, "%d");

START_TEST(test_add_int_01)
{
    DESCRIBE_TEST;
    DECLARE_LGV;

    struct swan_scope  *kernel;
    struct swan_string  *str;
    struct swan_obj  *obj;
    struct swan_macro  *macro;
    struct swan_expression  *lhs;
    struct swan_expression  *rhs;
    struct swan_expression  *expr;

    fail_if_error(s = lgv_new());
    fail_if_error(kernel = swan_create_kernel(s));

    fail_if_error(obj = swan_scope_get(s, kernel, "int:int-literal"));
    fail_unless(swan_is_macro(obj));
    macro = swan_obj_macro(obj);

    fail_if_error(str = swan_string_new(s, "42", sizeof("42")));
    fail_if_error(lhs = swan_macro_execute
                  (s, macro, 1, swan_string_obj(str)));
    cork_gc_decref(str);

    fail_if_error(str = swan_string_new(s, "-10", sizeof("-10")));
    fail_if_error(rhs = swan_macro_execute
                  (s, macro, 1, swan_string_obj(str)));
    cork_gc_decref(str);

    fail_if_error(obj = swan_scope_get(s, kernel, "int:add"));
    fail_unless(swan_is_macro(obj));
    macro = swan_obj_macro(obj);

    fail_if_error(expr = swan_macro_execute
                  (s, macro, 2,
                   swan_expression_obj(lhs),
                   swan_expression_obj(rhs)));

    test_expr(expr, int, si, 32, "%d");

    cork_gc_decref(expr);
    cork_gc_decref(kernel);
    CLEANUP_LGV;
}
END_TEST


/*-----------------------------------------------------------------------
 * Testing harness
 */

Suite *
test_suite()
{
    Suite  *s = suite_create("lgv-engine");

    TCase  *tc_constants = tcase_create("constants");
    tcase_add_test(tc_constants, test_constant_int_0);
    tcase_add_test(tc_constants, test_constant_int_1);
    tcase_add_test(tc_constants, test_constant_int__1);
    tcase_add_test(tc_constants, test_constant_int_100);
    tcase_add_test(tc_constants, test_constant_int__100);
    suite_add_tcase(s, tc_constants);

    TCase  *tc_arithmetic = tcase_create("arithmetic");
    tcase_add_test(tc_arithmetic, test_add_int_01);
    suite_add_tcase(s, tc_arithmetic);

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
