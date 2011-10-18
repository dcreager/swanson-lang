/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef LAGAVULIN_STACK_H
#define LAGAVULIN_STACK_H

/**
 * @file
 * @brief Implementation of the @ref stack module
 */

#include <libcork/core.h>

#include <lagavulin/checkers.h>

/**
 * @addtogroup stack Stacks
 *
 * <tt>#%include \<lagavulin/stack.h\></tt>
 *
 * The lagavulin execution engine is stack-based.  This module defines
 * the stack that is used to pass values between the different operands
 * and blocks that are executing.
 *
 * @{
 */


/**
 * @brief The raw value of an entry in the stack
 * @since 0.0-dev
 */

union lgv_stack_contents {
    void  *vp;
    bool  b;
    char  sc;
    unsigned char  uc;
    short  ss;
    unsigned short  us;
    int  si;
    unsigned int  ui;
    long  sl;
    unsigned long  ul;
    int8_t  s8;
    uint8_t  u8;
    int16_t  s16;
    uint16_t  u16;
    int32_t  s32;
    uint32_t  u32;
    int64_t  s64;
    uint64_t  u64;
    float  f;
    double  d;
} contents;

/**
 * @brief An individual entry in a stack.
 * @since 0.0-dev
 */
struct lgv_stack_entry {
    /** @brief The contents of this entry. @private */
    union lgv_stack_contents  contents;
};

/**
 * @brief An execution stack.
 * @since 0.0-dev
 */
struct lgv_stack {
    /** @brief A pointer to the top-most element of the stack. */
    struct lgv_stack_entry  *top;
    /** @brief A resizable array of the entries in this stack. */
    struct lgv_stack_entry  *entries;
    /** @brief The number of allocated entries in the stack. */
    size_t  allocated_count;
};

/**
 * @brief The default number of elements that are preallocated in a
 * stack object.
 * @since 0.0-dev
 */
#define LGV_STACK_DEFAULT_INITIAL_SIZE  20

/* end of stack group */
/**
 * @}
 */


/**
 * @brief Initialize a new stack object.
 * @public @memberof lgv_stack
 * @since 0.0-dev
 */
int
lgv_stack_init(struct cork_gc *gc, struct lgv_stack *stack, size_t initial_count);

/**
 * @brief Finalize a stack object.
 * @public @memberof lgv_stack
 * @since 0.0-dev
 */
void
lgv_stack_done(struct cork_gc *gc, struct lgv_stack *stack);

/**
 * @brief Return the number of used entries in the stack.
 * @public @memberof lgv_stack
 * @since 0.0-dev
 */
#define lgv_stack_count(stack) \
    ((size_t) ((stack)->top - (stack)->entries + 1))

/**
 * @brief Make sure that a stack is at least the given size.
 * @public @memberof lgv_stack
 * @since 0.0-dev
 */
int
lgv_stack_ensure_size(struct cork_gc *gc, struct lgv_stack *self, size_t count);


/**
 * @brief Push a new element onto the stack.
 * @public @memberof lgv_stack
 * @since 0.0-dev
 */

#define lgv_stack_push(gc, stack, top, union_branch, value) \
    do { \
        (top)++; \
        (top)->contents.union_branch = (value); \
    } while (0)

/**
 * @brief Pop an element off of the stack.
 * @public @memberof lgv_stack
 * @since 0.0-dev
 */
#define lgv_stack_pop(gc, stack, top, count) \
    do { \
        (top) -= count; \
    } while (0)

/**
 * @brief Get the element at the given index of the stack.
 * @public @memberof lgv_stack
 * @since 0.0-dev
 */
#define lgv_stack_get(stack, top, index, union_branch) \
    ((index) >= 0? (stack)->entries[(index)].contents.union_branch: \
     (top)[1 + (index)].contents.union_branch)


#endif  /* LAGAVULIN_STACK_H */
