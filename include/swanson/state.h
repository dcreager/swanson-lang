/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_STATE_H
#define SWANSON_STATE_H

/**
 * @file
 * @brief Implementation of the @ref state module
 */

#include <libcork/core.h>

/**
 * @addtogroup state Global execution state
 *
 * <tt>#%include \<swanson/state.h\></tt>
 */


struct swan {
    struct cork_gc  gc;
};

int
swan_init(struct swan *state, struct cork_alloc *alloc);

void
swan_done(struct swan *state);

#define swan_gc(s)  (&(s)->gc)
#define swan_alloc(s)  ((s)->gc.alloc)


/* Hash of "swanson.general" */
#define SWAN_GENERAL_ERROR  0xae419dc9

enum swan_general_error {
    SWAN_GENERAL_ERROR_CANNOT_ALLOCATE
};


#endif  /* SWANSON_STATE_H */
