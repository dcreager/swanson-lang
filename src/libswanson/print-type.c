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
          struct s0_type *type, size_t *index,
          struct cork_error *err)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry = NULL;
    rip_check(entry = cork_hash_table_get_or_create
              (&state->interface_indices, type, &is_new, err));

    if (is_new) {
        *index = state->next_index++;
        entry->value = (void *) (intptr_t) *index;
        rii_check(cork_hash_table_put
                  (&state->interfaces,
                   entry->value, type, NULL, NULL, NULL, err));
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
        return cork_buffer_append_string(dest, TYPE_NAMES[index], err);
    } else {
        return cork_buffer_append_printf
            (dest, err, "i%zu", index - TYPE_NAME_COUNT);
    }
}

static int
print_type_list(struct swan *s, struct s0_printer *state,
                s0_type_array *array, struct cork_buffer *dest,
                bool parenthize, struct cork_error *err)
{

    if (cork_array_is_empty(array)) {
        return cork_buffer_append_string(dest, "void", err);
    } else {
        size_t  i;
        if (parenthize) {
            rii_check(cork_buffer_append_string(dest, "(", err));
        }
        for (i = 0; i < cork_array_size(array); i++) {
            if (i > 0) {
                rii_check(cork_buffer_append_string(dest, ",", err));
            }
            rii_check(print_one
                      (s, state, cork_array_at(array, i), dest, true, err));
        }
        if (parenthize) {
            rii_check(cork_buffer_append_string(dest, ")", err));
        }
    }
    return 0;
}

static int
print_one(struct swan *s, struct s0_printer *state,
          struct s0_type *type, struct cork_buffer *dest,
          bool parenthize, struct cork_error *err)
{
    if (type == NULL) {
        return cork_buffer_append_string(dest, "void", err);
    }

    switch (type->kind) {
        case S0_TYPE_TYPE:
            return cork_buffer_append_string(dest, "TYPE", err);

        case S0_TYPE_LITERAL:
            return cork_buffer_append_string(dest, "LITERAL", err);

        case S0_TYPE_ANY:
            return cork_buffer_append_string(dest, "*", err);

        case S0_TYPE_PRODUCT:
        {
            rii_check(print_type_list
                      (s, state, &type->_.product, dest, parenthize, err));
            return 0;
        }

        case S0_TYPE_FUNCTION:
        {
            if (parenthize) {
                rii_check(cork_buffer_append_string(dest, "(", err));
            }
            rii_check(print_one
                      (s, state, type->_.function.input, dest, false, err));
            rii_check(cork_buffer_append_string(dest, " -> ", err));
            rii_check(print_one
                      (s, state, type->_.function.output, dest, false, err));
            if (parenthize) {
                rii_check(cork_buffer_append_string(dest, ")", err));
            }
            return 0;
        }

        case S0_TYPE_LOCATION:
        {
            rii_check(cork_buffer_append_string(dest, "*", err));
            return print_one
                (s, state, type->_.location.referent, dest, true, err);
        }

        case S0_TYPE_INTERFACE:
        {
            size_t  index;
            rii_check(assign_id(s, state, type, &index, err));
            return print_interface_name(s, index, dest, err);
        }

        case S0_TYPE_CLASS:
            return cork_buffer_append_string(dest, type->_.cls.name, err);

        case S0_TYPE_BLOCK:
        {
            rii_check(cork_buffer_append_string(dest, "{", err));
            rii_check(print_one
                      (s, state, type->_.block.result, dest, false, err));
            return cork_buffer_append_string(dest, "}", err);
        }

        case S0_TYPE_RECURSIVE:
        {
            return print_one
                (s, state, type->_.recursive.resolved, dest, parenthize, err);
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
    struct s0_type  *type;
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;
    rip_check(type = cork_hash_table_get
              (&state->interfaces, (void *) (intptr_t) index));

    if (index > 0) {
        rii_check(cork_buffer_append_string(dest, "\n", err));
    }

    rii_check(cork_buffer_append_string(dest, "interface ", err));
    rii_check(print_interface_name(s, index, dest, err));
    rii_check(cork_buffer_append_string(dest, " {\n", err));
    cork_hash_table_iterator_init(&type->_.interface.entries, &iter);
    while ((entry = cork_hash_table_iterator_next(&iter)) != NULL) {
        const char  *name = entry->key;
        rii_check(cork_buffer_append_printf(dest, err, "  %s ", name));
        rii_check(print_one(s, state, entry->value, dest, true, err));
        rii_check(cork_buffer_append_string(dest, "\n", err));
    }
    rii_check(cork_buffer_append_string(dest, "}\n", err));

    return 0;
}


int
s0_type_print(struct swan *s, struct s0_type *type,
              struct cork_buffer *dest, struct cork_buffer *givens,
              struct cork_error *err)
{
    size_t  i;
    struct s0_printer  state;
    cork_hash_table_init
        (&state.interface_indices, 0, constant_hasher, constant_comparator);
    cork_hash_table_init
        (&state.interfaces, 0, constant_hasher, constant_comparator);
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

    cork_hash_table_done(&state.interface_indices);
    cork_hash_table_done(&state.interfaces);
    return 0;

error:
    cork_hash_table_done(&state.interface_indices);
    cork_hash_table_done(&state.interfaces);
    return -1;
}


int
s0_type_print_many(struct swan *s, s0_type_array *types,
                   s0_buffer_array *dests, struct cork_buffer *givens,
                   struct cork_error *err)
{
    size_t  i;
    struct s0_printer  state;
    cork_hash_table_init
        (&state.interface_indices, 0, constant_hasher, constant_comparator);
    cork_hash_table_init
        (&state.interfaces, 0, constant_hasher, constant_comparator);
    state.next_index = 0;

    /* Print the type into dest.  If type contains references to any
     * interfaces, we'll assign IDs to them as part of printing the
     * overall type.  This will give us references to all interface
     * types in the state.interfaces hash table. */
    for (i = 0; i < cork_array_size(types); i++) {
        struct s0_type  *type = cork_array_at(types, i);
        struct cork_buffer  *dest;

        ep_check(dest = cork_buffer_new(err));
        ei_check(cork_array_append(dests, dest, err));
        ei_check(print_one(s, &state, type, dest, false, err));
    }

    /* next_index might get updated as we're printing out each
     * interface, if we encounter another interface that we've haven't
     * seen before. */
    for (i = 0; i < state.next_index; i++) {
        ei_check(print_interface(s, &state, i, givens, err));
    }

    cork_hash_table_done(&state.interface_indices);
    cork_hash_table_done(&state.interfaces);
    return 0;

error:
    cork_hash_table_done(&state.interface_indices);
    cork_hash_table_done(&state.interfaces);
    return -1;
}
