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

#include "lagavulin/block.h"

#if !defined(BLOCK_DEBUG)
#define BLOCK_DEBUG 0
#endif

#if BLOCK_DEBUG
#include <stdio.h>
#define DEBUG(...) \
    do { \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while (0)
#else
#define DEBUG(...) /* no debug messages */
#endif

/*
 * This file contains all of the block definitions.  The file is
 * organized so that all of the "execute" definitions appear together,
 * in the hopes that this will place them closer together in memory.
 * (Cache locality ftw)
 */

/*-----------------------------------------------------------------------
 * Execution states
 */

int
lgv_state_init(struct lgv_state *state)
{
    state->ret = NULL;
    return 0;
}

void
lgv_state_done(struct lgv_state *state)
{
    state->ret = NULL;
}


/*-----------------------------------------------------------------------
 * Subclass type definitions
 */

#define constant_struct(typ_id, typ) \
    struct lgv_block_constant_##typ_id { \
        struct lgv_block  parent; \
        typ  value; \
        struct lgv_block  *next; \
    };

constant_struct(bool, bool);
constant_struct(int, int);
constant_struct(long, long);

struct lgv_block_if {
    struct lgv_block  parent;
    struct lgv_block  brancher;
    struct lgv_block  *condition;
    struct lgv_block  *true_branch;
    struct lgv_block  *false_branch;
};

struct lgv_block_collect {
    struct lgv_block  parent;
    void  **dest;
};


/*-----------------------------------------------------------------------
 * Execute functions
 */

/*
 * TODO: Make sure these functions are compiled with
 * -fomit-frame-pointer.
 */

#define constant_execute(typ_id, typ) \
    static int \
    lgv_block_constant_##typ_id(struct lgv_block *vself, \
                                struct lgv_state *state, void *vinput) \
    { \
        struct lgv_block_constant_##typ_id  *self = \
            cork_container_of \
            (vself, struct lgv_block_constant_##typ_id, parent); \
        DEBUG("%p constant_" #typ_id, vself); \
        return lgv_block_execute(self->next, state, &self->value); \
    }

constant_execute(bool, bool);
constant_execute(int, int);
constant_execute(long, long);

static int
lgv_block_if(struct lgv_block *vself,
             struct lgv_state *state, void *vinput)
{
    struct lgv_block_if  *self =
        cork_container_of(vself, struct lgv_block_if, parent);
    DEBUG("%p if", vself);
    return lgv_block_execute(self->condition, state, NULL);
}

static int
lgv_block_if_brancher(struct lgv_block *vself,
                      struct lgv_state *state, void *vinput)
{
    struct lgv_block_if  *self =
        cork_container_of(vself, struct lgv_block_if, brancher);
    bool  *input = vinput;
    DEBUG("%p if (brancher): %s", vself, *input? "true": "false");

    if (*input) {
        return lgv_block_execute(self->true_branch, state, NULL);
    } else {
        return lgv_block_execute(self->false_branch, state, NULL);
    }
}

static int
lgv_block_return(struct lgv_block *vself,
                 struct lgv_state *state, void *vinput)
{
    DEBUG("%p return: %p", vself, vinput);
    return lgv_block_execute(state->ret, state, vinput);
}

static int
lgv_block_collect(struct lgv_block *vself,
                  struct lgv_state *state, void *vinput)
{
    struct lgv_block_collect  *self =
        cork_container_of(vself, struct lgv_block_collect, parent);
    DEBUG("%p collect: %p", vself, vinput);
    *self->dest = vinput;
    return 0;
}


/*-----------------------------------------------------------------------
 * set_next methods
 */

#define constant_set_next(typ_id, typ) \
    static void \
    lgv_block_constant_##typ_id##_set_next(struct lgv_block *vself, \
                                           struct lgv_block *next) \
    { \
        struct lgv_block_constant_##typ_id  *self = \
            cork_container_of \
            (vself, struct lgv_block_constant_##typ_id, parent); \
        self->next = next; \
    }

constant_set_next(bool, bool);
constant_set_next(int, int);
constant_set_next(long, long);

static void
lgv_block_if_set_next(struct lgv_block *vself, struct lgv_block *next)
{
    struct lgv_block_if  *self =
        cork_container_of(vself, struct lgv_block_if, parent);
    lgv_block_set_next(self->true_branch, next);
    lgv_block_set_next(self->false_branch, next);
}

static void
lgv_block_return_set_next(struct lgv_block *vself, struct lgv_block *next)
{
    /* nothing to do */
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
        self->parent.set_next = lgv_block_constant_##typ_id##_set_next; \
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
    self->parent.set_next = lgv_block_if_set_next;
    self->brancher.execute = lgv_block_if_brancher;
    self->brancher.set_next = NULL;
    self->condition = condition;
    self->true_branch = true_branch;
    self->false_branch = false_branch;
    lgv_block_set_next(self->condition, &self->brancher);
    return &self->parent;
}

struct lgv_block *
lgv_block_new_return(cork_allocator_t *alloc)
{
    make_new(alloc, struct lgv_block);
    self->execute = lgv_block_return;
    self->set_next = lgv_block_return_set_next;
    return self;
}

struct lgv_block *
lgv_block_new_collect(cork_allocator_t *alloc, void **dest)
{
    make_new(alloc, struct lgv_block_collect);
    self->parent.execute = lgv_block_collect;
    self->parent.set_next = NULL;
    self->dest = dest;
    return &self->parent;
}
