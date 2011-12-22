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

#include "swanson/s0.h"
#include "swanson/state.h"

#include "helpers.h"


/*-----------------------------------------------------------------------
 * Helpers
 */

#define check_good_parse(buf) \
    do { \
        struct cork_buffer  *b; \
        struct cork_slice  slice; \
        struct s0_basic_block  *result; \
        fail_if_error(b = cork_buffer_new(alloc, &err)); \
        fail_if_error(cork_buffer_set_string(alloc, b, buf, &err)); \
        fail_if_error(cork_buffer_to_slice(alloc, b, &slice, &err)); \
        fail_if_error(result = s0_parse(&s, &slice, &err)); \
        cork_gc_decref(swan_gc(&s), result); \
        cork_slice_finish(alloc, &slice); \
    } while (0)

#define check_bad_parse(buf) \
    do { \
        struct cork_buffer  *b; \
        struct cork_slice  slice; \
        struct s0_basic_block  *result = NULL; \
        fail_if_error(b = cork_buffer_new(alloc, &err)); \
        fail_if_error(cork_buffer_set_string(alloc, b, buf, &err)); \
        fail_if_error(cork_buffer_to_slice(alloc, b, &slice, &err)); \
        fail_unless_error(result = s0_parse(&s, &slice, &err), \
                          "Parse shouldn't succeed for " buf); \
        cork_gc_decref(swan_gc(&s), result); \
        cork_slice_finish(alloc, &slice); \
    } while (0)


/*-----------------------------------------------------------------------
 * Parsing
 */

START_TEST(test_parsing)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    check_good_parse("tliteral %0");

    check_bad_parse("foobar");
    check_bad_parse("tliteral");
    check_bad_parse("tliteral $0");
    check_bad_parse("tliteral foo");

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
