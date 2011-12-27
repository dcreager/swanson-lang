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


static void
s0_value_free(struct cork_gc *gc, void *vself)
{
    struct s0_value  *self = vself;
    switch (self->kind) {
        case S0_VALUE_LITERAL:
            cork_strfree(gc->alloc, self->_.literal);
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


static struct s0_type *
s0_macro_value_create_type(struct swan *s, struct s0_value *value,
                           struct cork_error *err)
{
    size_t  i;
    struct s0_type  *type;
    rpp_check(type = s0_function_type_new(s, err));
    for (i = 0; i < cork_array_size(&value->_.macro->params); i++) {
        ei_check(s0_function_type_add_param
                 (s, type, cork_array_at(&value->_.macro->params, i), err));
    }
    for (i = 0; i < cork_array_size(&value->_.macro->results); i++) {
        ei_check(s0_function_type_add_result
                 (s, type, cork_array_at(&value->_.macro->results, i), err));
    }
    return type;

error:
    cork_gc_decref(swan_gc(s), type);
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
                rpp_check(value->type =
                          s0_macro_value_create_type(s, value, err));
                break;

            default:
                cork_unknown_error_set(swan_alloc(s), err);
                return NULL;
        }
    }

    return value->type;
}
