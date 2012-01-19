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

#include "lagavulin/stack.h"

#if !defined(STACK_DEBUG)
#define STACK_DEBUG 0
#endif

#if STACK_DEBUG
#include <stdio.h>
#define DEBUG(...) \
    do { \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while (0)
#else
#define DEBUG(...) /* no debug messages */
#endif


int
lgv_stack_init(struct swan *s, struct lgv_stack *self, size_t initial_count)
{
    size_t  allocated_size = sizeof(struct lgv_stack_entry) * initial_count;
    self->entries = cork_malloc(swan_alloc(s), allocated_size);
    if (self->entries == NULL) {
        return -1;
    }

    self->top = self->entries - 1;
    self->allocated_count = initial_count;
    return 0;
}

void
lgv_stack_done(struct swan *s, struct lgv_stack *self)
{
    if (self->entries != NULL) {
        size_t  allocated_size =
            sizeof(struct lgv_stack_entry) * self->allocated_count;
        cork_free(swan_alloc(s), self->entries, allocated_size);
        self->entries = NULL;
    }
    self->top = NULL;
}

int
lgv_stack_ensure_size(struct swan *s, struct lgv_stack *self, size_t count)
{
    if (self->allocated_count >= count) {
        return 0;
    }

    size_t  old_allocated_size =
        sizeof(struct lgv_stack_entry) * self->allocated_count;
    size_t  needed_allocated_size = sizeof(struct lgv_stack_entry) * count;
    size_t  new_allocated_size = old_allocated_size * 2;
    if (new_allocated_size < needed_allocated_size) {
        new_allocated_size = needed_allocated_size;
    }

    struct lgv_stack_entry  *new_entries =
        cork_realloc(swan_alloc(s), self->entries,
                     old_allocated_size, new_allocated_size);
    if (new_entries == NULL) {
        return -1;
    }

    self->top = new_entries + (self->top - self->entries);
    self->entries = new_entries;
    return 0;
}
