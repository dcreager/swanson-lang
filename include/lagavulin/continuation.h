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
 * @{
 */

struct lgv_continuation {
    /** @brief Execute the continuation */
    void
    (*result)(struct lgv_continuation *self, void *input);
};

#define lgv_continuation_result(cont, input) \
    ((cont)->result((cont), (input)))

struct lgv_block {
    void
    (*execute)(struct lgv_block *self, struct lgv_continuation *dest);
};

#define lgv_block_execute(block, dest) \
    ((block)->execute((block), (dest)))


/* TODO: Figure out how to make this define not show up in the doxygen. */

#define lgv_block_new_constant(typ_id, typ) \
    /** @brief Create a block that returns a constant <code>typ</code> value. */ \
    struct lgv_block * \
    lgv_block_new_constant_##typ_id(cork_allocator_t *alloc, typ value);

lgv_block_new_constant(bool, bool);
lgv_block_new_constant(int, int);
lgv_block_new_constant(long, long);

#undef lgv_block_new_constant

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
