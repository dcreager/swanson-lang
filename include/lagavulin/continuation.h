/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef LAGAVULIN_CONTINUATION_H
#define LAGAVULIN_CONTINUATION_H

/**
 * @file
 * @brief Implementation of the @ref continuation module
 */

#include <libcork/core.h>

/**
 * @addtogroup continuation Continuations
 *
 * <tt>#%include \<lagavulin/continuation.h\></tt>
 *
 * This module implements a system of blocks and continuations that are
 * used to implement the Swanson language.  A <em>block</em> represents
 * a chunk of code that can be executed.  When it finishes, it passes
 * control to a <em>continuation</em>, passing it the code's final
 * result.
 *
 * Blocks and continuations are both implemented as structs that contain
 * a function pointer.  These structs are intended to be embedded within
 * custom types for the different specific implementations of blocks and
 * continuations.
 *
 * @{
 */

/*-----------------------------------------------------------------------
 * Blocks and continuations
 */

/**
 * @brief An object that a block passes control to when it finishes.
 * @since 0.0-dev
 */

struct lgv_continuation {
    /**
     * @brief Fire the continuation
     * @param [in] self  The continuation object
     * @param [in] input  The result of the block
     * @since 0.0-dev
     */
    void
    (*result)(struct lgv_continuation *self, void *input);
};

/**
 * @brief Call a continuation with the given result value.
 * @param [in] cont  A continuation object
 * @param [in] input  The result of the block
 */

#define lgv_continuation_result(cont, input) \
    ((cont)->result((cont), (input)))

/**
 * @brief A chunk of code that can be executed.
 * @since 0.0-dev
 */

struct lgv_block {
    /**
     * @brief Execute the code block.
     * @param [in] self  The block object
     * @param [in] dest  The continuation to pass control to when the
     * block finishes
     * @since 0.0-dev
     */
    void
    (*execute)(struct lgv_block *self, struct lgv_continuation *dest);
};

/**
 * @brief Execute a code block.
 * @param [in] block  A block object
 * @param [in] dest  The continuation to pass control to when the block
 * finishes
 * @since 0.0-dev
 */
#define lgv_block_execute(block, dest) \
    ((block)->execute((block), (dest)))


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
 * @brief Create a continuation that stores the result pointer into
 * <code>dest</code> whenever it's called.
 */

struct lgv_continuation *
lgv_continuation_new_collect(cork_allocator_t *alloc, void **dest);


/* end of block group */
/**
 * @}
 */

#endif  /* LAGAVULIN_CONTINUATION_H */
