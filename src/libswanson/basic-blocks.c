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
s0_basic_block_free(struct cork_gc *gc, void *vself)
{
    struct s0_basic_block  *self = vself;
    cork_strfree(gc->alloc, self->name);
    cork_array_done(gc->alloc, &self->upvalues);
    cork_array_done(gc->alloc, &self->params);
    cork_array_done(gc->alloc, &self->results);
}

static void
s0_basic_block_recurse(struct cork_gc *gc, void *vself,
                       cork_gc_recurser recurse, void *ud)
{
    size_t  i;
    struct s0_basic_block  *self = vself;
    for (i = 0; i < cork_array_size(&self->upvalues); i++) {
        recurse(gc, cork_array_at(&self->upvalues, i), ud);
    }
    for (i = 0; i < cork_array_size(&self->params); i++) {
        recurse(gc, cork_array_at(&self->params, i), ud);
    }
    for (i = 0; i < cork_array_size(&self->results); i++) {
        recurse(gc, cork_array_at(&self->results, i), ud);
    }
    for (i = 0; i < cork_array_size(&self->body); i++) {
        recurse(gc, cork_array_at(&self->body, i), ud);
    }
}

static struct cork_gc_obj_iface  s0_basic_block_gc = {
    s0_basic_block_free, s0_basic_block_recurse
};


struct s0_basic_block *
s0_basic_block_new(struct swan *s, const char *name, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_basic_block  *self = NULL;
    rp_check_gc_new(s0_basic_block, self, "basic_block");
    e_check_alloc(self->name = cork_strdup(swan_alloc(s), name),
                  "basic_block name");
    cork_array_init(swan_alloc(s), &self->upvalues);
    cork_array_init(swan_alloc(s), &self->params);
    cork_array_init(swan_alloc(s), &self->results);
    cork_array_init(swan_alloc(s), &self->body);
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}
