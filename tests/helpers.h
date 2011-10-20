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


#define DESCRIBE_TEST \
    fprintf(stderr, "--- %s\n", __func__);


#define fail_if_error(call) \
    do { \
        struct cork_error  err = CORK_ERROR_INIT(alloc); \
        call; \
        if (cork_error_occurred(&err)) { \
            fail("%s", cork_error_message(&err)); \
        } \
        cork_error_done(&err); \
    } while (0)

#define fail_unless_error(call, msg) \
    do { \
        struct cork_error  err = CORK_ERROR_INIT(alloc); \
        call; \
        if (!cork_error_occurred(&err)) { \
            fail(msg); \
        } \
        cork_error_done(&err); \
    } while (0)


#endif /* TESTS_HELPERS_H */
