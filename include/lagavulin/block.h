/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef LAGAVULIN_BLOCK_H
#define LAGAVULIN_BLOCK_H

/**
 * @file
 * @brief Implementation of the @ref block module
 */

#include <libcork/core.h>

#include <lagavulin/stack.h>

/**
 * @addtogroup block Blocks
 *
 * <tt>#%include \<lagavulin/block.h\></tt>
 *
 * This module implements <em>blocks</em>, which provide the execution
 * fabric for the Swanson language.  Each chunk of code that can be
 * executed is represented by a <em>block</em>.  Blocks are linked
 * together via a “next” pointer, which is used to implement both the
 * sequence of blocks in a linear code path, and the depth-first
 * execution of an expression tree.  Blocks can produce an output value,
 * which is passed into the execution function for the next block.
 *
 * Blocks are implemented as structs that contain a function pointer.
 * This struct is intended to be embedded within custom types for the
 * different specific block implementations.
 *
 * @{
 */

/*-----------------------------------------------------------------------
 * Blocks
 */

/* forward declaration */

struct lgv_state;

/**
 * @brief A chunk of code that can be executed.
 * @since 0.0-dev
 */

struct lgv_block {
    /**
     * @brief Execute the code block.
     * @param [in] self  The block object
     * @since 0.0-dev
     */
    int
    (*execute)(struct cork_gc *gc,
               struct lgv_block *self, struct lgv_state *state);

    /**
     * @brief A debug name for the block.
     * @since 0.0-dev
     */
    const char  *name;

    /**
     * @brief Sets the “next” pointer for this block.
     * @param [in] self  The block object
     * @param [in] next  The block to pass control to after this block
     * @since 0.0-dev
     */
    void
    (*set_next)(struct cork_gc *gc,
                struct lgv_block *self, struct lgv_block *next);
};

/**
 * @brief Execute a code block.
 * @param [in] block  A block object
 * @since 0.0-dev
 */
#define lgv_block_execute(gc, block, state) \
    ((block)->execute((gc), (block), (state)))

/**
 * @brief Sets the “next” pointer for this block.
 * @param [in] block  A block object
 * @param [in] next  The block to pass control to after this block
 * @since 0.0-dev
 */
#define lgv_block_set_next(gc, block, next) \
    ((block)->set_next((gc), (block), (next)))


/*-----------------------------------------------------------------------
 * Constants
 */

/**
 * @brief Create a block that returns a constant <code>bool</code> value.
 */
struct lgv_block *
lgv_block_new_constant_bool(struct cork_gc *gc, bool value);

/**
 * @brief Create a block that returns a constant <code>int</code> value.
 */
struct lgv_block *
lgv_block_new_constant_int(struct cork_gc *gc, int value);

/**
 * @brief Create a block that returns a constant <code>long</code> value.
 */
struct lgv_block *
lgv_block_new_constant_long(struct cork_gc *gc, long value);


/*-----------------------------------------------------------------------
 * Arithmetic
 */

struct lgv_block *
lgv_block_new_dup(struct cork_gc *gc);

struct lgv_block *
lgv_block_new_add_int(struct cork_gc *gc);

struct lgv_block *
lgv_block_new_lt_int(struct cork_gc *gc);


/*-----------------------------------------------------------------------
 * Control structures
 */

/**
 * @brief Create a block for an <code>if</code> statement.
 */
struct lgv_block *
lgv_block_new_if(struct cork_gc *gc,
                 struct lgv_block *condition,
                 struct lgv_block *true_branch,
                 struct lgv_block *false_branch);

/**
 * @brief Create a block for a <code>return</code> statement.
 */
struct lgv_block *
lgv_block_new_return(struct cork_gc *gc);

/**
 * @brief Create a block for a <code>seq</code> statement.
 */
struct lgv_block *
lgv_block_new_seq(struct cork_gc *gc,
                  struct lgv_block *b1, struct lgv_block *b2);

/**
 * @brief Create a block for a <code>while</code> statement.
 */
struct lgv_block *
lgv_block_new_while(struct cork_gc *gc,
                    struct lgv_block *condition,
                    struct lgv_block *body);

/**
 * @brief Create a block that halts execution.
 */
struct lgv_block *
lgv_block_new_halt(struct cork_gc *gc);


/*-----------------------------------------------------------------------
 * Execution states
 */

/**
 * @brief Represents the current state of execution.
 * @since 0.0-dev
 */

struct lgv_state {
    /**
     * @brief The stack for the current function.
     * @since 0.0-dev
     */
    struct lgv_stack  stack;

    /**
     * @brief The “return” pointer for the currently executing function.
     * @since 0.0-dev
     */
    struct lgv_block  *ret;
};

/**
 * @brief Initialize a new execution state.
 * @param [in] state  An execution state
 * @since 0.0-dev
 */
int
lgv_state_init(struct cork_gc *gc, struct lgv_state *state);

/**
 * @brief Finalize an execution state.
 * @param [in] state  An execution state
 * @since 0.0-dev
 */
void
lgv_state_done(struct cork_gc *gc, struct lgv_state *state);


/* end of block group */
/**
 * @}
 */

#endif  /* LAGAVULIN_BLOCK_H */
