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
swan_init(struct swan *self)
{
    cork_gc_init();
    self->prelude = NULL;
    return 0;
}

void
swan_done(struct swan *self)
{
    cork_gc_done();
}
