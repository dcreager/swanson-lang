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
#include <libcork/helpers/errors.h>

#include "swanson/state.h"

int
swan_init(struct swan *self, struct cork_error *err)
{
    ei_check(cork_gc_init(&self->gc));
    self->prelude = NULL;
    return 0;

error:
    cork_cannot_allocate_set(err, "Swanson state");
    return -1;
}

void
swan_done(struct swan *self)
{
    cork_gc_done(&self->gc);
}
