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

#include "swanson/s0.h"
#include "swanson/state.h"


/*-----------------------------------------------------------------------
 * Error handling
 */

static int
s0_bad_tuple_index(struct cork_alloc *alloc,
                   struct cork_error *err,
                   struct cork_buffer *dest)
{
    size_t  *index = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL, "Invalid tuple index: %zu", *index);
}

static int
s0_bad_tuple_index_set(struct cork_alloc *alloc, struct cork_error *err,
                       size_t index)
{
    return cork_error_set_extra(alloc, err,
                                S0_ERROR,
                                S0_REDEFINED,
                                s0_bad_tuple_index,
                                index);
}


/*-----------------------------------------------------------------------
 * Values
 */

static void
s0_value_free(struct cork_gc *gc, void *vself)
{
    struct s0_value  *self = vself;
    switch (self->kind) {
        case S0_VALUE_LITERAL:
            cork_strfree(gc->alloc, self->_.literal);
            break;

        case S0_VALUE_TUPLE:
            cork_array_done(gc->alloc, &self->_.tuple);
            break;

        default:
            break;
    }
}

static void
s0_value_recurse(struct cork_gc *gc, void *vself,
                      cork_gc_recurser recurse, void *ud)
{
    struct s0_value  *self = vself;
    switch (self->kind) {
        case S0_VALUE_TYPE:
            recurse(gc, self->_.type, ud);
            break;

        case S0_VALUE_MACRO:
            recurse(gc, self->_.macro, ud);
            break;

        case S0_VALUE_TUPLE:
        {
            size_t  i;
            for (i = 0; i < cork_array_size(&self->_.tuple); i++) {
                recurse(gc, cork_array_at(&self->_.tuple, i), ud);
            }
            break;
        }

        default:
            break;
    }
    recurse(gc, self->type, ud);
}

static struct cork_gc_obj_iface  s0_value_gc = {
    s0_value_free, s0_value_recurse
};


struct s0_value *
s0_type_value_new(struct swan *s, struct s0_type *type,
                  struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_value  *self = NULL;
    rp_check_gc_new(s0_value, self, "type value");
    self->kind = S0_VALUE_TYPE;
    self->type = NULL;
    self->_.type = cork_gc_incref(swan_gc(s), type);
    return self;
}


struct s0_value *
s0_literal_value_new(struct swan *s, const char *contents,
                     struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_value  *self = NULL;
    rp_check_gc_new(s0_value, self, "type value");
    self->kind = S0_VALUE_LITERAL;
    self->type = NULL;
    e_check_alloc(self->_.literal = cork_strdup(swan_alloc(s), contents),
                  "literal contents");
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}


struct s0_value *
s0_macro_value_new(struct swan *s, struct s0_basic_block *macro,
                   struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_value  *self = NULL;
    rp_check_gc_new(s0_value, self, "macro value");
    self->kind = S0_VALUE_MACRO;
    self->type = NULL;
    self->_.macro = cork_gc_incref(swan_gc(s), macro);
    return self;
}


struct s0_value *
s0_tuple_value_new(struct swan *s, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_value  *self = NULL;
    rp_check_gc_new(s0_value, self, "macro value");
    self->kind = S0_VALUE_TUPLE;
    self->type = NULL;
    cork_array_init(swan_alloc(s), &self->_.tuple);
    return self;
}

int
s0_tuple_value_add(struct swan *s, struct s0_value *self,
                   struct s0_value *value, struct cork_error *err)
{
    if (self->kind != S0_VALUE_TUPLE) {
        swan_general_bad_type_set
            (swan_alloc(s), err, "Can only add elements to tuples");
        return -1;
    }

    return cork_array_append
        (swan_alloc(s), &self->_.tuple,
         cork_gc_incref(swan_gc(s), value), err);
}

struct s0_value *
s0_tuple_value_get(struct swan *s, struct s0_value *self,
                   size_t index, struct cork_error *err)
{
    if (self->kind != S0_VALUE_TUPLE) {
        swan_general_bad_type_set
            (swan_alloc(s), err, "Can only get elements from tuples");
        return NULL;
    }

    if (index >= cork_array_size(&self->_.tuple)) {
        s0_bad_tuple_index_set(swan_alloc(s), err, index);
        return NULL;
    }

    return cork_array_at(&self->_.tuple, index);
}


static struct s0_type *
s0_tuple_value_create_type(struct swan *s, struct s0_value *value,
                           struct cork_error *err)
{
    size_t  i;
    struct s0_type  *result;
    rpp_check(result = s0_product_type_new(s, err));
    for (i = 0; i < cork_array_size(&value->_.tuple); i++) {
        struct s0_type  *element_type;
        ep_check(element_type = s0_value_get_type
                 (s, cork_array_at(&value->_.tuple, i), err));
        ei_check(s0_product_type_add(s, result, element_type, err));
    }
    return result;

error:
    cork_gc_decref(swan_gc(s), result);
    return NULL;
}

struct s0_type *
s0_value_get_type(struct swan *s, struct s0_value *value,
                  struct cork_error *err)
{
    if (value->type == NULL) {
        switch (value->kind) {
            case S0_VALUE_TYPE:
                rpp_check(value->type = s0_type_type_new(s, err));
                break;

            case S0_VALUE_LITERAL:
                rpp_check(value->type = s0_literal_type_new(s, err));
                break;

            case S0_VALUE_MACRO:
                rpp_check(value->type = s0_function_type_new
                          (s, value->_.macro->input,
                           value->_.macro->output, err));
                break;

            case S0_VALUE_TUPLE:
                rpp_check(value->type = s0_tuple_value_create_type
                          (s, value, err));
                break;

            default:
                cork_unknown_error_set(swan_alloc(s), err);
                return NULL;
        }
    }

    return value->type;
}
