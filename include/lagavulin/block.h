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
     * @param [in] input  The input value to pass in to the block
     * @since 0.0-dev
     */
    int
    (*execute)(struct lgv_block *self, struct lgv_state *state, void *input);

    /**
     * @brief Sets the “next” pointer for this block.
     * @param [in] self  The block object
     * @param [in] next  The block to pass control to after this block
     * @since 0.0-dev
     */
    void
    (*set_next)(struct lgv_block *self, struct lgv_block *next);
};

/**
 * @brief Execute a code block.
 * @param [in] block  A block object
 * @param [in] input  The input value to pass in to the block
 * finishes
 * @since 0.0-dev
 */
#define lgv_block_execute(block, state, input) \
    ((block)->execute((block), (state), (input)))

/**
 * @brief Sets the “next” pointer for this block.
 * @param [in] block  A block object
 * @param [in] next  The block to pass control to after this block
 * @since 0.0-dev
 */
#define lgv_block_set_next(block, next) \
    ((block)->set_next((block), (next)))


/*-----------------------------------------------------------------------
 * Constants
 */

/**
 * @brief Create a block that returns a constant <code>bool</code> value.
 */
struct lgv_block *
lgv_block_new_constant_bool(cork_allocator_t *alloc, bool value);

/**
 * @brief Create a block that returns a constant <code>int</code> value.
 */
struct lgv_block *
lgv_block_new_constant_int(cork_allocator_t *alloc, int value);

/**
 * @brief Create a block that returns a constant <code>long</code> value.
 */
struct lgv_block *
lgv_block_new_constant_long(cork_allocator_t *alloc, long value);


/*-----------------------------------------------------------------------
 * Control structures
 */

/**
 * @brief Create a block for an <code>if</code> statement.
 */
struct lgv_block *
lgv_block_new_if(cork_allocator_t *alloc,
                 struct lgv_block *condition,
                 struct lgv_block *true_branch,
                 struct lgv_block *false_branch);

/**
 * @brief Create a block for a <code>return</code> statement.
 */
struct lgv_block *
lgv_block_new_return(cork_allocator_t *alloc);

/**
 * @brief Create a block that stores the input pointer into
 * <code>dest</code> whenever it's called.
 */
struct lgv_block *
lgv_block_new_collect(cork_allocator_t *alloc, void **dest);


/*-----------------------------------------------------------------------
 * Execution states
 */

/**
 * @brief Represents the current state of execution.
 * @since 0.0-dev
 */

struct lgv_state {
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
lgv_state_init(struct lgv_state *state);

/**
 * @brief Finalize an execution state.
 * @param [in] state  An execution state
 * @since 0.0-dev
 */
void
lgv_state_done(struct lgv_state *state);

/* end of block group */
/**
 * @}
 */

#endif  /* LAGAVULIN_BLOCK_H */
