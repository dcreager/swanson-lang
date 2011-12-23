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
#include <libcork/core/checkers.h>

#include "swanson/s0.h"
#include "swanson/state.h"


static void
s0_value_recurse(struct cork_gc *gc, void *vself,
                      cork_gc_recurser recurse, void *ud)
{
    struct s0_value  *self = vself;
    switch (self->kind) {
        case S0_VALUE_TYPE:
            recurse(gc, self->_.type, ud);
            break;

        default:
            break;
    }
}

static struct cork_gc_obj_iface  s0_value_gc = {
    NULL, s0_value_recurse
};

struct s0_value *
s0_type_value_new(struct swan *s, struct s0_type *type,
                  struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_value  *self = NULL;
    rp_check_gc_new(s0_value, self, "type value");
    self->kind = S0_VALUE_TYPE;
    self->_.type = cork_gc_incref(swan_gc(s), type);
    return self;
}
