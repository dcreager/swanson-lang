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
#include <libcork/helpers/errors.h>
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

static void
print_one(struct swan *s, struct s0_printer *state,
          struct s0_type *type, struct cork_buffer *dest,
          bool parenthize);

static void
assign_id(struct swan *s, struct s0_printer *state,
          struct s0_type *type, size_t *index)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry = NULL;
    entry = cork_hash_table_get_or_create
        (&state->interface_indices, type, &is_new);

    if (is_new) {
        *index = state->next_index++;
        entry->value = (void *) (intptr_t) *index;
        cork_hash_table_put
            (&state->interfaces, entry->value, type, NULL, NULL, NULL);
    } else {
        *index = (size_t) (intptr_t) entry->value;
    }
}

static void
print_interface_name(struct swan *s, size_t index, struct cork_buffer *dest)
{
    if (index < TYPE_NAME_COUNT) {
        cork_buffer_append_string(dest, TYPE_NAMES[index]);
    } else {
        cork_buffer_append_printf(dest, "i%zu", index - TYPE_NAME_COUNT);
    }
}

static void
print_type_list(struct swan *s, struct s0_printer *state,
                s0_type_array *array, struct cork_buffer *dest,
                bool parenthize)
{

    if (cork_array_is_empty(array)) {
        cork_buffer_append_string(dest, "void");
    } else {
        size_t  i;
        if (parenthize) {
            cork_buffer_append_string(dest, "(");
        }
        for (i = 0; i < cork_array_size(array); i++) {
            if (i > 0) {
                cork_buffer_append_string(dest, ",");
            }
            print_one(s, state, cork_array_at(array, i), dest, true);
        }
        if (parenthize) {
            cork_buffer_append_string(dest, ")");
        }
    }
}

static void
print_one(struct swan *s, struct s0_printer *state,
          struct s0_type *type, struct cork_buffer *dest,
          bool parenthize)
{
    if (type == NULL) {
        cork_buffer_append_string(dest, "void");
        return;
    }

    switch (type->kind) {
        case S0_TYPE_TYPE:
            cork_buffer_append_string(dest, "TYPE");
            return;

        case S0_TYPE_LITERAL:
            cork_buffer_append_string(dest, "LITERAL");
            return;

        case S0_TYPE_ANY:
            cork_buffer_append_string(dest, "*");
            return;

        case S0_TYPE_PRODUCT:
            print_type_list(s, state, &type->_.product, dest, parenthize);
            return;

        case S0_TYPE_FUNCTION:
        {
            if (parenthize) {
                cork_buffer_append_string(dest, "(");
            }
            print_one(s, state, type->_.function.input, dest, false);
            cork_buffer_append_string(dest, " -> ");
            print_one(s, state, type->_.function.output, dest, false);
            if (parenthize) {
                cork_buffer_append_string(dest, ")");
            }
            return;
        }

        case S0_TYPE_LOCATION:
        {
            cork_buffer_append_string(dest, "*");
            print_one(s, state, type->_.location.referent, dest, true);
            return;
        }

        case S0_TYPE_INTERFACE:
        {
            size_t  index;
            assign_id(s, state, type, &index);
            print_interface_name(s, index, dest);
            return;
        }

        case S0_TYPE_CLASS:
            cork_buffer_append_string(dest, type->_.cls.name);
            return;

        case S0_TYPE_BLOCK:
        {
            cork_buffer_append_string(dest, "{");
            print_one(s, state, type->_.block.result, dest, false);
            cork_buffer_append_string(dest, "}");
            return;
        }

        case S0_TYPE_RECURSIVE:
        {
            print_one(s, state, type->_.recursive.resolved, dest, parenthize);
            return;
        }

        default:
            cork_abort("%s", "Unknown kind of type");
    }
}

static void
print_interface(struct swan *s, struct s0_printer *state,
                size_t index, struct cork_buffer *dest)
{
    struct s0_type  *type;
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;
    type = cork_hash_table_get(&state->interfaces, (void *) (intptr_t) index);

    if (index > 0) {
        cork_buffer_append_string(dest, "\n");
    }

    cork_buffer_append_string(dest, "interface ");
    print_interface_name(s, index, dest);
    cork_buffer_append_string(dest, " {\n");
    cork_hash_table_iterator_init(&type->_.interface.entries, &iter);
    while ((entry = cork_hash_table_iterator_next(&iter)) != NULL) {
        const char  *name = entry->key;
        cork_buffer_append_printf(dest, "  %s ", name);
        print_one(s, state, entry->value, dest, true);
        cork_buffer_append_string(dest, "\n");
    }
    cork_buffer_append_string(dest, "}\n");
}


int
s0_type_print(struct swan *s, struct s0_type *type,
              struct cork_buffer *dest, struct cork_buffer *givens)
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
    print_one(s, &state, type, dest, false);

    /* next_index might get updated as we're printing out each
     * interface, if we encounter another interface that we've haven't
     * seen before. */
    for (i = 0; i < state.next_index; i++) {
        print_interface(s, &state, i, givens);
    }

    cork_hash_table_done(&state.interface_indices);
    cork_hash_table_done(&state.interfaces);
    return 0;
}


int
s0_type_print_many(struct swan *s, s0_type_array *types,
                   s0_buffer_array *dests, struct cork_buffer *givens)
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

        dest = cork_buffer_new();
        cork_array_append(dests, dest);
        print_one(s, &state, type, dest, false);
    }

    /* next_index might get updated as we're printing out each
     * interface, if we encounter another interface that we've haven't
     * seen before. */
    for (i = 0; i < state.next_index; i++) {
        print_interface(s, &state, i, givens);
    }

    cork_hash_table_done(&state.interface_indices);
    cork_hash_table_done(&state.interfaces);
    return 0;
}
