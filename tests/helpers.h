/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef TESTS_HELPERS_H
#define TESTS_HELPERS_H


#if !defined(PRINT_EXPECTED_FAILURES)
#define PRINT_EXPECTED_FAILURES  0
#endif

#if PRINT_EXPECTED_FAILURES
#define print_expected_failure() \
            printf("%s\n", cork_error_message());
#else
#define print_expected_failure()  /* do nothing */
#endif


#define DESCRIBE_TEST \
    fprintf(stderr, "--- %s\n", __func__);

#define DECLARE_SWAN \
    struct swan  s; \
    swan_init(&s);

#define CLEANUP_SWAN \
    swan_done(&s);

#define DECLARE_LGV \
    struct swan  *s; \
    fail_if_error(s = lgv_new());

#define CLEANUP_LGV \
    lgv_free(s);


#define fail_if_error(call) \
    do { \
        call; \
        if (cork_error_occurred()) { \
            fail("%s", cork_error_message()); \
        } \
    } while (0)

#define fail_unless_error(call, ...) \
    do { \
        call; \
        if (!cork_error_occurred()) { \
            fail(__VA_ARGS__); \
        } else { \
            print_expected_failure(); \
            cork_error_clear(); \
        } \
    } while (0)


#endif /* TESTS_HELPERS_H */
