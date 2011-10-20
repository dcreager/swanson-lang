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
    struct cork_alloc  *alloc = cork_allocator_new_debug();
    struct swan  s;
    swan_init(&s, alloc);

    struct swan_scope  *s0;
    struct swan_scope  *s1;
    struct swan_obj  *out;

    fail_if_error(s0 = swan_scope_new(&s, "root", NULL, &err));
    fail_if_error(s1 = swan_scope_new(&s, "prelude", s0, &err));
    fail_if_error(swan_scope_add(&s, s0, "prelude", swan_scope_obj(s1), &err));

    fail_if_error(out = swan_scope_get(&s, s0, "prelude", &err));
    fail_unless(out == swan_scope_obj(s1),
                "Unexpected root.prelude scope");
    fail_unless_error(out = swan_scope_get(&s, s0, "undefined", &err),
                      "Unexpected root.undefined scope");

    fail_if_error(out = swan_scope_get(&s, s1, "prelude", &err));
    fail_unless(out == swan_scope_obj(s1),
                "Unexpected prelude.prelude scope");
    fail_unless_error(out = swan_scope_get(&s, s1, "undefined", &err),
                      "Unexpected prelude.undefined scope");

    cork_gc_decref(swan_gc(&s), s0);

    swan_done(&s);
    cork_allocator_free(alloc);
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
