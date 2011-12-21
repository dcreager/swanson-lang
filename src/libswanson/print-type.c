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


static const char  *TYPE_NAMES[] = {
    "α", "β", "γ", "δ", "ε", "ζ", "η", "θ",
    "ι", "κ", "λ", "μ", "ν", "ξ", "ο", "π",
    "ρ", "σ", "τ", "υ", "φ", "χ", "ψ", "ω",
};

#define TYPE_NAME_COUNT  (sizeof(TYPE_NAMES) / sizeof(TYPE_NAMES[0]))


struct s0_printer {
    /* A mapping of interface types to integer indices. */
    struct cork_hash_table  interface_indices;

    /* and vice versa */
    struct cork_hash_table  interfaces;

    /* The next available type index. */
    size_t  next_index;
};

static cork_hash
constant_hasher(const void *key)
{
    return (cork_hash) (intptr_t) key;
}

static bool
constant_comparator(const void *k1, const void *k2)
{
    return (k1 == k2);
}

static int
print_one(struct swan *s, struct s0_printer *state,
          struct s0_type *type, struct cork_buffer *dest,
          bool parenthize, struct cork_error *err);

static int
assign_id(struct swan *s, struct s0_printer *state,
          struct s0_interface_type *itype, size_t *index,
          struct cork_error *err)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry = NULL;
    rip_check(entry = cork_hash_table_get_or_create
              (swan_alloc(s), &state->interface_indices,
               itype, &is_new, err));

    if (is_new) {
        *index = state->next_index++;
        entry->value = (void *) (intptr_t) *index;
        rii_check(cork_hash_table_put
                  (swan_alloc(s), &state->interfaces,
                   entry->value, itype, NULL, NULL, NULL, err));
    } else {
        *index = (size_t) (intptr_t) entry->value;
    }
    return 0;
}

static int
print_interface_name(struct swan *s, size_t index, struct cork_buffer *dest,
                     struct cork_error *err)
{
    if (index < TYPE_NAME_COUNT) {
        return cork_buffer_append_string
            (swan_alloc(s), dest, TYPE_NAMES[index], err);
    } else {
        return cork_buffer_append_printf
            (swan_alloc(s), dest, err, "i%zu", index - TYPE_NAME_COUNT);
    }
}

static int
print_type_list(struct swan *s, struct s0_printer *state,
                struct s0_type_list *list, struct cork_buffer *dest,
                struct cork_error *err)
{

    if (list == NULL) {
        return cork_buffer_append_string(swan_alloc(s), dest, "void", err);
    } else {
        bool  first = true;
        struct s0_type_list  *curr;
        for (curr = list; curr != NULL; curr = curr->tail) {
            if (first) {
                first = false;
            } else {
                rii_check(cork_buffer_append_string
                          (swan_alloc(s), dest, ",", err));
            }
            rii_check(print_one(s, state, curr->head, dest, true, err));
        }
    }
    return 0;
}

static int
print_one(struct swan *s, struct s0_printer *state,
          struct s0_type *type, struct cork_buffer *dest,
          bool parenthize, struct cork_error *err)
{
    switch (type->kind) {
        case S0_KIND_LITERAL:
            return cork_buffer_append_string
                (swan_alloc(s), dest, "LITERAL", err);

        case S0_KIND_FUNCTION:
        {
            struct s0_function_type  *ftype =
                cork_container_of(type, struct s0_function_type, parent);

            if (parenthize) {
                rii_check(cork_buffer_append_string
                          (swan_alloc(s), dest, "(", err));
            }
            rii_check(print_type_list(s, state, ftype->params, dest, err));
            rii_check(cork_buffer_append_string
                      (swan_alloc(s), dest, " -> ", err));
            rii_check(print_type_list(s, state, ftype->results, dest, err));
            if (parenthize) {
                rii_check(cork_buffer_append_string
                          (swan_alloc(s), dest, ")", err));
            }
            return 0;
        }

        case S0_KIND_LOCATION:
        {
            struct s0_location_type  *ltype =
                cork_container_of(type, struct s0_location_type, parent);
            rii_check(cork_buffer_append_string
                      (swan_alloc(s), dest, "*", err));
            return print_one(s, state, ltype->referent, dest, true, err);
        }

        case S0_KIND_INTERFACE:
        {
            struct s0_interface_type  *itype =
                cork_container_of(type, struct s0_interface_type, parent);
            size_t  index;
            rii_check(assign_id(s, state, itype, &index, err));
            return print_interface_name(s, index, dest, err);
        }

        case S0_KIND_RECURSIVE:
        {
            struct s0_recursive_type  *rtype =
                cork_container_of(type, struct s0_recursive_type, parent);
            return print_one(s, state, rtype->resolved, dest, parenthize, err);
        }

        default:
            return -1;
    }
}

static int
print_interface(struct swan *s, struct s0_printer *state,
                size_t index, struct cork_buffer *dest,
                struct cork_error *err)
{
    struct s0_interface_type  *itype = NULL;
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;
    rip_check(itype = cork_hash_table_get
              (swan_alloc(s), &state->interfaces,
               (void *) (intptr_t) index));

    if (index > 0) {
        rii_check(cork_buffer_append_string(swan_alloc(s), dest, "\n", err));
    }

    rii_check(cork_buffer_append_string
              (swan_alloc(s), dest, "interface ", err));
    rii_check(print_interface_name(s, index, dest, err));
    rii_check(cork_buffer_append_string
              (swan_alloc(s), dest, " {\n", err));
    cork_hash_table_iterator_init(&itype->entries, &iter);
    while ((entry = cork_hash_table_iterator_next(&itype->entries, &iter))
           != NULL) {
        const char  *name = entry->key;
        rii_check(cork_buffer_append_printf
                  (swan_alloc(s), dest, err, "  %s ", name));
        rii_check(print_one(s, state, entry->value, dest, true, err));
        rii_check(cork_buffer_append_string
                  (swan_alloc(s), dest, "\n", err));
    }
    rii_check(cork_buffer_append_string
              (swan_alloc(s), dest, "}\n", err));

    return 0;
}


int
s0_type_print(struct swan *s, struct s0_type *type,
              struct cork_buffer *dest, struct cork_buffer *givens,
              struct cork_error *err)
{
    size_t  i;
    struct s0_printer  state;
    rii_check(cork_hash_table_init
              (swan_alloc(s), &state.interface_indices, 0,
               constant_hasher, constant_comparator, err));
    rii_check(cork_hash_table_init
              (swan_alloc(s), &state.interfaces, 0,
               constant_hasher, constant_comparator, err));
    state.next_index = 0;

    /* Print the type into dest.  If type contains references to any
     * interfaces, we'll assign IDs to them as part of printing the
     * overall type.  This will give us references to all interface
     * types in the state.interfaces hash table. */
    ei_check(print_one(s, &state, type, dest, false, err));

    /* next_index might get updated as we're printing out each
     * interface, if we encounter another interface that we've haven't
     * seen before. */
    for (i = 0; i < state.next_index; i++) {
        ei_check(print_interface(s, &state, i, givens, err));
    }

    cork_hash_table_done(swan_alloc(s), &state.interface_indices);
    cork_hash_table_done(swan_alloc(s), &state.interfaces);
    return 0;

error:
    cork_hash_table_done(swan_alloc(s), &state.interface_indices);
    cork_hash_table_done(swan_alloc(s), &state.interfaces);
    return -1;
}
