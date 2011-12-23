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
#include <libcork/ds.h>

#include "swanson/s0.h"
#include "swanson/state.h"


static void
s0_instruction_free(struct cork_gc *gc, void *vself)
{
    struct s0_instruction  *self = vself;
    switch (self->op) {
        case S0_TFUNCTION:
            cork_array_done(gc->alloc, &self->_.tfunction.params);
            cork_array_done(gc->alloc, &self->_.tfunction.results);
            break;

        default:
            break;
    }
}

static struct cork_gc_obj_iface  s0_instruction_gc = {
    s0_instruction_free, NULL
};


struct s0_instruction *
s0_trecursive_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TRECURSIVE instruction");
    self->op = S0_TRECURSIVE;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    return self;
}

struct s0_instruction *
s0_tliteral_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TLITERAL instruction");
    self->op = S0_TLITERAL;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    return self;
}

struct s0_instruction *
s0_tfunction_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TFUNCTION instruction");
    self->op = S0_TFUNCTION;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    cork_array_init(swan_alloc(s), &self->_.tfunction.params);
    cork_array_init(swan_alloc(s), &self->_.tfunction.results);
    return self;
}

struct s0_instruction *
s0_tlocation_new(struct swan *s, s0_id dest, s0_tagged_id referent,
                 struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TLOCATION instruction");
    self->op = S0_TLOCATION;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    self->_.tlocation.referent = referent;
    return self;
}


static void
s0_basic_block_recurse(struct cork_gc *gc, void *vself,
                       cork_gc_recurser recurse, void *ud)
{
    struct s0_basic_block  *self = vself;
    struct cork_dllist_item  *curr;
    for (curr = cork_dllist_start(&self->body);
         !cork_dllist_is_end(&self->body, curr); curr = curr->next) {
        struct s0_instruction  *element =
            cork_container_of(curr, struct s0_instruction, siblings);
        recurse(gc, element, ud);
    }
}

static struct cork_gc_obj_iface  s0_basic_block_gc = {
    NULL, s0_basic_block_recurse
};

struct s0_basic_block *
s0_basic_block_new(struct swan *s, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_basic_block  *self = NULL;
    rp_check_gc_new(s0_basic_block, self, "basic block");
    cork_dllist_init(&self->body);
    return self;
}

int
s0_basic_block_add(struct swan *s, struct s0_basic_block *self,
                   struct s0_instruction *instr, struct cork_error *err)
{
    cork_dllist_add(&self->body, &instr->siblings);
    return 0;
}
