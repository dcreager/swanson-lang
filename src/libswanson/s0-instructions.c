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
#include <libcork/helpers/gc.h>
#include <libcork/ds.h>

#include "swanson/s0.h"
#include "swanson/state.h"


/*-----------------------------------------------------------------------
 * Instructions
 */

_free_(s0_instruction) {
    struct s0_instruction  *self = obj;
    size_t  i;
    switch (self->op) {
        case S0_TPRODUCT:
            cork_array_done(&self->_.tproduct.elements);
            break;

        case S0_TINTERFACE:
            for (i = 0; i < cork_array_size
                    (&self->_.tinterface.entries); i++) {
                struct s0_tinterface_entry  entry =
                    cork_array_at(&self->_.tinterface.entries, i);
                cork_strfree(entry.key);
            }
            cork_array_done(&self->_.tinterface.entries);
            break;

        case S0_TCLASS:
            for (i = 0; i < cork_array_size
                    (&self->_.tclass.entries); i++) {
                struct s0_tinterface_entry  entry =
                    cork_array_at(&self->_.tclass.entries, i);
                cork_strfree(entry.key);
            }
            cork_array_done(&self->_.tclass.entries);
            cork_strfree(self->_.tclass.name);
            break;

        case S0_LITERAL:
            cork_strfree(self->_.literal.contents);
            break;

        case S0_TUPLE:
            cork_array_done(&self->_.tuple.elements);
            break;

        case S0_GETCLASS:
            cork_strfree(self->_.getclass.index);
            break;

        case S0_MACRO:
            cork_strfree(self->_.macro.name);
            cork_array_done(&self->_.macro.body);
            break;

        default:
            break;
    }
}

_gc_no_recurse_(s0_instruction);


struct s0_instruction *
s0i_trecursive_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TRECURSIVE instruction");
    self->op = S0_TRECURSIVE;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    return self;
}

struct s0_instruction *
s0i_ttype_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TTYPE instruction");
    self->op = S0_TTYPE;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    return self;
}

struct s0_instruction *
s0i_tliteral_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TLITERAL instruction");
    self->op = S0_TLITERAL;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    return self;
}

struct s0_instruction *
s0i_tany_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TANY instruction");
    self->op = S0_TANY;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    return self;
}

struct s0_instruction *
s0i_tproduct_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TPRODUCT instruction");
    self->op = S0_TPRODUCT;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    cork_array_init(&self->_.tproduct.elements);
    return self;
}

struct s0_instruction *
s0i_tfunction_new(struct swan *s, s0_id dest, s0_tagged_id input,
                  s0_tagged_id output, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TFUNCTION instruction");
    self->op = S0_TFUNCTION;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    self->_.tfunction.input = input;
    self->_.tfunction.output = output;
    return self;
}

struct s0_instruction *
s0i_tlocation_new(struct swan *s, s0_id dest, s0_tagged_id referent,
                  struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TLOCATION instruction");
    self->op = S0_TLOCATION;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    self->_.tlocation.referent = referent;
    return self;
}

struct s0_instruction *
s0i_tinterface_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TINTERFACE instruction");
    self->op = S0_TINTERFACE;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    cork_array_init(&self->_.tinterface.entries);
    return self;
}

int
s0i_tinterface_add_entry(struct swan *s, struct s0_instruction *self,
                         const char *key, s0_tagged_id entry,
                         struct cork_error *err)
{
    struct s0_tinterface_entry  new_entry = { key, entry };
    ei_check(cork_array_append
             (&self->_.tinterface.entries, new_entry, err));
    return 0;

error:
    cork_strfree(new_entry.key);
    return -1;
}

struct s0_instruction *
s0i_tclass_new(struct swan *s, s0_id dest, const char *name,
               struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TCLASS instruction");
    e_check_alloc(self->_.tclass.name = cork_strdup(name),
                  "TCLASS name");
    self->op = S0_TCLASS;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    cork_array_init(&self->_.tclass.entries);
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

int
s0i_tclass_add_entry(struct swan *s, struct s0_instruction *self,
                     const char *key, s0_tagged_id entry,
                     struct cork_error *err)
{
    struct s0_tinterface_entry  new_entry = { key, entry };
    ei_check(cork_array_append
             (&self->_.tclass.entries, new_entry, err));
    return 0;

error:
    cork_strfree(new_entry.key);
    return -1;
}

struct s0_instruction *
s0i_tblock_new(struct swan *s, s0_id dest, s0_tagged_id result,
               struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TBLOCK instruction");
    self->op = S0_TBLOCK;
    self->dest = s0_tagged_id(S0_ID_TAG_TYPE, dest);
    self->_.tblock.result = result;
    return self;
}

struct s0_instruction *
s0i_literal_new(struct swan *s, s0_id dest, const char *contents,
                struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "LITERAL instruction");
    self->op = S0_LITERAL;
    self->dest = s0_tagged_id(S0_ID_TAG_LOCAL, dest);
    e_check_alloc(self->_.literal.contents =
                      cork_strdup(contents),
                  "LITERAL contents");
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

struct s0_instruction *
s0i_tuple_new(struct swan *s, s0_id dest, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "TUPLE instruction");
    self->op = S0_TUPLE;
    self->dest = s0_tagged_id(S0_ID_TAG_LOCAL, dest);
    cork_array_init(&self->_.tuple.elements);
    return self;
}

struct s0_instruction *
s0i_gettuple_new(struct swan *s, s0_id dest, s0_tagged_id src, size_t index,
                 struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "GETTUPLE instruction");
    self->op = S0_GETTUPLE;
    self->dest = s0_tagged_id(S0_ID_TAG_LOCAL, dest);
    self->_.gettuple.src = src;
    self->_.gettuple.index = index;
    return self;
}

struct s0_instruction *
s0i_getclass_new(struct swan *s, s0_id dest, s0_tagged_id src,
                 const char *index, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "GETCLASS instruction");
    self->op = S0_GETCLASS;
    self->dest = s0_tagged_id(S0_ID_TAG_LOCAL, dest);
    self->_.gettuple.src = src;
    e_check_alloc(self->_.getclass.index = cork_strdup(index),
                  "GETCLASS index");
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

struct s0_instruction *
s0i_macro_new(struct swan *s, s0_id dest, const char *name,
              struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "MACRO instruction");
    self->op = S0_MACRO;
    self->dest = s0_tagged_id(S0_ID_TAG_LOCAL, dest);
    e_check_alloc(self->_.macro.name = cork_strdup(name),
                  "macro name");
    self->_.macro.upvalue = S0_ID_NULL;
    self->_.macro.input = S0_ID_NULL;
    self->_.macro.output = S0_ID_NULL;
    cork_array_init(&self->_.macro.body);
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

struct s0_instruction *
s0i_call_new(struct swan *s, s0_id dest, s0_tagged_id callee,
             s0_tagged_id input, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "CALL instruction");
    self->op = S0_CALL;
    self->dest = s0_tagged_id(S0_ID_TAG_LOCAL, dest);
    self->_.call.callee = callee;
    self->_.call.input = input;
    return self;
}

struct s0_instruction *
s0i_return_new(struct swan *s, s0_tagged_id result, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct s0_instruction  *self = NULL;
    rp_check_gc_new(s0_instruction, self, "RETURN instruction");
    self->op = S0_RETURN;
    self->_.ret.result = result;
    return self;
}
