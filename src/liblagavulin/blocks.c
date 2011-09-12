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
lgv_state_init(struct cork_gc *gc, struct lgv_state *state)
{
    state->ret = NULL;
    return 0;
}

void
lgv_state_done(struct cork_gc *gc, struct lgv_state *state)
{
    cork_gc_decref(gc, state->ret);
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

struct lgv_block_brancher {
    struct lgv_block  parent;
    struct lgv_block  *true_branch;
    struct lgv_block  *false_branch;
};

struct lgv_block_if {
    struct lgv_block  parent;
    struct lgv_block  *condition;
    struct lgv_block  *brancher;
};

struct lgv_block_collect {
    struct lgv_block  parent;
    void  **dest;
};


/*-----------------------------------------------------------------------
 * Garbage collection functions
 */

#define constant_gc(typ_id) \
    static void \
    lgv_block_constant_##typ_id##_recurse \
    (void *vself, cork_gc_recurser recurse, void *ud) \
    { \
        struct lgv_block_constant_##typ_id  *self = vself; \
        recurse(self->next, ud); \
    } \
    \
    static struct cork_gc_obj_iface \
    lgv_block_constant_##typ_id##_gc = { \
        NULL, lgv_block_constant_##typ_id##_recurse \
    };

constant_gc(bool);
constant_gc(int);
constant_gc(long);

static struct cork_gc_obj_iface  lgv_block_leaf_gc = {
    NULL, NULL
};

static void
lgv_block_brancher_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct lgv_block_brancher  *self = vself;
    recurse(self->true_branch, ud);
    recurse(self->false_branch, ud);
}

static struct cork_gc_obj_iface  lgv_block_brancher_gc = {
    NULL, lgv_block_brancher_recurse
};

static void
lgv_block_if_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct lgv_block_if  *self = vself;
    recurse(self->condition, ud);
    recurse(self->brancher, ud);
}

static struct cork_gc_obj_iface  lgv_block_if_gc = {
    NULL, lgv_block_if_recurse
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
        DEBUG("%p %s", vself, vself->name); \
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
    DEBUG("%p %s", vself, vself->name);
    return lgv_block_execute(self->condition, state, NULL);
}

static int
lgv_block_brancher(struct lgv_block *vself,
                   struct lgv_state *state, void *vinput)
{
    struct lgv_block_brancher  *self =
        cork_container_of(vself, struct lgv_block_brancher, parent);
    bool  *input = vinput;
    DEBUG("%p %s: %s", vself, vself->name, *input? "true": "false");

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
    DEBUG("%p %s: %p", vself, vself->name, vinput);
    return lgv_block_execute(state->ret, state, vinput);
}

static int
lgv_block_collect(struct lgv_block *vself,
                  struct lgv_state *state, void *vinput)
{
    struct lgv_block_collect  *self =
        cork_container_of(vself, struct lgv_block_collect, parent);
    DEBUG("%p %s: %p", vself, vself->name, vinput);
    *self->dest = vinput;
    return 0;
}


/*-----------------------------------------------------------------------
 * set_next methods
 */

#define constant_set_next(typ_id, typ) \
    static void \
    lgv_block_constant_##typ_id##_set_next \
    (struct cork_gc *gc, struct lgv_block *vself, struct lgv_block *next) \
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
lgv_block_brancher_set_next(struct cork_gc *gc,
                            struct lgv_block *vself, struct lgv_block *next)
{
    struct lgv_block_brancher  *self =
        cork_container_of(vself, struct lgv_block_brancher, parent);

    /*
     * We're assuming control of one reference, which we can pass on to
     * one of the branches.  The other branch needs a new reference.
     */
    lgv_block_set_next(gc, self->true_branch, next);
    lgv_block_set_next(gc, self->false_branch, cork_gc_incref(gc, next));
}

static void
lgv_block_if_set_next(struct cork_gc *gc,
                      struct lgv_block *vself, struct lgv_block *next)
{
    struct lgv_block_if  *self =
        cork_container_of(vself, struct lgv_block_if, parent);
    lgv_block_set_next(gc, self->brancher, next);
}

static void
lgv_block_return_set_next(struct cork_gc *gc,
                          struct lgv_block *vself, struct lgv_block *next)
{
    /*
     * We're taking control of the reference, but we don't actually need
     * it.
     */
    cork_gc_decref(gc, next);
}


/*-----------------------------------------------------------------------
 * Constructors
 */

#define make_new(gc, typ, iface) \
    typ *self = cork_gc_new(gc, typ, &iface##_gc); \
    if (self == NULL) { \
        return NULL; \
    }

#define constant_new(typ_id, typ) \
    struct lgv_block * \
    lgv_block_new_constant_##typ_id(struct cork_gc *gc, typ value) \
    { \
        make_new(gc, struct lgv_block_constant_##typ_id, \
                 lgv_block_constant_##typ_id); \
        self->parent.name = "constant_" #typ_id; \
        self->parent.execute = lgv_block_constant_##typ_id; \
        self->parent.set_next = lgv_block_constant_##typ_id##_set_next; \
        self->value = value; \
        return &self->parent; \
    }

constant_new(bool, bool);
constant_new(int, int);
constant_new(long, long);

static struct lgv_block *
lgv_block_new_brancher(struct cork_gc *gc,
                       const char *name,
                       struct lgv_block *true_branch,
                       struct lgv_block *false_branch)
{
    make_new(gc, struct lgv_block_brancher, lgv_block_brancher);
    self->parent.name = name;
    self->parent.execute = lgv_block_brancher;
    self->parent.set_next = lgv_block_brancher_set_next;
    self->true_branch = true_branch;
    self->false_branch = false_branch;
    return &self->parent;
}

struct lgv_block *
lgv_block_new_if(struct cork_gc *gc,
                 struct lgv_block *condition,
                 struct lgv_block *true_branch,
                 struct lgv_block *false_branch)
{
    make_new(gc, struct lgv_block_if, lgv_block_if);

    self->brancher =
        lgv_block_new_brancher(gc, "if-brancher", true_branch, false_branch);
    if (self->brancher == NULL) {
        cork_gc_decref(gc, self);
        return NULL;
    }

    self->parent.name = "if";
    self->parent.execute = lgv_block_if;
    self->parent.set_next = lgv_block_if_set_next;
    self->condition = condition;
    cork_gc_incref(gc, self->brancher);
    lgv_block_set_next(gc, self->condition, self->brancher);
    return &self->parent;
}

struct lgv_block *
lgv_block_new_return(struct cork_gc *gc)
{
    make_new(gc, struct lgv_block, lgv_block_leaf);
    self->name = "return";
    self->execute = lgv_block_return;
    self->set_next = lgv_block_return_set_next;
    return self;
}

struct lgv_block *
lgv_block_new_collect(struct cork_gc *gc, void **dest)
{
    make_new(gc, struct lgv_block_collect, lgv_block_leaf);
    self->parent.name = "collect";
    self->parent.execute = lgv_block_collect;
    self->parent.set_next = NULL;
    self->dest = dest;
    return &self->parent;
}
