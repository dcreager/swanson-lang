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

#include "lagavulin/checkers.h"
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
lgv_stack_init(struct cork_gc *gc, struct lgv_stack *self, size_t initial_size)
{
    size_t  allocated_size = sizeof(struct lgv_stack_entry) * initial_size;
    self->entries = cork_malloc(gc->alloc, allocated_size);
    if (self->entries == NULL) {
        return -1;
    }

    self->allocated_size = allocated_size;
    self->size = 0;
    return 0;
}

void
lgv_stack_done(struct cork_gc *gc, struct lgv_stack *self)
{
    if (self->entries != NULL) {
        cork_free(gc->alloc, self->entries, self->allocated_size);
        self->entries = NULL;
    }
}

static int
lgv_stack_ensure_size(struct cork_gc *gc, struct lgv_stack *self, size_t count)
{
    size_t  needed_allocated_size = sizeof(struct lgv_stack_entry) * count;
    if (self->allocated_size >= needed_allocated_size) {
        return 0;
    }

    size_t  new_allocated_size = self->allocated_size * 2;
    if (new_allocated_size < needed_allocated_size) {
        new_allocated_size = needed_allocated_size;
    }

    struct lgv_stack_entry  *new_entries =
        cork_realloc(gc->alloc, self->entries,
                     self->allocated_size, new_allocated_size);
    if (new_entries == NULL) {
        return -1;
    }

    self->entries = new_entries;
    return 0;
}

int
lgv_stack_push(struct cork_gc *gc, struct lgv_stack *self,
               union lgv_stack_contents value)
{
    r_check(lgv_stack_ensure_size(gc, self, self->size + 1));
    struct lgv_stack_entry  *entry = &self->entries[self->size++];
    entry->contents = value;
    return 0;
}

int
lgv_stack_pop(struct cork_gc *gc, struct lgv_stack *self, size_t count)
{
    if (self->size == 0) {
        return -1;
    }
    self->size--;
    return 0;
}
