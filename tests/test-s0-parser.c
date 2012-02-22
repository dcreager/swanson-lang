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
#include <libcork/core.h>
#include <libcork/ds.h>

#include "swanson/ast.h"
#include "swanson/ds.h"
#include "swanson/metamodel.h"
#include "swanson/state.h"

#include "helpers.h"


/*-----------------------------------------------------------------------
 * Helpers
 */

static void
check_good_parse_(struct swan *s, const char *buf)
{
    struct swan_sllist  *call;
    fail_if_error(call = swan_ast_parse(s, buf, strlen(buf)));
    cork_gc_decref(call);
}

#define check_good_parse(buf)  check_good_parse_(&s, buf)


/*-----------------------------------------------------------------------
 * Parsing
 */

START_TEST(test_parsing)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    check_good_parse("  prelude.foo();  ");
    check_good_parse("  prelude.foo(x1);  ");
    check_good_parse("  prelude.foo(x1, x2);  ");

    check_good_parse("  x = prelude.foo();  ");
    check_good_parse("  x = prelude.foo(x1);  ");
    check_good_parse("  x = prelude.foo(x1, x2);  ");

    check_good_parse("  x, y = prelude.foo();  ");
    check_good_parse("  x, y = prelude.foo(x1);  ");
    check_good_parse("  x, y = prelude.foo(x1, x2);  ");

    check_good_parse("  \"x y\", y = prelude.\"foo\"();  ");
    check_good_parse("  \"x y\", y = prelude.\"foo\"(x1);  ");
    check_good_parse("  \"x y\", y = prelude.\"foo ->\"(x1, \"x2 y\");  ");

    CLEANUP_SWAN;
}
END_TEST


/*-----------------------------------------------------------------------
 * Testing harness
 */

Suite *
test_suite()
{
    Suite  *s = suite_create("s0-parser");

    TCase  *tc_parser = tcase_create("parser");
    tcase_add_test(tc_parser, test_parsing);
    suite_add_tcase(s, tc_parser);

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
