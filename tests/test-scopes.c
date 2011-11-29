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

#include "swanson/swanson0.h"
#include "swanson/state.h"

#include "helpers.h"


/*-----------------------------------------------------------------------
 * Scopes
 */

START_TEST(test_scope_01)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct swan_scope  *s0;
    struct swan_scope  *s1;
    struct swan_string  *str1;
    struct swan_obj  *out;

    fail_if_error(s0 = swan_scope_new(&s, "root", NULL, &err));
    fail_if_error(s1 = swan_scope_new(&s, "kernel", s0, &err));
    fail_if_error(str1 = swan_string_new(&s, "hiya", 4, &err));
    fail_if_error(swan_scope_add(&s, s0, "kernel", swan_scope_obj(s1), &err));
    fail_if_error(swan_scope_add(&s, s1, "name", swan_string_obj(str1), &err));

    fail_if_error(out = swan_scope_get(&s, s0, "kernel", &err));
    fail_unless(out == swan_scope_obj(s1),
                "Unexpected root.kernel scope");
    fail_unless_error(out = swan_scope_get(&s, s0, "undefined", &err),
                      "Unexpected root.undefined scope");

    fail_if_error(out = swan_scope_get(&s, s1, "kernel", &err));
    fail_unless(out == swan_scope_obj(s1),
                "Unexpected kernel.kernel scope");
    fail_unless_error(out = swan_scope_get(&s, s1, "undefined", &err),
                      "Unexpected kernel.undefined scope");

    fail_if_error(out = swan_scope_get(&s, s1, "name", &err));
    fail_unless(out == swan_string_obj(str1),
                "Unexpected kernel.name string");

    cork_gc_decref(swan_gc(&s), s0);

    CLEANUP_SWAN;
}
END_TEST


/*-----------------------------------------------------------------------
 * Testing harness
 */

Suite *
test_suite()
{
    Suite  *s = suite_create("scopes");

    TCase  *tc_scope = tcase_create("scope");
    tcase_add_test(tc_scope, test_scope_01);
    suite_add_tcase(s, tc_scope);

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
