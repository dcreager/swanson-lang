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

static void
check_good_parse_(struct swan *s, const char *buf)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_buffer  *b;
    struct cork_slice  slice;
    struct s0_basic_block  *result;

    fail_if_error(b = cork_buffer_new(alloc, &err));
    fail_if_error(cork_buffer_set_string(alloc, b, buf, &err));
    fail_if_error(cork_buffer_to_slice(alloc, b, &slice, &err));
    fail_if_error(result = s0_parse(s, &slice, &err));
    cork_gc_decref(swan_gc(s), result);
    cork_slice_finish(alloc, &slice);
}

#define check_good_parse(buf)  check_good_parse_(&s, buf)

static void
check_bad_parse_(struct swan *s, const char *buf)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_buffer  *b;
    struct cork_slice  slice;
    struct s0_basic_block  *result;

    fail_if_error(b = cork_buffer_new(alloc, &err));
    fail_if_error(cork_buffer_set_string(alloc, b, buf, &err));
    fail_if_error(cork_buffer_to_slice(alloc, b, &slice, &err));
    fail_unless_error(result = s0_parse(s, &slice, &err),
                      "Parse shouldn't succeed for %s", buf);
    cork_gc_decref(swan_gc(s), result);
    cork_slice_finish(alloc, &slice);
}

#define check_bad_parse(buf)  check_bad_parse_(&s, buf)


/*-----------------------------------------------------------------------
 * Parsing
 */

START_TEST(test_parsing)
{
    DESCRIBE_TEST;
    DECLARE_SWAN;

    check_good_parse("  trecursive %0;");
    check_good_parse("trecursive %0;  ");
    check_good_parse("  trecursive %0;  ");
    check_good_parse("# comment\ntrecursive %0;");
    check_good_parse("trecursive %0;# comment\n");
    check_good_parse("# comment\ntrecursive %0;# comment\n");

    check_good_parse("trecursive %0;");
    check_good_parse("ttype %0;");
    check_good_parse("tliteral %0;");
    check_good_parse("tany %0;");
    check_good_parse(
        "tliteral %0; "
        "tlocation %1 = %0; "
    );
    check_good_parse(
        "tliteral %0; "
        "tlocation %1 = %0; "
        "tproduct %2 = (%0); "
    );
    check_good_parse(
        "tliteral %0; "
        "tlocation %1 = %0; "
        "tproduct %2 = (); "
        "tproduct %3 = (%0); "
        "tfunction %4 = %2 -> %3; "
    );
    check_good_parse(
        "tliteral %0; "
        "tlocation %1 = %0; "
        "tproduct %2 = (%1, %1); "
        "tproduct %3 = (%0); "
        "tfunction %4 = %2 -> %3; "
    );

    check_good_parse(
        "tinterface %0 = {}; "
    );
    check_good_parse(
        "tliteral %0; "
        "tinterface %1 = { \"hi\": %0 }; "
    );
    check_good_parse(
        "tliteral %0; "
        "tinterface %1 = { \"hi\": %0, \"there\": %0 }; "
    );
    check_good_parse(
        "tliteral %0; "
        "tinterface %1 = { \"h\\\"i\": %0 }; "
    );
    check_good_parse(
        "tliteral %0; "
        "tinterface %1 = { \"h\\\\i\": %0 }; "
    );

    check_good_parse(
        "tclass %0 = \"class\" {}; "
    );
    check_good_parse(
        "tliteral %0; "
        "tclass %1 = \"class\" { \"hi\": %0 }; "
    );
    check_good_parse(
        "tliteral %0; "
        "tclass %1 = \"class\" { \"hi\": %0, \"there\": %0 }; "
    );
    check_good_parse(
        "tliteral %0; "
        "tclass %1 = \"class\" { \"h\\\"i\": %0 }; "
    );
    check_good_parse(
        "tliteral %0; "
        "tclass %1 = \"class\" { \"h\\\\i\": %0 }; "
    );

    check_good_parse(
        "tliteral %0; "
        "tblock %1 = %0; "
    );
    check_good_parse(
        "tliteral %0; "
        "tblock %1 = %0; "
        "tproduct %2 = (); "
        "tproduct %3 = (%0); "
        "tfunction %4 = %2 -> %3; "
    );
    check_good_parse(
        "tliteral %0; "
        "tblock %1 = %0; "
        "tproduct %2 = (%1, %1); "
        "tproduct %3 = (%0); "
        "tfunction %4 = %2 -> %3; "
    );

    check_good_parse(
        "ttype %0; "
        "tlocation %1 = %0; "
    );

    check_good_parse("literal $0 = \"hello world\";");

    check_good_parse("tuple $0 = ();");
    check_good_parse("tuple $0 = ($1);");
    check_good_parse("tuple $0 = ($1,$2);");

    check_good_parse("gettuple $0 = $0.0;");
    check_good_parse("gettuple $0 = $0.1;");

    check_good_parse("getclass $0 = $0 \"foo\";");
    check_good_parse("getclass $0 = $0 \"bar baz\";");

    check_good_parse(
        "macro $0 = \"test\" %1 -> %0 {}; "
    );
    check_good_parse(
        "macro $0 = \"test\" %1 -> %0 { "
        "  literal $0 = \"hello\"; "
        "}; "
    );

    check_good_parse(
        "macro $0 = \"test\" upvalue ($0) %1 -> %0 {}; "
    );
    check_good_parse(
        "macro $0 = \"test\" upvalue ($0) %1 -> %0 { "
        "  literal $0 = \"hello\"; "
        "}; "
    );

    check_good_parse("call $0 = $1($2);");
    check_good_parse("return $0;");

    check_bad_parse("foobar");

    check_bad_parse("trecursive");
    check_bad_parse("trecursive %0");
    check_bad_parse("trecursive $0;");
    check_bad_parse("trecursive foo;");

    check_bad_parse("ttype");
    check_bad_parse("ttype %0");
    check_bad_parse("ttype $0;");
    check_bad_parse("ttype foo;");

    check_bad_parse("tliteral");
    check_bad_parse("tliteral %0");
    check_bad_parse("tliteral $0;");
    check_bad_parse("tliteral foo;");

    check_bad_parse("tany");
    check_bad_parse("tany %0");
    check_bad_parse("tany $0;");
    check_bad_parse("tany foo;");

    check_bad_parse("tlocation");
    check_bad_parse("tlocation %1");
    check_bad_parse("tlocation %1 =");
    check_bad_parse("tlocation %1 = %1");
    check_bad_parse("tlocation $1 = %0;");
    check_bad_parse("tlocation foo;");

    check_bad_parse("tproduct");
    check_bad_parse("tproduct %1");
    check_bad_parse("tproduct %1 =");
    check_bad_parse("tproduct %1 = (");
    check_bad_parse("tproduct %1 = ()");
    check_bad_parse("tproduct %1 = (%2)");
    check_bad_parse("tproduct %1 = %2;");
    check_bad_parse("tproduct $1 = ();");
    check_bad_parse("tproduct foo;");

    check_bad_parse("tfunction");
    check_bad_parse("tfunction %1");
    check_bad_parse("tfunction %1 =");
    check_bad_parse("tfunction %1 = %1;");
    check_bad_parse("tfunction $1 = () -> ();");
    check_bad_parse("tfunction foo;");

    check_bad_parse("tinterface");
    check_bad_parse("tinterface %1");
    check_bad_parse("tinterface %1 =");
    check_bad_parse("tinterface %1 = {");
    check_bad_parse("tinterface %1 = { \"hi");
    check_bad_parse("tinterface %1 = { \"hi\"");
    check_bad_parse("tinterface %1 = { \"hi\": ");
    check_bad_parse("tinterface %1 = { \"hi\": %0 ");
    check_bad_parse("tinterface %1 = { \"hi\": %0 }");
    check_bad_parse("tinterface %1 = %1;");
    check_bad_parse("tinterface $1 = {};");
    check_bad_parse("tinterface foo;");

    check_bad_parse("tclass");
    check_bad_parse("tclass %1");
    check_bad_parse("tclass %1 =");
    check_bad_parse("tclass %1 = \"class");
    check_bad_parse("tclass %1 = \"class\"");
    check_bad_parse("tclass %1 = \"class\" {");
    check_bad_parse("tclass %1 = \"class\" { \"hi");
    check_bad_parse("tclass %1 = \"class\" { \"hi\"");
    check_bad_parse("tclass %1 = \"class\" { \"hi\":");
    check_bad_parse("tclass %1 = \"class\" { \"hi\": %0");
    check_bad_parse("tclass %1 = \"class\" { \"hi\": %0 }");
    check_bad_parse("tclass %1 = %1;");
    check_bad_parse("tclass $1 = {};");
    check_bad_parse("tclass foo;");

    check_bad_parse("tblock");
    check_bad_parse("tblock %1");
    check_bad_parse("tblock %1 =");
    check_bad_parse("tblock %1 = %1");
    check_bad_parse("tblock $1 = %0;");
    check_bad_parse("tblock foo;");

    check_bad_parse("literal");
    check_bad_parse("literal $0");
    check_bad_parse("literal $0 = ");
    check_bad_parse("literal $0 = \"hello world");
    check_bad_parse("literal $0 = \"hello world\"");
    check_bad_parse("literal %0;");
    check_bad_parse("literal foo;");

    check_bad_parse("tuple");
    check_bad_parse("tuple $0");
    check_bad_parse("tuple $0 =");
    check_bad_parse("tuple $0 = (");
    check_bad_parse("tuple $0 = ($1");
    check_bad_parse("tuple $0 = ($1,");
    check_bad_parse("tuple $0 = ($1,$2");
    check_bad_parse("tuple $0 = ($1,$2)");
    check_bad_parse("tuple %0 = ($1,$2);");
    check_bad_parse("tuple foo;");

    check_bad_parse("gettuple");
    check_bad_parse("gettuple $0");
    check_bad_parse("gettuple $0 =");
    check_bad_parse("gettuple $0 = $0");
    check_bad_parse("gettuple $0 = $0.");
    check_bad_parse("gettuple $0 = $0.1");
    check_bad_parse("gettuple %0 = $0.1;");
    check_bad_parse("gettuple foo;");

    check_bad_parse("getclass");
    check_bad_parse("getclass $0");
    check_bad_parse("getclass $0 =");
    check_bad_parse("getclass $0 = $0");
    check_bad_parse("getclass $0 = $0 \"");
    check_bad_parse("getclass $0 = $0 \"foo");
    check_bad_parse("getclass $0 = $0 \"foo\"");
    check_bad_parse("getclass foo;");

    check_bad_parse("macro");
    check_bad_parse("macro $0");
    check_bad_parse("macro $0 =");
    check_bad_parse("macro $0 = \"test\"");
    check_bad_parse("macro $0 = \"test\" {");
    check_bad_parse("macro $0 = \"test\" {}");
    check_bad_parse("macro $0 = \"test\" upvalue");
    check_bad_parse("macro $0 = \"test\" upvalue ($0) $1");
    check_bad_parse("macro $0 = \"test\" upvalue ($0) $1 ->");
    check_bad_parse("macro $0 = \"test\" upvalue ($0) $1 -> $2");
    check_bad_parse("macro $0 = \"test\" upvalue ($0) $1 -> $2 {");
    check_bad_parse("macro $0 = \"test\" upvalue ($0) $1 -> $2 {}");
    check_bad_parse("macro $0 = \"test\" results (%0) params (%0) {}; ");
    check_bad_parse("macro $0 = \"test\" params (%0) upvalues ($0) {}; ");
    check_bad_parse("macro $0 = \"test\" results (%0) upvalues ($0) {}; ");

    check_bad_parse("call");
    check_bad_parse("call $0");
    check_bad_parse("call $0 =");
    check_bad_parse("call $0 = $2");
    check_bad_parse("call $0 = $2(");
    check_bad_parse("call $0 = $2($3");
    check_bad_parse("call $0 = $2($3)");

    check_bad_parse("call $0 = $2(); ");
    check_bad_parse("call $0 = $2($3,$4); ");
    check_bad_parse("call ($0) = $2(); ");
    check_bad_parse("call ($0) = $2($3); ");
    check_bad_parse("call ($0) = $2($3,$4); ");
    check_bad_parse("call ($0,$1) = $2(); ");
    check_bad_parse("call ($0,$1) = $2($3); ");
    check_bad_parse("call ($0,$1) = $2($3,$4); ");

    check_bad_parse("return");
    check_bad_parse("return (");
    check_bad_parse("return ($0");
    check_bad_parse("return ($0,");
    check_bad_parse("return ($0,$1");
    check_bad_parse("return ($0,$1)");

    check_bad_parse(
        "tliteral %0; "
        "tlocation %1 "
    );

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
