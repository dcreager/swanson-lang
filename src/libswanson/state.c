/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <libcork/core.h>

#include "swanson/checkers.h"
#include "swanson/state.h"

int
swan_init(struct swan *self, struct cork_alloc *alloc,
          struct cork_error *err)
{
    e_check(cork_gc_init(&self->gc, alloc));
    return 0;

error:
    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Can't initialize Swanson state");
    return -1;
}

void
swan_done(struct swan *self)
{
    cork_gc_done(&self->gc);
}
