/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
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

#include "swanson/helpers.h"
#include "swanson/metamodel.h"
#include "swanson/prelude.h"
#include "swanson/state.h"

#include "helpers.h"


/*-----------------------------------------------------------------------
 * Breed names
 */

static void
test_breed_name(struct swan *s, struct swan_thing *t, const char *expected)
{
    struct swan_thing  *breed_name;
    struct swan_static_string  *str;
    fail_if_error(breed_name = swan_thing_call_method
                  (s, t->b, "name", 1, &t, 1, NULL));
    _ensure_breed_(str, breed_name, swan_static_string);
    fail_unless(strcmp(str->contents, expected) == 0,
                "Unexpected breed name: got %s, expected %s",
                str->contents, expected);
    cork_gc_decref(str);
}

START_TEST(test_breed_name_01)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct swan_thing  *prelude;
    struct swan_thing  *prelude_breed;
    struct swan_thing  *prelude_mbreed;
    struct swan_thing  *prelude_mmbreed;
    struct swan_thing  *prelude_mmmbreed;
    prelude = swan_prelude_new(&s);
    fail_if_error(prelude_breed = swan_thing_call_method
                  (&s, prelude->b, "breed",
                   1, &prelude, 1, NULL));
    fail_if_error(prelude_mbreed = swan_thing_call_method
                  (&s, prelude_breed->b, "breed",
                   1, &prelude_breed, 1, NULL));
    fail_if_error(prelude_mmbreed = swan_thing_call_method
                  (&s, prelude_mbreed->b, "breed",
                   1, &prelude_mbreed, 1, NULL));
    fail_if_error(prelude_mmmbreed = swan_thing_call_method
                  (&s, prelude_mmbreed->b, "breed",
                   1, &prelude_mmbreed, 1, NULL));
    test_breed_name(&s, prelude_breed, "prelude");
    test_breed_name(&s, prelude_mbreed, "swan_gc_metabreed");
    test_breed_name(&s, prelude_mmbreed, "swan_metabreed__breed");
    test_breed_name(&s, prelude_mmmbreed, "swan_static_metabreed");

    cork_gc_decref(prelude->b);
    free(prelude);
    CLEANUP_SWAN;
}
END_TEST

START_TEST(test_breed_name_02)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct swan_thing  *prelude;
    struct swan_thing  *string_breed;
    struct swan_thing  *string_mbreed;
    struct swan_thing  *string_mmbreed;
    struct swan_thing  *string_mmmbreed;
    prelude = swan_prelude_new(&s);
    fail_if_error(string_breed = swan_thing_call_method
                  (&s, prelude->b, "string", 0, NULL, 1, NULL));
    fail_if_error(string_mbreed = swan_thing_call_method
                  (&s, string_breed->b, "breed",
                   1, &string_breed, 1, NULL));
    fail_if_error(string_mmbreed = swan_thing_call_method
                  (&s, string_mbreed->b, "breed",
                   1, &string_mbreed, 1, NULL));
    fail_if_error(string_mmmbreed = swan_thing_call_method
                  (&s, string_mmbreed->b, "breed",
                   1, &string_mmbreed, 1, NULL));
    test_breed_name(&s, string_breed, "swan_static_string");
    test_breed_name(&s, string_mbreed, "swan_static_metabreed");
    test_breed_name(&s, string_mmbreed, "swan_metabreed__breed");
    test_breed_name(&s, string_mmmbreed, "swan_static_metabreed");

    cork_gc_decref(prelude->b);
    free(prelude);
    CLEANUP_SWAN;
}
END_TEST


/*-----------------------------------------------------------------------
 * Testing harness
 */

Suite *
test_suite()
{
    Suite  *s = suite_create("metamodel");

    TCase  *tc_metamodel = tcase_create("metamodel");
    tcase_add_test(tc_metamodel, test_breed_name_01);
    tcase_add_test(tc_metamodel, test_breed_name_02);
    suite_add_tcase(s, tc_metamodel);

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
