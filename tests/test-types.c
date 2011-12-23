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

#include "swanson/s0.h"
#include "swanson/state.h"

#include "helpers.h"


/*-----------------------------------------------------------------------
 * Helpers
 */

#define check_type(t, expected, expected_givens) \
    do { \
        struct cork_buffer  actual = CORK_BUFFER_INIT(alloc); \
        struct cork_buffer  givens = CORK_BUFFER_INIT(alloc); \
        fail_if_error(cork_buffer_set_string(alloc, &actual, "", &err)); \
        fail_if_error(cork_buffer_set_string(alloc, &givens, "", &err)); \
        fail_if_error(s0_type_print(&s, t, &actual, &givens, &err)); \
        if ((strcmp(actual.buf, expected) != 0) || \
            (strcmp(givens.buf, expected_givens) != 0)) { \
            fail("Unexpected type: %s\n\n%s", \
                 (char *) actual.buf, (char *) givens.buf); \
        } \
    } while (0)


/*-----------------------------------------------------------------------
 * Literals
 */

START_TEST(test_literal)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct s0_type  *t0;
    fail_if_error(t0 = s0_literal_type_new(&s, &err));
    check_type(t0, "LITERAL", "");

    cork_gc_decref(swan_gc(&s), t0);
    CLEANUP_SWAN;
}
END_TEST


/*-----------------------------------------------------------------------
 * Locations
 */

START_TEST(test_location_01)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct s0_type  *t0;
    struct s0_type  *t1;
    fail_if_error(t1 = s0_literal_type_new(&s, &err));
    fail_if_error(t0 = s0_location_type_new(&s, t1, &err));
    check_type(t0, "*LITERAL", "");

    cork_gc_decref(swan_gc(&s), t0);
    cork_gc_decref(swan_gc(&s), t1);
    CLEANUP_SWAN;
}
END_TEST


/*-----------------------------------------------------------------------
 * Functions
 */

START_TEST(test_function_01)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct s0_type  *t0;
    struct s0_type  *t1;
    fail_if_error(t0 = s0_function_type_new(&s, &err));
    fail_if_error(t1 = s0_literal_type_new(&s, &err));
    fail_if_error(s0_function_type_add_param(&s, t0, t1, &err));
    fail_if_error(s0_function_type_add_result(&s, t0, t1, &err));
    check_type(t0, "LITERAL -> LITERAL", "");

    cork_gc_decref(swan_gc(&s), t0);
    cork_gc_decref(swan_gc(&s), t1);
    CLEANUP_SWAN;
}
END_TEST

START_TEST(test_function_02)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct s0_type  *t0;
    struct s0_type  *t1;
    fail_if_error(t0 = s0_function_type_new(&s, &err));
    fail_if_error(t1 = s0_literal_type_new(&s, &err));
    fail_if_error(s0_function_type_add_param(&s, t0, t1, &err));
    fail_if_error(s0_function_type_add_param(&s, t0, t1, &err));
    fail_if_error(s0_function_type_add_result(&s, t0, t1, &err));
    check_type(t0, "LITERAL,LITERAL -> LITERAL", "");

    cork_gc_decref(swan_gc(&s), t0);
    cork_gc_decref(swan_gc(&s), t1);
    CLEANUP_SWAN;
}
END_TEST


/*-----------------------------------------------------------------------
 * Interfaces
 */

START_TEST(test_interface_01)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct s0_type  *t0;
    struct s0_type  *t1;
    fail_if_error(t1 = s0_literal_type_new(&s, &err));
    fail_if_error(t0 = s0_interface_type_new(&s, &err));
    fail_if_error(s0_interface_type_add(&s, t0, "lit", t1, &err));
    cork_gc_decref(swan_gc(&s), t1);
    check_type(t0, "α",
        "interface α {\n"
        "  lit LITERAL\n"
        "}\n"
    );

    cork_gc_decref(swan_gc(&s), t0);
    CLEANUP_SWAN;
}
END_TEST

START_TEST(test_interface_02)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct s0_type  *tr;
    struct s0_type  *t0;
    struct s0_type  *t1;

    fail_if_error(tr = s0_recursive_type_new(&s, &err));
    fail_if_error(t0 = s0_interface_type_new(&s, &err));

    fail_if_error(t1 = s0_function_type_new(&s, &err));
    fail_if_error(s0_function_type_add_param(&s, t1, tr, &err));
    fail_if_error(s0_function_type_add_param(&s, t1, tr, &err));
    fail_if_error(s0_function_type_add_result(&s, t1, tr, &err));
    fail_if_error(s0_interface_type_add(&s, t0, "+", t1, &err));
    cork_gc_decref(swan_gc(&s), t1);

    fail_if_error(s0_recursive_type_resolve(&s, tr, t0, &err));

    check_type(t0, "α",
        "interface α {\n"
        "  + (α,α -> α)\n"
        "}\n"
    );

    cork_gc_decref(swan_gc(&s), t0);
    cork_gc_decref(swan_gc(&s), tr);
    CLEANUP_SWAN;
}
END_TEST

START_TEST(test_interface_03)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct s0_type  *tr;
    struct s0_type  *tl;
    struct s0_type  *t0;
    struct s0_type  *t1;

    fail_if_error(tr = s0_recursive_type_new(&s, &err));
    fail_if_error(tl = s0_location_type_new(&s, tr, &err));
    fail_if_error(t0 = s0_interface_type_new(&s, &err));

    fail_if_error(t1 = s0_function_type_new(&s, &err));
    fail_if_error(s0_function_type_add_param(&s, t1, tl, &err));
    fail_if_error(s0_function_type_add_param(&s, t1, tr, &err));
    fail_if_error(s0_interface_type_add(&s, t0, "=", t1, &err));
    cork_gc_decref(swan_gc(&s), t1);

    fail_if_error(t1 = s0_function_type_new(&s, &err));
    fail_if_error(s0_function_type_add_param(&s, t1, tl, &err));
    fail_if_error(s0_function_type_add_result(&s, t1, tr, &err));
    fail_if_error(s0_interface_type_add(&s, t0, "unary *", t1, &err));
    cork_gc_decref(swan_gc(&s), t1);

    fail_if_error(s0_recursive_type_resolve(&s, tr, t0, &err));

    check_type(t0, "α",
        "interface α {\n"
        "  = (*α,α -> void)\n"
        "  unary * (*α -> α)\n"
        "}\n"
    );

    cork_gc_decref(swan_gc(&s), t0);
    cork_gc_decref(swan_gc(&s), tr);
    CLEANUP_SWAN;
}
END_TEST

START_TEST(test_interface_no_duplicates)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct s0_type  *t0;
    struct s0_type  *t1;
    fail_if_error(t1 = s0_literal_type_new(&s, &err));
    fail_if_error(t0 = s0_interface_type_new(&s, &err));
    fail_if_error(s0_interface_type_add(&s, t0, "lit", t1, &err));
    fail_unless_error(s0_interface_type_add(&s, t0, "lit", t1, &err),
                      "Shouldn't be able to add a duplicate entry");
    cork_gc_decref(swan_gc(&s), t1);
    cork_gc_decref(swan_gc(&s), t0);
    CLEANUP_SWAN;
}
END_TEST


/*-----------------------------------------------------------------------
 * Blocks
 */

START_TEST(test_block_01)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct s0_type  *t0;
    struct s0_type  *t1;
    fail_if_error(t1 = s0_literal_type_new(&s, &err));
    fail_if_error(t0 = s0_block_type_new(&s, t1, &err));
    check_type(t0, "{LITERAL}", "");

    cork_gc_decref(swan_gc(&s), t0);
    cork_gc_decref(swan_gc(&s), t1);
    CLEANUP_SWAN;
}
END_TEST


/*-----------------------------------------------------------------------
 * Testing harness
 */

Suite *
test_suite()
{
    Suite  *s = suite_create("types");

    TCase  *tc_types = tcase_create("types");
    tcase_add_test(tc_types, test_literal);
    tcase_add_test(tc_types, test_location_01);
    tcase_add_test(tc_types, test_function_01);
    tcase_add_test(tc_types, test_function_02);
    tcase_add_test(tc_types, test_interface_01);
    tcase_add_test(tc_types, test_interface_02);
    tcase_add_test(tc_types, test_interface_03);
    tcase_add_test(tc_types, test_interface_no_duplicates);
    tcase_add_test(tc_types, test_block_01);
    suite_add_tcase(s, tc_types);

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
