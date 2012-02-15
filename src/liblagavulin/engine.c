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
                    size_t num_args, ...)
{
    if (num_args != 1) {
        cork_error_set
            (SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Wrong number of arguments to %s (got %zu, expected %zu)",
             "int:int_literal", num_args, (size_t) 1);
        return NULL;
    }

    va_list  args;
    va_start(args, num_args);

    struct swan_string  *str;
    rpp_check(str = swan_check_arg_string(s, args, "int:int_literal", 1));

    char  *endptr;
    long  l_value = strtol(str->value, &endptr, 0);
    if (*endptr != '\0' || errno == ERANGE ||
        l_value < INT_MIN || l_value > INT_MAX) {
        cork_error_set
            (SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Expected string constant for argument 0 in %s",
             "int:int_literal");
        return NULL;
    }

    struct lgv_block  *block = NULL;
    struct swan_expression  *expr = NULL;
    rpp_check(block = lgv_block_new_constant_int(s, (int) l_value));
    rpp_check(expr = lgv_expression_new(s, block));
    return expr;
}

static struct swan_expression *
lgv_int_add(struct swan *s, struct swan_macro *macro,
            size_t num_args, ...)
{
    if (num_args != 2) {
        cork_error_set
            (SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Wrong number of arguments to %s (got %zu, expected %zu)",
             "int:add", num_args, (size_t) 2);
        return NULL;
    }

    va_list  args;
    va_start(args, num_args);

    struct swan_expression  *lhs;
    struct swan_expression  *rhs;
    rpp_check(lhs = swan_check_arg_expression(s, args, "int:add", 1));
    rpp_check(rhs = swan_check_arg_expression(s, args, "int:add", 2));

    struct lgv_block  *blhs = lgv_expression_block(lhs);
    struct lgv_block  *brhs = lgv_expression_block(rhs);
    struct lgv_block  *block = NULL;
    struct swan_expression  *expr = NULL;

    rpp_check(block = lgv_block_new_add_int(s));
    rpp_check(block = lgv_block_new_seq(s, brhs, block));
    rpp_check(block = lgv_block_new_seq(s, blhs, block));
    rpp_check(expr = lgv_expression_new(s, block));
    return expr;
}


static struct swan_scope *
lgv_engine_create_kernel(struct swan *s)
{
    struct swan_scope  *kernel = NULL;
    struct swan_macro  *macro = NULL;

    ep_check(kernel = swan_scope_new(s, "kernel", NULL));

    ep_check(macro = swan_macro_new
             (s, "int:int-literal", lgv_int_int_literal));
    ei_check(swan_scope_add
             (s, kernel, "int:int-literal", swan_macro_obj(macro)));

    ep_check(macro = swan_macro_new
             (s, "int:add", lgv_int_add));
    ei_check(swan_scope_add
             (s, kernel, "int:add", swan_macro_obj(macro)));

    return kernel;

error:
    /* We don't have to decref macro.  Either the error will have come
     * from swan_macro_new, in which case there's nothing to decref, or
     * we'll have already passed it in to swan_scope_add. */

    if (kernel != NULL) {
        cork_gc_decref(kernel);
    }
    return NULL;
}


/*-----------------------------------------------------------------------
 * Execution engine
 */

static int
lgv_engine_execute(struct swan *s, struct swan_expression *expr)
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
        cork_gc_decref(halt);
    }

    return -1;
}

static struct swan_engine *
lgv_engine_new(struct swan *s)
{
    struct lgv_engine  *self = cork_new(struct lgv_engine);
    lgv_state_init(s, &self->root_state);
    self->parent.execute = lgv_engine_execute;
    self->parent.create_kernel = lgv_engine_create_kernel;
    return &self->parent;
}

static void
lgv_engine_free(struct swan *s, struct swan_engine *vself)
{
    struct lgv_engine  *self =
        cork_container_of(vself, struct lgv_engine, parent);
    lgv_state_done(s, &self->root_state);
    free(self);
}


struct swan *
lgv_new(void)
{
    struct swan  *self = cork_new(struct swan);
    swan_init(self);
    self->engine = lgv_engine_new(self);
    return self;
}

void
lgv_free(struct swan *s)
{
    lgv_engine_free(s, s->engine);
    free(s);
}
