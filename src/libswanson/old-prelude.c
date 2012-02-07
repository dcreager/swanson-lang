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

#include "swanson/prelude.h"
#include "swanson/s0.h"
#include "swanson/state.h"


/*-----------------------------------------------------------------------
 * Prelude wrapper object
 */

static void
swan_prelude_wrapper_recurse(struct cork_gc *gc, void *vself,
                             cork_gc_recurser recurse, void *ud)
{
    struct swan_old_prelude  *self = vself;
    recurse(gc, self->prelude, ud);
    recurse(gc, self->t_bool, ud);
    recurse(gc, self->v_false, ud);
    recurse(gc, self->v_true, ud);
}

static struct cork_gc_obj_iface  swan_prelude_wrapper_gc = {
    NULL, swan_prelude_wrapper_recurse
};

static struct swan_old_prelude *
swan_old_prelude_wrapper_new(struct swan *s)
{
    struct swan_old_prelude  *self =
        cork_gc_new_iface(struct swan_old_prelude, &swan_prelude_wrapper_gc);
    memset(self, 0, sizeof(struct swan_old_prelude));
    return self;
}


/*-----------------------------------------------------------------------
 * Default GC methods
 */

static void
swan_prelude_obj_recurse(struct cork_gc *gc, void *vself,
                         cork_gc_recurser recurse, void *ud)
{
    struct s0_object  *self = vself;
    recurse(gc, self->type, ud);
}

static struct cork_gc_obj_iface  swan_prelude_obj_gc = {
    NULL, swan_prelude_obj_recurse
};


/*-----------------------------------------------------------------------
 * bool type
 */

struct swan_old_bool {
    struct s0_object  parent;
    bool  value;
};

/* Only use if you've verified that value is a bool OBJECT */
#define swan_bool_from_value(value) \
    (cork_container_of((value)->_.obj, struct swan_old_bool, parent))

/* Creates new reference to type */
static struct s0_value *
swan_old_bool_new(struct swan *s, struct s0_type *type,
                  bool value)
{
    struct swan_old_bool  *self =
        cork_gc_new_iface(struct swan_old_bool, &swan_prelude_obj_gc);
    self->parent.type = cork_gc_incref(type);
    self->value = value;
    return s0_object_value_new(s, &self->parent);
}

#define swan_bool_bin_op(name, op) \
static struct s0_value * \
swan_bool_##name(struct swan *s, struct s0_c_function *self, \
                 struct s0_value *input) \
{ \
    struct swan_old_bool  *lhs = \
        swan_bool_from_value(s0_tuple_value_get_fast(input, 0)); \
    struct swan_old_bool  *rhs = \
        swan_bool_from_value(s0_tuple_value_get_fast(input, 1)); \
    return swan_old_bool_new \
        (s, lhs->parent.type, lhs->value op rhs->value); \
}

swan_bool_bin_op(and, &&);
swan_bool_bin_op(or,  ||);
swan_bool_bin_op(xor, ^);

static int
swan_prelude_bool(struct swan *s, struct swan_old_prelude *prelude)
{
    struct s0_type  *input = NULL;
    struct s0_type  *output = NULL;
    struct s0_value  *value = NULL;
    struct s0_c_function  *func;

    rip_check(prelude->t_bool = s0_class_type_new(s, "bool"));

#define swan_bool_add_bin_op(name, op) \
    do { \
        ep_check(input = s0_product_type_new(s)); \
        ei_check(s0_product_type_add(s, input, prelude->t_bool)); \
        ei_check(s0_product_type_add(s, input, prelude->t_bool)); \
        ep_check(output = cork_gc_incref(prelude->t_bool)); \
        ep_check(func = s0_c_function_new \
                 (s, #op, swan_bool_##name, input, output)); \
        input = output = NULL; \
        ep_check(value = s0_c_value_new(s, func)); \
        func = NULL; \
        ei_check(s0_class_type_add(s, prelude->t_bool, #op, value)); \
        cork_gc_decref(value); \
        value = NULL; \
    } while (0)

    swan_bool_add_bin_op(and, &&);
    swan_bool_add_bin_op(or,  ||);
    swan_bool_add_bin_op(xor, ^);

    ep_check(prelude->v_false = swan_old_bool_new
             (s, prelude->t_bool, false));
    ei_check(s0_class_type_add
             (s, prelude->prelude, "false", prelude->v_false));

    ep_check(prelude->v_true = swan_old_bool_new
             (s, prelude->t_bool, true));
    ei_check(s0_class_type_add
             (s, prelude->prelude, "true", prelude->v_true));

    ep_check(value = s0_type_value_new(s, prelude->t_bool));
    ei_check(s0_class_type_add(s, prelude->prelude, "bool", value));
    cork_gc_decref(value);
    return 0;

error:
    cork_gc_decref(input);
    cork_gc_decref(output);
    cork_gc_decref(value);
    return -1;
}


/*-----------------------------------------------------------------------
 * Control structures
 */

static struct s0_value *
swan_if(struct swan *s, struct s0_c_function *self,
        struct s0_value *input)
{
    struct swan_old_bool  *cond =
        swan_bool_from_value(s0_tuple_value_get_fast(input, 0));
    struct s0_value  *true_branch = s0_tuple_value_get_fast(input, 1);
    struct s0_value  *false_branch = s0_tuple_value_get_fast(input, 2);

    if (cond->value) {
        return s0_value_evaluate(s, true_branch, NULL);
    } else {
        return s0_value_evaluate(s, false_branch, NULL);
    }
}

static int
swan_prelude_control(struct swan *s, struct swan_old_prelude *prelude)
{
    struct s0_value  *value = NULL;
    struct s0_c_function  *func;

    ep_check(func = s0_c_function_new(s, "if", swan_if, NULL, NULL));
    ep_check(value = s0_c_value_new(s, func));
    ei_check(s0_class_type_add(s, prelude->prelude, "if", value));
    cork_gc_decref(value);
    return 0;

error:
    cork_gc_decref(value);
    return -1;
}


/*-----------------------------------------------------------------------
 * The prelude
 */

struct s0_value *
swan_old_prelude_new(struct swan *s)
{
    struct s0_value  *value;

    rpp_check(s->prelude = swan_old_prelude_wrapper_new(s));
    ep_check(s->prelude->prelude = s0_class_type_new(s, "<prelude>"));
    ei_check(swan_prelude_bool(s, s->prelude));
    ei_check(swan_prelude_control(s, s->prelude));

    ep_check(value = s0_type_value_new(s, s->prelude->prelude));
    return value;

error:
    cork_gc_decref(s->prelude);
    s->prelude = NULL;
    return NULL;
}
