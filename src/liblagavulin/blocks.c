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
    struct lgv_block_brancher  *brancher;
};

struct lgv_block_seq {
    struct lgv_block  parent;
    /* We hold a reference to head, but not to tail */
    struct lgv_block  *head;
    struct lgv_block  *tail;
};

struct lgv_block_while {
    struct lgv_block  parent;
    struct lgv_block  *condition;
    struct lgv_block_brancher  *brancher;
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


static void
lgv_block_while_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct lgv_block_while  *self = vself;
    recurse(self->condition, ud);
    recurse(self->brancher, ud);
}

static struct cork_gc_obj_iface  lgv_block_while_gc = {
    BLOCK_FREE, lgv_block_while_recurse
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
                                struct lgv_state *state, struct lgv_stack_entry *top) \
    { \
        struct lgv_block_constant_##typ_id  *self = \
            cork_container_of \
            (vself, struct lgv_block_constant_##typ_id, parent.parent); \
        DEBUG("%p %s", vself, vself->name); \
        lgv_stack_push(gc, &state->stack, top, union_branch, self->value); \
        return lgv_block_execute(gc, self->parent.next, state, top); \
    }

constant_execute(bool, bool, b);
constant_execute(int, int, si);
constant_execute(long, long, sl);

static int
lgv_block_dup(struct cork_gc *gc, struct lgv_block *vself,
              struct lgv_state *state, struct lgv_stack_entry *top)
{
    struct lgv_block_simple  *self =
        cork_container_of(vself, struct lgv_block_simple, parent);
    DEBUG("%p %s", vself, vself->name);
    top++;
    top[0] = top[-1];
    return lgv_block_execute(gc, self->next, state, top);
}

static int
lgv_block_add_int(struct cork_gc *gc, struct lgv_block *vself,
                  struct lgv_state *state, struct lgv_stack_entry *top)
{
    struct lgv_block_simple  *self =
        cork_container_of(vself, struct lgv_block_simple, parent);
    int  lhs = lgv_stack_get(&state->stack, top, -2, si);
    int  rhs = lgv_stack_get(&state->stack, top, -1, si);
    DEBUG("%p %s: %d %d", vself, vself->name, lhs, rhs);
    lgv_stack_pop(gc, &state->stack, top, 2);
    lgv_stack_push(gc, &state->stack, top, si, lhs + rhs);
    return lgv_block_execute(gc, self->next, state, top);
}

static int
lgv_block_lt_int(struct cork_gc *gc, struct lgv_block *vself,
                 struct lgv_state *state, struct lgv_stack_entry *top)
{
    struct lgv_block_simple  *self =
        cork_container_of(vself, struct lgv_block_simple, parent);
    int  lhs = lgv_stack_get(&state->stack, top, -2, si);
    int  rhs = lgv_stack_get(&state->stack, top, -1, si);
    DEBUG("%p %s: %d %d", vself, vself->name, lhs, rhs);
    lgv_stack_pop(gc, &state->stack, top, 2);
    lgv_stack_push(gc, &state->stack, top, b, lhs < rhs);
    return lgv_block_execute(gc, self->next, state, top);
}

static int
lgv_block_brancher(struct cork_gc *gc, struct lgv_block *vself,
                   struct lgv_state *state, struct lgv_stack_entry *top)
{
    struct lgv_block_brancher  *self =
        cork_container_of(vself, struct lgv_block_brancher, parent);
    bool  input = lgv_stack_get(&state->stack, top, -1, b);
    lgv_stack_pop(gc, &state->stack, top, 1);
    DEBUG("%p %s: %s", vself, vself->name, input? "true": "false");

    if (input) {
        return lgv_block_execute(gc, self->true_branch, state, top);
    } else {
        return lgv_block_execute(gc, self->false_branch, state, top);
    }
}

static int
lgv_block_return(struct cork_gc *gc, struct lgv_block *vself,
                 struct lgv_state *state, struct lgv_stack_entry *top)
{
    DEBUG("%p %s", vself, vself->name);
    return lgv_block_execute(gc, state->ret, state, top);
}

static int
lgv_block_halt(struct cork_gc *gc, struct lgv_block *vself,
               struct lgv_state *state, struct lgv_stack_entry *top)
{
    DEBUG("%p %s", vself, vself->name);
    state->stack.top = top;
    return 0;
}


/*-----------------------------------------------------------------------
 * get_head methods
 */

static struct lgv_block *
lgv_block_simple_get_head(struct lgv_block *vself)
{
    return vself;
}

static struct lgv_block *
lgv_block_if_get_head(struct lgv_block *vself)
{
    struct lgv_block_if  *self =
        cork_container_of(vself, struct lgv_block_if, parent);
    return lgv_block_get_head(self->condition);
}

static struct lgv_block *
lgv_block_seq_get_head(struct lgv_block *vself)
{
    struct lgv_block_seq  *self =
        cork_container_of(vself, struct lgv_block_seq, parent);
    return lgv_block_get_head(self->head);
}

static struct lgv_block *
lgv_block_while_get_head(struct lgv_block *vself)
{
    struct lgv_block_while  *self =
        cork_container_of(vself, struct lgv_block_while, parent);
    return lgv_block_get_head(self->condition);
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
    struct lgv_block  *head = lgv_block_get_head(next);
    if (head != next) {
        cork_gc_incref(gc, head);
        cork_gc_decref(gc, next);
    }
    self->next = head;
}

static void
lgv_block_if_set_next(struct cork_gc *gc,
                      struct lgv_block *vself, struct lgv_block *next)
{
    struct lgv_block_if  *self =
        cork_container_of(vself, struct lgv_block_if, parent);

    struct lgv_block  *head = lgv_block_get_head(next);
    if (head != next) {
        cork_gc_incref(gc, head);
        cork_gc_decref(gc, next);
    }

    /*
     * We're assuming control of one reference, which we can pass on to
     * one of the branches.  The other branch needs a new reference.
     */
    cork_gc_incref(gc, head);
    lgv_block_set_next(gc, self->brancher->true_branch, head);
    lgv_block_set_next(gc, self->brancher->false_branch, head);
}

static void
lgv_block_seq_set_next(struct cork_gc *gc,
                         struct lgv_block *vself, struct lgv_block *next)
{
    struct lgv_block_seq  *self =
        cork_container_of(vself, struct lgv_block_seq, parent);
    struct lgv_block  *head = lgv_block_get_head(next);
    if (head != next) {
        cork_gc_incref(gc, head);
        cork_gc_decref(gc, next);
    }
    lgv_block_set_next(gc, self->tail, head);
}

static void
lgv_block_while_set_next(struct cork_gc *gc,
                         struct lgv_block *vself, struct lgv_block *next)
{
    struct lgv_block_while  *self =
        cork_container_of(vself, struct lgv_block_while, parent);
    struct lgv_block  *head = lgv_block_get_head(next);
    if (head != next) {
        cork_gc_incref(gc, head);
        cork_gc_decref(gc, next);
    }
    self->brancher->false_branch = head;
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

static const struct lgv_block_iface  lgv_block_simple_iface = {
    lgv_block_simple_get_head,
    lgv_block_simple_set_next
};

#define constant_new(typ_id, typ) \
    struct lgv_block * \
    lgv_block_new_constant_##typ_id(struct cork_gc *gc, typ value) \
    { \
        make_new(gc, struct lgv_block_constant_##typ_id, lgv_block_simple); \
        self->parent.parent.name = "constant_" #typ_id; \
        self->parent.parent.execute = lgv_block_constant_##typ_id; \
        self->parent.parent.iface = &lgv_block_simple_iface; \
        self->value = value; \
        return &self->parent.parent; \
    }

constant_new(bool, bool);
constant_new(int, int);
constant_new(long, long);

struct lgv_block *
lgv_block_new_dup(struct cork_gc *gc)
{
    make_new(gc, struct lgv_block_simple, lgv_block_simple);
    self->parent.name = "dup";
    self->parent.execute = lgv_block_dup;
    self->parent.iface = &lgv_block_simple_iface;
    return &self->parent;
}

struct lgv_block *
lgv_block_new_add_int(struct cork_gc *gc)
{
    make_new(gc, struct lgv_block_simple, lgv_block_simple);
    self->parent.name = "add-int";
    self->parent.execute = lgv_block_add_int;
    self->parent.iface = &lgv_block_simple_iface;
    return &self->parent;
}

struct lgv_block *
lgv_block_new_lt_int(struct cork_gc *gc)
{
    make_new(gc, struct lgv_block_simple, lgv_block_simple);
    self->parent.name = "lt-int";
    self->parent.execute = lgv_block_lt_int;
    self->parent.iface = &lgv_block_simple_iface;
    return &self->parent;
}

static const struct lgv_block_iface  lgv_block_brancher_iface = {
    lgv_block_simple_get_head,
    NULL
};

static struct lgv_block_brancher *
lgv_block_new_brancher(struct cork_gc *gc,
                       const char *name,
                       struct lgv_block *true_branch,
                       struct lgv_block *false_branch)
{
    make_new(gc, struct lgv_block_brancher, lgv_block_brancher);
    self->parent.name = name;
    self->parent.execute = lgv_block_brancher;
    self->parent.iface = &lgv_block_brancher_iface;
    if (true_branch != NULL) {
        self->true_branch = lgv_block_get_head(true_branch);
    }
    if (false_branch != NULL) {
        self->false_branch = lgv_block_get_head(false_branch);
    }
    return self;
}

static const struct lgv_block_iface  lgv_block_if_iface = {
    lgv_block_if_get_head,
    lgv_block_if_set_next
};

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
    self->parent.execute = NULL;
    self->parent.iface = &lgv_block_if_iface;
    self->condition = condition;
    cork_gc_incref(gc, self->brancher);
    lgv_block_set_next(gc, self->condition, &self->brancher->parent);
    return &self->parent;
}


static const struct lgv_block_iface  lgv_block_seq_iface = {
    lgv_block_seq_get_head,
    lgv_block_seq_set_next
};

#define lgv_block_is_seq(block)  ((block)->iface == &lgv_block_seq_iface)

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
            cork_gc_incref(gc, s2->head);
            lgv_block_set_next(gc, s1->tail, s2->head);
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
            self->parent.execute = NULL;
            self->parent.iface = &lgv_block_seq_iface;
            lgv_block_set_next(gc, b1, b2);
            self->head = b1;
            self->tail = b2;
            return &self->parent;
        }
    }
}

static const struct lgv_block_iface  lgv_block_while_iface = {
    lgv_block_while_get_head,
    lgv_block_while_set_next
};

struct lgv_block *
lgv_block_new_while(struct cork_gc *gc,
                    struct lgv_block *condition,
                    struct lgv_block *body)
{
    make_new(gc, struct lgv_block_while, lgv_block_while);

    self->brancher =
        lgv_block_new_brancher(gc, "while-brancher", body, NULL);
    if (self->brancher == NULL) {
        cork_gc_decref(gc, self);
        return NULL;
    }

    self->parent.name = "while";
    self->parent.execute = NULL;
    self->parent.iface = &lgv_block_while_iface;
    self->condition = condition;
    cork_gc_incref(gc, self->brancher);
    lgv_block_set_next(gc, self->condition, &self->brancher->parent);
    cork_gc_incref(gc, self->condition);
    lgv_block_set_next(gc, body, self->condition);
    return &self->parent;
}

static const struct lgv_block_iface  lgv_block_return_iface = {
    lgv_block_simple_get_head,
    lgv_block_return_set_next
};

struct lgv_block *
lgv_block_new_return(struct cork_gc *gc)
{
    make_new(gc, struct lgv_block, lgv_block_leaf);
    self->name = "return";
    self->execute = lgv_block_return;
    self->iface = &lgv_block_return_iface;
    return self;
}

static const struct lgv_block_iface  lgv_block_halt_iface = {
    lgv_block_simple_get_head,
    lgv_block_halt_set_next
};

struct lgv_block *
lgv_block_new_halt(struct cork_gc *gc)
{
    make_new(gc, struct lgv_block, lgv_block_leaf);
    self->name = "halt";
    self->execute = lgv_block_halt;
    self->iface = &lgv_block_halt_iface;
    return self;
}
