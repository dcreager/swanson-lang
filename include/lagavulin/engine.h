/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef LAGAVULIN_ENGINE_H
#define LAGAVULIN_ENGINE_H

/**
 * @file
 * @brief Implementation of the @ref engine module
 */

#include <libcork/core.h>

#include <swanson/engine.h>
#include <swanson/state.h>
#include <swanson/swanson0.h>

#include <lagavulin/block.h>

/**
 * @addtogroup engine The Lagavulin execution engine
 *
 * <tt>#%include \<lagavulin/engine.h\></tt>
 */

struct lgv_engine {
    struct swan_engine  parent;
    struct lgv_state  root_state;
};

#define lgv_engine_state(seng) \
    (&(cork_container_of((seng), struct lgv_engine, parent)->root_state))

struct swan *
lgv_new(struct cork_alloc *alloc, struct cork_error *err);

void
lgv_free(struct swan *s);


struct lgv_expression {
    struct swan_expression  parent;
    struct lgv_block  *block;
};

#define lgv_expression_block(sexpr) \
    (cork_container_of((sexpr), struct lgv_expression, parent)->block)

struct swan_expression *
lgv_expression_new(struct swan *s, struct lgv_block *b,
                   struct cork_error *err);


#endif  /* LAGAVULIN_ENGINE_H */
