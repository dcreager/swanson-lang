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
 * Values
 */

static const char *
s0_value_kind_name(enum s0_value_kind kind)
{
    switch (kind) {
        case S0_VALUE_TYPE:
            return "type";
        case S0_VALUE_LITERAL:
            return "literal";
        case S0_VALUE_MACRO:
            return "macro";
        case S0_VALUE_TUPLE:
            return "tuple";
        case S0_VALUE_C:
            return "C function";
        case S0_VALUE_OBJECT:
            return "object";
        default:
            return "<unknown>";
    }
}

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

        case S0_VALUE_OBJECT:
            recurse(gc, self->_.obj, ud);
            break;

        case S0_VALUE_C:
            recurse(gc, self->_.c, ud);
            break;

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
        cork_error_set
            (swan_alloc(s), err, SWAN_GENERAL_ERROR, SWAN_GENERAL_BAD_TYPE,
             "Can only add elements to tuples");
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
        cork_error_set
            (swan_alloc(s), err, SWAN_GENERAL_ERROR, SWAN_GENERAL_BAD_TYPE,
             "Can only get elements from tuples");
        return NULL;
    }

    if (index >= cork_array_size(&self->_.tuple)) {
        cork_error_set
            (swan_alloc(s), err, S0_ERROR, S0_UNDEFINED,
             "Invalid tuple index: %zu", index);
        return NULL;
    }

    return cork_array_at(&self->_.tuple, index);
}


struct s0_value *
s0_c_value_new(struct swan *s, struct s0_c_function *func,
               struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_value  *self = NULL;
    e_check_gc_new(s0_value, self, "C function value");
    self->kind = S0_VALUE_C;
    self->type = NULL;
    self->_.c = func;
    return self;

error:
    cork_gc_decref(swan_gc(s), func);
    return NULL;
}


struct s0_value *
s0_object_value_new(struct swan *s, struct s0_object *obj,
                    struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_value  *self = NULL;
    e_check_gc_new(s0_value, self, "object value");
    self->kind = S0_VALUE_OBJECT;
    self->type = NULL;
    self->_.obj = obj;
    return self;

error:
    cork_gc_decref(swan_gc(s), obj);
    return NULL;
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

            case S0_VALUE_C:
                rpp_check(value->type = s0_function_type_new
                          (s, value->_.c->input,
                           value->_.c->output, err));
                break;

            case S0_VALUE_OBJECT:
                return value->_.obj->type;

            default:
                cork_unknown_error_set(swan_alloc(s), err);
                return NULL;
        }
    }

    return value->type;
}


struct s0_value *
s0_value_evaluate(struct swan *s, struct s0_value *value,
                  struct s0_value *input, struct cork_error *err)
{
    switch (value->kind) {
        case S0_VALUE_MACRO:
            return s0_basic_block_evaluate(s, value->_.macro, input, err);

        case S0_VALUE_C:
            return s0_c_function_call(s, value->_.c, input, err);

        default:
            cork_error_set
                (swan_alloc(s), err, S0_ERROR, S0_EVALUATION_ERROR,
                 "Cannot evaluate a %s", s0_value_kind_name(value->kind));
            return NULL;
    }
}
