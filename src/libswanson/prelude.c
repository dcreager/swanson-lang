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
    struct swan_prelude  *self = vself;
    recurse(gc, self->prelude, ud);
    recurse(gc, self->t_bool, ud);
    recurse(gc, self->v_false, ud);
    recurse(gc, self->v_true, ud);
}

static struct cork_gc_obj_iface  swan_prelude_wrapper_gc = {
    NULL, swan_prelude_wrapper_recurse
};

static struct swan_prelude *
swan_prelude_wrapper_new(struct swan *s, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct swan_prelude  *self;
    rp_check_gc_inew(swan_prelude, &swan_prelude_wrapper_gc, self, "prelude");
    memset(self, 0, sizeof(struct swan_prelude));
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

struct swan_bool {
    struct s0_object  parent;
    bool  value;
};

/* Only use if you've verified that value is a bool OBJECT */
#define swan_bool_from_value(value) \
    (cork_container_of((value)->_.obj, struct swan_bool, parent))

/* Creates new reference to type */
static struct s0_value *
swan_bool_new(struct swan *s, struct s0_type *type,
              bool value, struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct swan_bool  *self = NULL;
    rp_check_gc_inew(swan_bool, &swan_prelude_obj_gc, self, "bool object");
    self->parent.type = cork_gc_incref(swan_gc(s), type);
    self->value = value;
    return s0_object_value_new(s, &self->parent, err);
}

#define swan_bool_bin_op(name, op) \
static struct s0_value * \
swan_bool_##name(struct swan *s, struct s0_c_function *self, \
                 struct s0_value *input, struct cork_error *err) \
{ \
    struct swan_bool  *lhs = \
        swan_bool_from_value(s0_tuple_value_get_fast(input, 0)); \
    struct swan_bool  *rhs = \
        swan_bool_from_value(s0_tuple_value_get_fast(input, 1)); \
    return swan_bool_new \
        (s, lhs->parent.type, lhs->value op rhs->value, err); \
}

swan_bool_bin_op(and, &&);
swan_bool_bin_op(or,  ||);
swan_bool_bin_op(xor, ^);

static int
swan_prelude_bool(struct swan *s, struct swan_prelude *prelude,
                  struct cork_error *err)
{
    struct s0_type  *input = NULL;
    struct s0_type  *output = NULL;
    struct s0_value  *value = NULL;
    struct s0_c_function  *func;

    rip_check(prelude->t_bool = s0_class_type_new(s, "bool", err));

#define swan_bool_add_bin_op(name, op) \
    do { \
        ep_check(input = s0_product_type_new(s, err)); \
        ei_check(s0_product_type_add(s, input, prelude->t_bool, err)); \
        ei_check(s0_product_type_add(s, input, prelude->t_bool, err)); \
        ep_check(output = cork_gc_incref(swan_gc(s), prelude->t_bool)); \
        ep_check(func = s0_c_function_new \
                 (s, #op, swan_bool_##name, input, output, err)); \
        input = output = NULL; \
        ep_check(value = s0_c_value_new(s, func, err)); \
        func = NULL; \
        ei_check(s0_class_type_add(s, prelude->t_bool, #op, value, err)); \
        cork_gc_decref(swan_gc(s), value); \
        value = NULL; \
    } while (0)

    swan_bool_add_bin_op(and, &&);
    swan_bool_add_bin_op(or,  ||);
    swan_bool_add_bin_op(xor, ^);

    ep_check(prelude->v_false = swan_bool_new
             (s, prelude->t_bool, false, err));
    ei_check(s0_class_type_add
             (s, prelude->prelude, "false", prelude->v_false, err));

    ep_check(prelude->v_true = swan_bool_new
             (s, prelude->t_bool, true, err));
    ei_check(s0_class_type_add
             (s, prelude->prelude, "true", prelude->v_true, err));

    ep_check(value = s0_type_value_new(s, prelude->t_bool, err));
    ei_check(s0_class_type_add(s, prelude->prelude, "bool", value, err));
    cork_gc_decref(swan_gc(s), value);
    return 0;

error:
    cork_gc_decref(swan_gc(s), input);
    cork_gc_decref(swan_gc(s), output);
    cork_gc_decref(swan_gc(s), value);
    return -1;
}


/*-----------------------------------------------------------------------
 * Control structures
 */

static struct s0_value *
swan_if(struct swan *s, struct s0_c_function *self,
        struct s0_value *input, struct cork_error *err)
{
    struct swan_bool  *cond =
        swan_bool_from_value(s0_tuple_value_get_fast(input, 0));
    struct s0_value  *true_branch = s0_tuple_value_get_fast(input, 1);
    struct s0_value  *false_branch = s0_tuple_value_get_fast(input, 2);

    if (cond->value) {
        return s0_value_evaluate(s, true_branch, NULL, err);
    } else {
        return s0_value_evaluate(s, false_branch, NULL, err);
    }
}

static int
swan_prelude_control(struct swan *s, struct swan_prelude *prelude,
                     struct cork_error *err)
{
    struct s0_value  *value = NULL;
    struct s0_c_function  *func;

    ep_check(func = s0_c_function_new(s, "if", swan_if, NULL, NULL, err));
    ep_check(value = s0_c_value_new(s, func, err));
    ei_check(s0_class_type_add(s, prelude->prelude, "if", value, err));
    cork_gc_decref(swan_gc(s), value);
    return 0;

error:
    cork_gc_decref(swan_gc(s), value);
    return -1;
}


/*-----------------------------------------------------------------------
 * The prelude
 */

struct s0_value *
swan_prelude_new(struct swan *s, struct cork_error *err)
{
    struct s0_value  *value;

    rpp_check(s->prelude = swan_prelude_wrapper_new(s, err));
    ep_check(s->prelude->prelude = s0_class_type_new(s, "<prelude>", err));
    ei_check(swan_prelude_bool(s, s->prelude, err));
    ei_check(swan_prelude_control(s, s->prelude, err));

    ep_check(value = s0_type_value_new(s, s->prelude->prelude, err));
    return value;

error:
    cork_gc_decref(swan_gc(s), s->prelude);
    s->prelude = NULL;
    return NULL;
}
