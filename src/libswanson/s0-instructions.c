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


/*-----------------------------------------------------------------------
 * Instructions
 */

static void
s0_instruction_free(struct cork_gc *gc, void *vself)
{
    struct s0_instruction  *self = vself;
    size_t  i;
    switch (self->op) {
        case S0_TFUNCTION:
            cork_array_done(gc->alloc, &self->_.tfunction.params);
            cork_array_done(gc->alloc, &self->_.tfunction.results);
            break;

        case S0_TINTERFACE:
            for (i = 0; i < cork_array_size
                    (&self->_.tinterface.entries); i++) {
                struct s0_tinterface_entry  entry =
                    cork_array_at(&self->_.tinterface.entries, i);
                cork_strfree(gc->alloc, entry.key);
            }
            cork_array_done(gc->alloc, &self->_.tinterface.entries);
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

struct s0_instruction *
s0_tinterface_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TINTERFACE instruction");
    self->op = S0_TINTERFACE;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    cork_array_init(swan_alloc(s), &self->_.tinterface.entries);
    return self;
}

int
s0_tinterface_add_entry(struct swan *s, struct s0_instruction *self,
                        const char *key, s0_tagged_id entry,
                        struct cork_error *err)
{
    struct s0_tinterface_entry  new_entry = { key, entry };
    ei_check(cork_array_append
             (swan_alloc(s), &self->_.tinterface.entries, new_entry, err));
    return 0;

error:
    cork_strfree(swan_alloc(s), new_entry.key);
    return -1;
}

struct s0_instruction *
s0_tblock_new(struct swan *s, s0_id dest, s0_tagged_id result,
              struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TBLOCK instruction");
    self->op = S0_TBLOCK;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    self->_.tblock.result = result;
    return self;
}

struct s0_instruction *
s0_ttype_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TTYPE instruction");
    self->op = S0_TTYPE;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    return self;
}

struct s0_instruction *
s0_literal_new(struct swan *s, s0_id dest, const char *contents,
               struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "LITERAL instruction");
    self->op = S0_LITERAL;
    self->dest = s0_tagged_id(S0_ID_TAG_LOCAL, dest);
    e_check_alloc(self->_.literal.contents =
                      cork_strdup(swan_alloc(s), contents),
                  "LITERAL contents");
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}


/*-----------------------------------------------------------------------
 * Basic blocks
 */

static void
s0_basic_block_recurse(struct cork_gc *gc, void *vself,
                       cork_gc_recurser recurse, void *ud)
{
    struct s0_basic_block  *self = vself;
    size_t  i;
    for (i = 0; i < cork_array_size(&self->body); i++) {
        struct s0_instruction  *element = cork_array_at(&self->body, i);
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
    cork_array_init(swan_alloc(s), &self->body);
    return self;
}

int
s0_basic_block_add(struct swan *s, struct s0_basic_block *self,
                   struct s0_instruction *instr, struct cork_error *err)
{
    return cork_array_append(swan_alloc(s), &self->body, instr, err);
}
