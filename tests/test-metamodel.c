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
 * Class names
 */

static void
test_class_name(struct swan *s, struct swan_object *obj, const char *expected)
{
    struct swan_object  *class_name;
    struct swan_static_string  *str;
    fail_if_error(class_name = swan_object_call_method
                  (s, obj->c, "name", 1, &obj, 1, NULL));
    _ensure_class_(str, class_name, swan_static_string);
    fail_unless(strcmp(str->contents, expected) == 0,
                "Unexpected class name: got %s, expected %s",
                str->contents, expected);
    cork_gc_decref(str);
}

START_TEST(test_class_name_01)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct swan_object  *prelude;
    struct swan_object  *prelude_class;
    struct swan_object  *prelude_mclass;
    struct swan_object  *prelude_mmclass;
    struct swan_object  *prelude_mmmclass;
    prelude = swan_prelude_new(&s);
    fail_if_error(prelude_class = swan_object_call_method
                  (&s, prelude->c, "class",
                   1, &prelude, 1, NULL));
    fail_if_error(prelude_mclass = swan_object_call_method
                  (&s, prelude_class->c, "class",
                   1, &prelude_class, 1, NULL));
    fail_if_error(prelude_mmclass = swan_object_call_method
                  (&s, prelude_mclass->c, "class",
                   1, &prelude_mclass, 1, NULL));
    fail_if_error(prelude_mmmclass = swan_object_call_method
                  (&s, prelude_mmclass->c, "class",
                   1, &prelude_mmclass, 1, NULL));
    test_class_name(&s, prelude_class, "prelude");
    test_class_name(&s, prelude_mclass, "swan_gc_metaclass");
    test_class_name(&s, prelude_mmclass, "swan_metaclass__class");
    test_class_name(&s, prelude_mmmclass, "swan_static_metaclass");

    cork_gc_decref(prelude->c);
    free(prelude);
    CLEANUP_SWAN;
}
END_TEST

START_TEST(test_class_name_02)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    struct swan_object  *prelude;
    struct swan_object  *string_class;
    struct swan_object  *string_mclass;
    struct swan_object  *string_mmclass;
    struct swan_object  *string_mmmclass;
    prelude = swan_prelude_new(&s);
    fail_if_error(string_class = swan_object_call_method
                  (&s, prelude->c, "string", 0, NULL, 1, NULL));
    fail_if_error(string_mclass = swan_object_call_method
                  (&s, string_class->c, "class",
                   1, &string_class, 1, NULL));
    fail_if_error(string_mmclass = swan_object_call_method
                  (&s, string_mclass->c, "class",
                   1, &string_mclass, 1, NULL));
    fail_if_error(string_mmmclass = swan_object_call_method
                  (&s, string_mmclass->c, "class",
                   1, &string_mmclass, 1, NULL));
    test_class_name(&s, string_class, "swan_static_string");
    test_class_name(&s, string_mclass, "swan_static_metaclass");
    test_class_name(&s, string_mmclass, "swan_metaclass__class");
    test_class_name(&s, string_mmmclass, "swan_static_metaclass");

    cork_gc_decref(prelude->c);
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
    tcase_add_test(tc_metamodel, test_class_name_01);
    tcase_add_test(tc_metamodel, test_class_name_02);
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
