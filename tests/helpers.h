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
            struct cork_buffer  buf = CORK_BUFFER_INIT(alloc); \
            cork_error_message(alloc, &err, &buf); \
            printf("%s\n", (char *) buf.buf); \
            cork_buffer_done(alloc, &buf);
#else
#define print_expected_failure()  /* do nothing */
#endif


#define DESCRIBE_TEST \
    fprintf(stderr, "--- %s\n", __func__);

#define DECLARE_SWAN \
    struct cork_alloc  *alloc = cork_allocator_new_debug(); \
    struct swan  s; \
    swan_init(&s, alloc, NULL);

#define CLEANUP_SWAN \
    swan_done(&s); \
    cork_allocator_free(alloc);

#define DECLARE_LGV \
    struct cork_alloc  *alloc = cork_allocator_new_debug(); \
    struct swan  *s; \
    fail_if_error(s = lgv_new(alloc, &err));

#define CLEANUP_LGV \
    lgv_free(s); \
    cork_allocator_free(alloc);


#define fail_if_error(call) \
    do { \
        struct cork_error  err = CORK_ERROR_INIT(alloc); \
        call; \
        if (cork_error_occurred(&err)) { \
            struct cork_buffer  buf = CORK_BUFFER_INIT(alloc); \
            cork_error_message(alloc, &err, &buf); \
            fail("%s", (char *) buf.buf); \
            cork_buffer_done(alloc, &buf); \
        } \
        cork_error_done(alloc, &err); \
    } while (0)

#define fail_unless_error(call, ...) \
    do { \
        struct cork_error  err = CORK_ERROR_INIT(alloc); \
        call; \
        if (!cork_error_occurred(&err)) { \
            fail(__VA_ARGS__); \
        } else { \
            print_expected_failure(); \
        } \
        cork_error_done(alloc, &err); \
    } while (0)


#endif /* TESTS_HELPERS_H */
