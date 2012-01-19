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
#include <libcork/core/checkers.h>
#include <libcork/ds.h>

#include "swanson/s0.h"
#include "swanson/state.h"


static bool
s0_product_type_satisfies(struct swan *s, struct s0_type *self,
                          struct s0_type *other, struct cork_error *err)
{
    /* Check each element of the tuple recursively. */
    size_t  i;
    size_t  self_size = cork_array_size(&self->_.product);
    size_t  other_size = cork_array_size(&other->_.product);
    if (self_size != other_size) {
        return false;
    }

    for (i = 0; i < self_size; i++) {
        struct s0_type  *self_child = cork_array_at(&self->_.product, i);
        struct s0_type  *other_child = cork_array_at(&other->_.product, i);
        if (!s0_type_satisfies(s, self_child, other_child, err)) {
            return false;
        }
    }

    return true;
}

/* self :: INTERFACE, other :: INTERFACE */
static bool
s0_interface_entries_satisfy(struct swan *s, struct cork_hash_table *self,
                             struct cork_hash_table *other,
                             struct cork_error *err)
{
    /* Every entry in other must have a corresponding entry in self, and
     * the type of self's entry must satisfy the type of other's entry. */
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;

    cork_hash_table_iterator_init(other, &iter);
    while ((entry = cork_hash_table_iterator_next(other, &iter)) != NULL) {
        struct s0_type  *self_child;
        struct s0_type  *other_child = entry->value;
        struct cork_hash_table_entry  *self_entry =
            cork_hash_table_get_entry(swan_alloc(s), self, entry->key);

        if (self_entry == NULL) {
            return false;
        }

        self_child = self_entry->value;
        if (!s0_type_satisfies(s, self_child, other_child, err)) {
            return false;
        }
    }

    /* It's okay for there to be extra entries in self. */
    return true;
}

/* self :: CLASS, other :: INTERFACE */
static bool
s0_class_entries_satisfy(struct swan *s, struct cork_hash_table *self,
                         struct cork_hash_table *other, struct cork_error *err)
{
    /* Every entry in other must have a corresponding entry in self, and
     * the type of self's entry must satisfy the type of other's entry. */
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;

    cork_hash_table_iterator_init(other, &iter);
    while ((entry = cork_hash_table_iterator_next(other, &iter)) != NULL) {
        struct s0_type  *self_child;
        struct s0_type  *other_child = entry->value;
        struct cork_hash_table_entry  *self_entry =
            cork_hash_table_get_entry(swan_alloc(s), self, entry->key);

        if (self_entry == NULL) {
            return false;
        }

        xp_check(false, self_child = s0_value_get_type
                 (s, self_entry->value, err));
        if (!s0_type_satisfies(s, self_child, other_child, err)) {
            return false;
        }
    }

    /* It's okay for there to be extra entries in self. */
    return true;
}

bool
s0_type_satisfies(struct swan *s, struct s0_type *self,
                  struct s0_type *other, struct cork_error *err)
{
    /* x <: x */
    if (self == other) {
        return true;
    }

    /* Follow recursive links */
    while (self != NULL && self->kind == S0_TYPE_RECURSIVE) {
        self = self->_.recursive.resolved;
    }

    while (other != NULL && other->kind == S0_TYPE_RECURSIVE) {
        other = other->_.recursive.resolved;
    }

    /* NULL == void type */
    if (self == NULL) {
        return
            /* x <: ANY */
            (other->kind == S0_TYPE_ANY) ||
            /* x <: x */
            ((other->kind == S0_TYPE_PRODUCT) &&
             cork_array_is_empty(&other->_.product));
    }

    if (other == NULL) {
        return
            /* x <: x */
            ((self->kind == S0_TYPE_PRODUCT) &&
             cork_array_is_empty(&self->_.product));
    }

    /* At this point, self and other are both guaranteed to be non-NULL,
     * distinct pointers. */

    switch (other->kind) {
        case S0_TYPE_TYPE:
            /* x <: x */
            return self->kind == S0_TYPE_TYPE;

        case S0_TYPE_LITERAL:
            /* x <: x */
            return self->kind == S0_TYPE_LITERAL;

        case S0_TYPE_ANY:
            /* x <: ANY */
            return true;

        case S0_TYPE_PRODUCT:
            /* x1 <: y1, ... => (x1, ...) <: (y1, ...) */
            return (self->kind == S0_TYPE_PRODUCT) &&
                s0_product_type_satisfies(s, self, other, err);

        case S0_TYPE_FUNCTION:
            /* xp :> yp, xr <: yp => xp -> xr <: yp -> yr */
            return (self->kind == S0_TYPE_FUNCTION) &&
                s0_type_satisfies
                    (s, other->_.function.input,
                     self->_.function.input, err) &&
                s0_type_satisfies
                    (s, self->_.function.output,
                     other->_.function.output, err);

        case S0_TYPE_LOCATION:
            /* x <: y => *x <: *y */
            return (self->kind == S0_TYPE_LOCATION) &&
                s0_type_satisfies
                    (s, self->_.location.referent,
                     other->_.location.referent, err);

        case S0_TYPE_INTERFACE:
            /* Any class or interface can satisfy an interface */
            if (self->kind == S0_TYPE_INTERFACE) {
                return s0_interface_entries_satisfy
                    (s, &self->_.interface.entries,
                     &other->_.interface.entries, err);
            }

            if (self->kind == S0_TYPE_CLASS) {
                return s0_class_entries_satisfy
                    (s, &self->_.cls.entries,
                     &other->_.interface.entries, err);
            }

            return false;

        case S0_TYPE_CLASS:
            /* Classes can only match exactly, and we've already checked
             * whether the type pointers are identical. */
            return false;

        case S0_TYPE_BLOCK:
            /* x <: y => {x} <: {y} */
            return (self->kind == S0_TYPE_BLOCK) &&
                s0_type_satisfies
                    (s, self->_.block.result, other->_.block.result, err);

        default:
            return false;
    }
}
