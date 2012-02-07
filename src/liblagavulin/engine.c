/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcork/core.h>
#include <libcork/helpers/errors.h>

#include "swanson/engine.h"
#include "swanson/state.h"
#include "swanson/swanson0.h"

#include "lagavulin/block.h"
#include "lagavulin/engine.h"


/*-----------------------------------------------------------------------
 * The kernel
 */

static struct swan_expression *
lgv_int_int_literal(struct swan *s, struct swan_macro *macro,
                    struct cork_error *err, size_t num_args, ...)
{
    if (num_args != 1) {
        cork_error_set
            (err, SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Wrong number of arguments to %s (got %zu, expected %zu)",
             "int:int_literal", num_args, (size_t) 1);
        return NULL;
    }

    va_list  args;
    va_start(args, num_args);

    struct swan_string  *str;
    rpp_check(str = swan_check_arg_string(s, args, "int:int_literal", 1, err));

    char  *endptr;
    long  l_value = strtol(str->value, &endptr, 0);
    if (*endptr != '\0' || errno == ERANGE ||
        l_value < INT_MIN || l_value > INT_MAX) {
        cork_error_set
            (err, SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Expected string constant for argument 0 in %s",
             "int:int_literal");
        return NULL;
    }

    struct lgv_block  *block = NULL;
    struct swan_expression  *expr = NULL;
    rpp_check(block = lgv_block_new_constant_int(s, (int) l_value));
    rpp_check(expr = lgv_expression_new(s, block, err));
    return expr;
}

static struct swan_expression *
lgv_int_add(struct swan *s, struct swan_macro *macro,
            struct cork_error *err, size_t num_args, ...)
{
    if (num_args != 2) {
        cork_error_set
            (err, SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Wrong number of arguments to %s (got %zu, expected %zu)",
             "int:add", num_args, (size_t) 2);
        return NULL;
    }

    va_list  args;
    va_start(args, num_args);

    struct swan_expression  *lhs;
    struct swan_expression  *rhs;
    rpp_check(lhs = swan_check_arg_expression(s, args, "int:add", 1, err));
    rpp_check(rhs = swan_check_arg_expression(s, args, "int:add", 2, err));

    struct lgv_block  *blhs = lgv_expression_block(lhs);
    struct lgv_block  *brhs = lgv_expression_block(rhs);
    struct lgv_block  *block = NULL;
    struct swan_expression  *expr = NULL;

    rpp_check(block = lgv_block_new_add_int(s));
    rpp_check(block = lgv_block_new_seq(s, brhs, block));
    rpp_check(block = lgv_block_new_seq(s, blhs, block));
    rpp_check(expr = lgv_expression_new(s, block, err));
    return expr;
}


static struct swan_scope *
lgv_engine_create_kernel(struct swan *s, struct cork_error *err)
{
    struct swan_scope  *kernel = NULL;
    struct swan_macro  *macro = NULL;

    ep_check(kernel = swan_scope_new(s, "kernel", NULL, err));

    ep_check(macro = swan_macro_new
             (s, "int:int-literal", lgv_int_int_literal, err));
    ei_check(swan_scope_add
             (s, kernel, "int:int-literal", swan_macro_obj(macro), err));

    ep_check(macro = swan_macro_new
             (s, "int:add", lgv_int_add, err));
    ei_check(swan_scope_add
             (s, kernel, "int:add", swan_macro_obj(macro), err));

    return kernel;

error:
    /* We don't have to decref macro.  Either the error will have come
     * from swan_macro_new, in which case there's nothing to decref, or
     * we'll have already passed it in to swan_scope_add. */

    if (kernel != NULL) {
        cork_gc_decref(swan_gc(s), kernel);
    }
    return NULL;
}


/*-----------------------------------------------------------------------
 * Execution engine
 */

static int
lgv_engine_execute(struct swan *s, struct swan_expression *expr,
                   struct cork_error *err)
{
    struct lgv_engine  *engine =
        cork_container_of(s->engine, struct lgv_engine, parent);
    struct lgv_block  *block = lgv_expression_block(expr);
    struct lgv_block  *halt = NULL;
    struct lgv_block  *head = NULL;

    ep_check(halt = lgv_block_new_halt(s));
    lgv_block_set_next(s, block, halt);
    head = lgv_block_get_head(s, block);
    ei_check(lgv_block_execute
             (s, head, &engine->root_state, engine->root_state.stack.top));
    return 0;

error:
    if (halt != NULL) {
        cork_gc_decref(swan_gc(s), halt);
    }

    return -1;
}

static struct swan_engine *
lgv_engine_new(struct swan *s, struct cork_error *err)
{
    struct lgv_engine  *self = NULL;
    rp_check_new(struct lgv_engine, self, "Lagavulin engine");

    ei_check(lgv_state_init(s, &self->root_state));
    self->parent.execute = lgv_engine_execute;
    self->parent.create_kernel = lgv_engine_create_kernel;
    return &self->parent;

error:
    /* We don't call lgv_engine_free since it will try to finalize the
     * non-initialized state object. */
    cork_delete(struct lgv_engine, self);
    return NULL;
}

static void
lgv_engine_free(struct swan *s, struct swan_engine *vself)
{
    struct lgv_engine  *self =
        cork_container_of(vself, struct lgv_engine, parent);
    lgv_state_done(s, &self->root_state);
    cork_delete(struct lgv_engine, self);
}


struct swan *
lgv_new(struct cork_error *err)
{
    struct swan  *self = NULL;
    rp_check_new(struct swan, self, "Lagavulin state");

    ei_check(swan_init(self, err));
    ep_check(self->engine = lgv_engine_new(self, err));
    return self;

error:
    if (self->engine != NULL) {
        lgv_engine_free(self, self->engine);
    }
    swan_done(self);

    cork_delete(struct swan, self);
    return NULL;
}

void
lgv_free(struct swan *s)
{
}
