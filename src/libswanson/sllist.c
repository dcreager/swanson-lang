/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <libcork/core.h>
#include <libcork/helpers/errors.h>
#include <libcork/helpers/gc.h>

#include "swanson/ds.h"
#include "swanson/metamodel.h"
#include "swanson/state.h"

_recurse_(swan_sllist) {
    struct swan_sllist  *self = obj;
    recurse(gc, self->head, ud);
    recurse(gc, self->tail, ud);
}

_gc_no_free_(swan_sllist);

struct swan_sllist *
swan_sllist_new(struct swan *s, struct swan_thing *head,
                struct swan_sllist *tail)
{
    struct swan_sllist  *self = cork_gc_new(swan_sllist);
    self->head = head;
    self->tail = tail;
    return self;
}
