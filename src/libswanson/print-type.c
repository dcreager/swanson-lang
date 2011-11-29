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
#include <libcork/ds.h>

#include "swanson/checkers.h"
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
          bool parenthize);

static int
assign_id(struct swan *s, struct s0_printer *state,
          struct s0_interface_type *itype, size_t *index)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry = NULL;
    r_pcheck(entry = cork_hash_table_get_or_create
             (&state->interface_indices, itype, &is_new));

    if (is_new) {
        *index = state->next_index++;
        entry->value = (void *) (intptr_t) *index;
        cork_hash_table_put(&state->interfaces,
                            entry->value, itype, NULL, NULL);
    } else {
        *index = (size_t) (intptr_t) entry->value;
    }
    return 0;
}

static int
print_interface_name(struct swan *s, size_t index, struct cork_buffer *dest)
{
    if (index < TYPE_NAME_COUNT) {
        r_bcheck(cork_buffer_append_string(dest, TYPE_NAMES[index]));
        return 0;
    } else {
        r_bcheck(cork_buffer_append_printf
                 (dest, "i%zu", index - TYPE_NAME_COUNT));
        return 0;
    }
}

static int
print_type_list(struct swan *s, struct s0_printer *state,
                struct s0_type_list *list, struct cork_buffer *dest)
{

    if (list == NULL) {
        r_bcheck(cork_buffer_append_string(dest, "void"));
    } else {
        bool  first = true;
        struct s0_type_list  *curr;
        for (curr = list; curr != NULL; curr = curr->tail) {
            if (first) {
                first = false;
            } else {
                r_bcheck(cork_buffer_append_string(dest, ","));
            }
            r_check(print_one(s, state, curr->head, dest, true));
        }
    }
    return 0;
}

static int
print_one(struct swan *s, struct s0_printer *state,
          struct s0_type *type, struct cork_buffer *dest,
          bool parenthize)
{
    switch (type->kind) {
        case S0_KIND_LITERAL:
            r_bcheck(cork_buffer_append_string(dest, "LITERAL"));
            return 0;

        case S0_KIND_FUNCTION:
        {
            struct s0_function_type  *ftype =
                cork_container_of(type, struct s0_function_type, parent);

            if (parenthize) {
                r_bcheck(cork_buffer_append_string(dest, "("));
            }
            r_check(print_type_list(s, state, ftype->params, dest));
            r_bcheck(cork_buffer_append_string(dest, " -> "));
            r_check(print_type_list(s, state, ftype->results, dest));
            if (parenthize) {
                r_bcheck(cork_buffer_append_string(dest, ")"));
            }
            return 0;
        }

        case S0_KIND_LOCATION:
        {
            struct s0_location_type  *ltype =
                cork_container_of(type, struct s0_location_type, parent);
            r_bcheck(cork_buffer_append_string(dest, "*"));
            return print_one(s, state, ltype->referent, dest, true);
        }

        case S0_KIND_INTERFACE:
        {
            struct s0_interface_type  *itype =
                cork_container_of(type, struct s0_interface_type, parent);
            size_t  index;
            r_check(assign_id(s, state, itype, &index));
            return print_interface_name(s, index, dest);
        }

        case S0_KIND_RECURSIVE:
        {
            struct s0_recursive_type  *rtype =
                cork_container_of(type, struct s0_recursive_type, parent);
            return print_one(s, state, rtype->resolved, dest, parenthize);
        }

        default:
            return -1;
    }
}

static int
print_interface(struct swan *s, struct s0_printer *state,
                size_t index, struct cork_buffer *dest)
{
    struct s0_interface_type  *itype = NULL;
    struct s0_interface_entry  *curr;
    r_pcheck(itype = cork_hash_table_get
             (&state->interfaces, (void *) (intptr_t) index));

    if (index > 0) {
        r_bcheck(cork_buffer_append_string(dest, "\n"));
    }

    r_bcheck(cork_buffer_append_string(dest, "interface "));
    r_check(print_interface_name(s, index, dest));
    r_bcheck(cork_buffer_append_string(dest, " {\n"));
    for (curr = itype->entries; curr != NULL; curr = curr->tail) {
        r_bcheck(cork_buffer_append_printf(dest, "  %s ", curr->name));
        r_check(print_one(s, state, curr->type, dest, true));
        r_bcheck(cork_buffer_append_string(dest, "\n"));
    }
    r_bcheck(cork_buffer_append_string(dest, "}\n"));

    return 0;
}


int
s0_type_print(struct swan *s, struct s0_type *type,
              struct cork_buffer *dest, struct cork_buffer *givens,
              struct cork_error *err)
{
    size_t  i;
    struct s0_printer  state;
    cork_hash_table_init(swan_alloc(s), &state.interface_indices, 0,
                         constant_hasher, constant_comparator);
    cork_hash_table_init(swan_alloc(s), &state.interfaces, 0,
                         constant_hasher, constant_comparator);
    state.next_index = 0;

    /* Print the type into dest.  If type contains references to any
     * interfaces, we'll assign IDs to them as part of printing the
     * overall type.  This will give us references to all interface
     * types in the state.interfaces hash table. */
    e_check(print_one(s, &state, type, dest, false));

    /* next_index might get updated as we're printing out each
     * interface, if we encounter another interface that we've haven't
     * seen before. */
    for (i = 0; i < state.next_index; i++) {
        e_check(print_interface(s, &state, i, givens));
    }

    cork_hash_table_done(&state.interface_indices);
    cork_hash_table_done(&state.interfaces);
    return 0;

error:
    cork_hash_table_done(&state.interface_indices);
    cork_hash_table_done(&state.interfaces);
    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_UNKNOWN,
                   "Cannot pretty-print type");
    return -1;
}
