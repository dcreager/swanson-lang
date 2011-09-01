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

#include "lagavulin/continuation.h"

/*
 * This file contains all of the continuation definitions.  The file is
 * organized so that all of the "execute" and "result" definitions
 * appear together, in the hopes that this will place them closer
 * together in memory.  (Cache locality ftw)
 */

/*-----------------------------------------------------------------------
 * Subclass type definitions
 */

#define constant_struct(typ_id, typ) \
    struct lgv_block_constant_##typ_id { \
        struct lgv_block  parent; \
        typ  value; \
    };

constant_struct(bool, bool);
constant_struct(int, int);
constant_struct(long, long);

struct lgv_block_if {
    struct lgv_block  parent;
    struct lgv_continuation  brancher;
    struct lgv_block  *condition;
    struct lgv_block  *true_branch;
    struct lgv_block  *false_branch;
    struct lgv_continuation  *dest;
};

struct lgv_continuation_collect {
    struct lgv_continuation  parent;
    void  **dest;
};

/*-----------------------------------------------------------------------
 * Execute and result functions
 */

/*
 * TODO: Make sure these functions are compiled with
 * -fomit-frame-pointer.
 */

#define constant_execute(typ_id, typ) \
    static void \
    lgv_block_constant_##typ_id(struct lgv_block *vself, \
                                struct lgv_continuation *dest) \
    { \
        struct lgv_block_constant_##typ_id  *self = \
            cork_container_of \
            (vself, struct lgv_block_constant_##typ_id, parent); \
        return lgv_continuation_result(dest, &self->value); \
    }

constant_execute(bool, bool);
constant_execute(int, int);
constant_execute(long, long);

static void
lgv_block_if(struct lgv_block *vself, struct lgv_continuation *dest)
{
    struct lgv_block_if  *self =
        cork_container_of(vself, struct lgv_block_if, parent);
    self->dest = dest;
    return lgv_block_execute(self->condition, &self->brancher);
}

static void
lgv_block_if_brancher(struct lgv_continuation *vself, void *vinput)
{
    struct lgv_block_if  *self =
        cork_container_of(vself, struct lgv_block_if, brancher);
    bool  *input = vinput;

    if (*input) {
        return lgv_block_execute(self->true_branch, self->dest);
    } else {
        return lgv_block_execute(self->false_branch, self->dest);
    }
}

static void
lgv_continuation_collect(struct lgv_continuation *vself, void *vinput)
{
    struct lgv_continuation_collect  *self =
        cork_container_of(vself, struct lgv_continuation_collect, parent);
    *self->dest = vinput;
}

/*-----------------------------------------------------------------------
 * Constructors
 */

#define make_new(alloc, typ) \
    typ *self = cork_new(alloc, typ); \
    if (self == NULL) { \
        return NULL; \
    }

#define constant_new(typ_id, typ) \
    struct lgv_block * \
    lgv_block_new_constant_##typ_id(cork_allocator_t *alloc, typ value) \
    { \
        make_new(alloc, struct lgv_block_constant_##typ_id); \
        self->parent.execute = lgv_block_constant_##typ_id; \
        self->value = value; \
        return &self->parent; \
    }

constant_new(bool, bool);
constant_new(int, int);
constant_new(long, long);

struct lgv_block *
lgv_block_new_if(cork_allocator_t *alloc,
                 struct lgv_block *condition,
                 struct lgv_block *true_branch,
                 struct lgv_block *false_branch)
{
    make_new(alloc, struct lgv_block_if);
    self->parent.execute = lgv_block_if;
    self->brancher.result = lgv_block_if_brancher;
    self->condition = condition;
    self->true_branch = true_branch;
    self->false_branch = false_branch;
    self->dest = NULL;
    return &self->parent;
}

struct lgv_continuation *
lgv_continuation_new_collect(cork_allocator_t *alloc, void **dest)
{
    make_new(alloc, struct lgv_continuation_collect);
    self->parent.result = lgv_continuation_collect;
    self->dest = dest;
    return &self->parent;
}
