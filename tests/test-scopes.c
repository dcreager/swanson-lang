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


/*-----------------------------------------------------------------------
 * Helper functions
 */

#define DESCRIBE_TEST \
    fprintf(stderr, "--- %s\n", __func__);


/*-----------------------------------------------------------------------
 * Scopes
 */

START_TEST(test_scope_01)
{
    DESCRIBE_TEST;
    struct cork_alloc  *alloc = cork_allocator_new_debug();
    struct swan  s;
    swan_init(&s, alloc);

    struct swan_scope  *s0 = swan_scope_new(&s, "root", NULL);
    struct swan_scope  *s1 = swan_scope_new(&s, "prelude", NULL);
    swan_scope_add(&s, s0, "prelude", swan_scope_obj(s1), NULL);
    fail_unless(swan_scope_get(&s, s0, "prelude", NULL) == swan_scope_obj(s1),
                "Unexpected root.prelude scope");
    fail_unless(swan_scope_get(&s, s0, "undefined", NULL) == NULL,
                "Unexpected root.undefined scope");
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
    Suite  *s = suite_create("blocks");

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

