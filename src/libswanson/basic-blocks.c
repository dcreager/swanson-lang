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
    cork_strfree(self->name);
}

static void
s0_basic_block_recurse(struct cork_gc *gc, void *vself,
                       cork_gc_recurser recurse, void *ud)
{
    size_t  i;
    struct s0_basic_block  *self = vself;
    recurse(gc, self->upvalue, ud);
    recurse(gc, self->input, ud);
    recurse(gc, self->output, ud);
    for (i = 0; i < cork_array_size(&self->body); i++) {
        recurse(gc, cork_array_at(&self->body, i), ud);
    }
}

static struct cork_gc_obj_iface  s0_basic_block_gc = {
    s0_basic_block_free, s0_basic_block_recurse
};


struct s0_basic_block *
s0_basic_block_new(struct swan *s, const char *name,
                   struct s0_value *upvalue, struct s0_type *input,
                   struct s0_type *output, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_basic_block  *self = NULL;
    rp_check_gc_new(s0_basic_block, self, "basic block");
    e_check_alloc(self->name = cork_strdup(name), "basic block name");
    self->upvalue = cork_gc_incref(swan_gc(s), upvalue);
    self->input = cork_gc_incref(swan_gc(s), input);
    self->output = cork_gc_incref(swan_gc(s), output);
    cork_array_init(&self->body);
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}
