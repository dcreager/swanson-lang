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
#include "lagavulin/checkers.h"
#include "lagavulin/stack.h"

#if !defined(BLOCK_DEBUG)
#define BLOCK_DEBUG 0
#endif

#if !defined(BLOCK_DEBUG_FREE)
#define BLOCK_DEBUG_FREE 0
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
    r_check(lgv_stack_init(gc, &state->stack, LGV_STACK_DEFAULT_INITIAL_SIZE));
    state->ret = NULL;
    return 0;
}

void
lgv_state_done(struct cork_gc *gc, struct lgv_state *state)
{
    cork_gc_decref(gc, state->ret);
    lgv_stack_done(gc, &state->stack);
    state->ret = NULL;
}


/*-----------------------------------------------------------------------
 * Subclass type definitions
 */

struct lgv_block_simple {
    struct lgv_block  parent;
    struct lgv_block  *next;
};

#define constant_struct(typ_id, typ) \
    struct lgv_block_constant_##typ_id { \
        struct lgv_block_simple  parent; \
        typ  value; \
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

struct lgv_block_seq {
    struct lgv_block  parent;
    /* We hold a reference to head, but not to tail */
    struct lgv_block  *head;
    struct lgv_block  *tail;
};


/*-----------------------------------------------------------------------
 * Garbage collection functions
 */

#if BLOCK_DEBUG_FREE
static void
lgv_block_free(struct cork_alloc *alloc, void *vself)
{
    struct lgv_block  *self = vself;
    DEBUG("Freeing %s(%p)", self->name, self);
}
#define BLOCK_FREE  lgv_block_free
#else
#define BLOCK_FREE  NULL
#endif

static struct cork_gc_obj_iface  lgv_block_leaf_gc = {
    BLOCK_FREE, NULL
};


static void
lgv_block_simple_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct lgv_block_simple  *self = vself;
    recurse(self->next, ud);
}

static struct cork_gc_obj_iface  lgv_block_simple_gc = {
    BLOCK_FREE, lgv_block_simple_recurse
};


static void
lgv_block_brancher_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct lgv_block_brancher  *self = vself;
    recurse(self->true_branch, ud);
    recurse(self->false_branch, ud);
}

static struct cork_gc_obj_iface  lgv_block_brancher_gc = {
    BLOCK_FREE, lgv_block_brancher_recurse
};


static void
lgv_block_if_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct lgv_block_if  *self = vself;
    recurse(self->condition, ud);
    recurse(self->brancher, ud);
}

static struct cork_gc_obj_iface  lgv_block_if_gc = {
    BLOCK_FREE, lgv_block_if_recurse
};


static void
lgv_block_seq_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct lgv_block_seq  *self = vself;
    /* We don't keep a reference to tail, since it's reachable from head */
    recurse(self->head, ud);
}

static struct cork_gc_obj_iface  lgv_block_seq_gc = {
    BLOCK_FREE, lgv_block_seq_recurse
};


/*-----------------------------------------------------------------------
 * Execute functions
 */

/*
 * TODO: Make sure these functions are compiled with
 * -fomit-frame-pointer.
 */

#define constant_execute(typ_id, typ, union_branch) \
    static int \
    lgv_block_constant_##typ_id(struct cork_gc *gc, struct lgv_block *vself, \
                                struct lgv_state *state) \
    { \
        struct lgv_block_constant_##typ_id  *self = \
            cork_container_of \
            (vself, struct lgv_block_constant_##typ_id, parent.parent); \
        DEBUG("%p %s", vself, vself->name); \
        lgv_stack_push(gc, &state->stack, union_branch, self->value); \
        return lgv_block_execute(gc, self->parent.next, state); \
    }

constant_execute(bool, bool, b);
constant_execute(int, int, si);
constant_execute(long, long, sl);

static int
lgv_block_add_int(struct cork_gc *gc, struct lgv_block *vself,
                  struct lgv_state *state)
{
    struct lgv_block_simple  *self =
        cork_container_of(vself, struct lgv_block_simple, parent);
    int  lhs = lgv_stack_get(&state->stack, -2, si);
    int  rhs = lgv_stack_get(&state->stack, -1, si);
    DEBUG("%p %s: %d %d", vself, vself->name, lhs, rhs);
    lgv_stack_pop(gc, &state->stack, 2);
    lgv_stack_push(gc, &state->stack, si, lhs + rhs);
    return lgv_block_execute(gc, self->next, state);
}

static int
lgv_block_if(struct cork_gc *gc, struct lgv_block *vself,
             struct lgv_state *state)
{
    struct lgv_block_if  *self =
        cork_container_of(vself, struct lgv_block_if, parent);
    DEBUG("%p %s", vself, vself->name);
    return lgv_block_execute(gc, self->condition, state);
}

static int
lgv_block_seq(struct cork_gc *gc, struct lgv_block *vself,
              struct lgv_state *state)
{
    struct lgv_block_seq  *self =
        cork_container_of(vself, struct lgv_block_seq, parent);
    DEBUG("%p %s", vself, vself->name);
    return lgv_block_execute(gc, self->head, state);
}

static int
lgv_block_brancher(struct cork_gc *gc, struct lgv_block *vself,
                   struct lgv_state *state)
{
    struct lgv_block_brancher  *self =
        cork_container_of(vself, struct lgv_block_brancher, parent);
    bool  input = lgv_stack_get(&state->stack, -1, b);
    DEBUG("%p %s: %s", vself, vself->name, input? "true": "false");

    if (input) {
        return lgv_block_execute(gc, self->true_branch, state);
    } else {
        return lgv_block_execute(gc, self->false_branch, state);
    }
}

static int
lgv_block_return(struct cork_gc *gc, struct lgv_block *vself,
                 struct lgv_state *state)
{
    DEBUG("%p %s", vself, vself->name);
    return lgv_block_execute(gc, state->ret, state);
}

static int
lgv_block_halt(struct cork_gc *gc, struct lgv_block *vself,
               struct lgv_state *state)
{
    DEBUG("%p %s", vself, vself->name);
    return 0;
}


/*-----------------------------------------------------------------------
 * set_next methods
 */

static void
lgv_block_simple_set_next(struct cork_gc *gc,
                          struct lgv_block *vself, struct lgv_block *next)
{
    struct lgv_block_simple  *self =
        cork_container_of(vself, struct lgv_block_simple, parent);
    DEBUG("%s(%p) ==> %s(%p)", vself->name, vself, next->name, next);
    self->next = next;
}

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
lgv_block_seq_set_next(struct cork_gc *gc,
                       struct lgv_block *vself, struct lgv_block *next)
{
    struct lgv_block_seq  *self =
        cork_container_of(vself, struct lgv_block_seq, parent);
    lgv_block_set_next(gc, self->tail, next);
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

static void
lgv_block_halt_set_next(struct cork_gc *gc,
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
        make_new(gc, struct lgv_block_constant_##typ_id, lgv_block_simple); \
        self->parent.parent.name = "constant_" #typ_id; \
        self->parent.parent.execute = lgv_block_constant_##typ_id; \
        self->parent.parent.set_next = lgv_block_simple_set_next; \
        self->value = value; \
        return &self->parent.parent; \
    }

constant_new(bool, bool);
constant_new(int, int);
constant_new(long, long);

struct lgv_block *
lgv_block_new_add_int(struct cork_gc *gc)
{
    make_new(gc, struct lgv_block_simple, lgv_block_simple);
    self->parent.name = "add-int";
    self->parent.execute = lgv_block_add_int;
    self->parent.set_next = lgv_block_simple_set_next;
    return &self->parent;
}

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


#define lgv_block_is_seq(block)  ((block)->execute == lgv_block_seq)

struct lgv_block *
lgv_block_new_seq(struct cork_gc *gc,
                  struct lgv_block *b1, struct lgv_block *b2)
{
    /*
     * If either (or both) of the input blocks is already a seq, just
     * merge the other block into it instead of creating a new one.
     */

    if (lgv_block_is_seq(b1)) {
        if (lgv_block_is_seq(b2)) {
            /* Both blocks are seqs */
            struct lgv_block_seq  *s1 =
                cork_container_of(b1, struct lgv_block_seq, parent);
            struct lgv_block_seq  *s2 =
                cork_container_of(b2, struct lgv_block_seq, parent);

            /* s2 will drop its reference to head later, so we need to
             * create a new one */
            lgv_block_set_next(gc, s1->tail, cork_gc_incref(gc, s2->head));
            s1->tail = s2->tail;
            /* s2 is no longer needed */
            cork_gc_decref(gc, b2);
            return b1;
        } else {
            /* b2 isn't a seq, so add it to b1 */
            struct lgv_block_seq  *s1 =
                cork_container_of(b1, struct lgv_block_seq, parent);
            /* set_next takes over the reference to b2 */
            lgv_block_set_next(gc, s1->tail, b2);
            s1->tail = b2;
            return b1;
        }
    } else {
        if (lgv_block_is_seq(b2)) {
            /* b1 isn't a seq, so add it to b2 */
            struct lgv_block_seq  *s2 =
                cork_container_of(b2, struct lgv_block_seq, parent);
            /* set_next takes over the reference to s2->head */
            lgv_block_set_next(gc, b1, s2->head);
            s2->head = b1;
            return b2;
        } else {
            /* Neither is a seq; create a new one */
            make_new(gc, struct lgv_block_seq, lgv_block_seq);
            self->parent.name = "seq";
            self->parent.execute = lgv_block_seq;
            self->parent.set_next = lgv_block_seq_set_next;
            lgv_block_set_next(gc, b1, b2);
            self->head = b1;
            self->tail = b2;
            return &self->parent;
        }
    }
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
lgv_block_new_halt(struct cork_gc *gc)
{
    make_new(gc, struct lgv_block, lgv_block_leaf);
    self->name = "halt";
    self->execute = lgv_block_halt;
    self->set_next = lgv_block_halt_set_next;
    return self;
}
