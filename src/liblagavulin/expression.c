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

#include "swanson/engine.h"
#include "swanson/state.h"
#include "swanson/swanson0.h"

#include "lagavulin/block.h"
#include "lagavulin/engine.h"


static void
lgv_expression_recurse(struct cork_gc *gc, void *vself,
                       cork_gc_recurser recurse, void *ud)
{
    struct lgv_expression  *self = vself;
    recurse(gc, self->block, ud);
}

static struct cork_gc_obj_iface  lgv_expression_gc = {
    NULL, lgv_expression_recurse
};


struct swan_expression *
lgv_expression_new(struct swan *s, struct lgv_block *b,
                   struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);

    struct lgv_expression  *self = NULL;
    e_check_gc_new(lgv_expression, self, "expression");
    ei_check(swan_expression_init(s, &self->parent, err));
    self->block = b;
    return &self->parent;

error:
    cork_gc_decref(swan_gc(s), b);
    return NULL;
}
