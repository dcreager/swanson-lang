/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_ENGINE_H
#define SWANSON_ENGINE_H

/**
 * @file
 * @brief Implementation of the @ref engine module
 */

#include <libcork/core.h>

#include <swanson/state.h>
#include <swanson/swanson0.h>

/**
 * @addtogroup engine Execution engine
 *
 * <tt>#%include \<swanson/engine.h\></tt>
 */

struct swan_engine {
    /* Executes the given top-level expression */
    int
    (*execute)(struct swan *s, struct swan_expression *expr,
               struct cork_error *err);

    /* Creates a Swanson0 kernel */
    struct swan_scope *
    (*create_kernel)(struct swan *s, struct cork_error *err);
};


#define swan_execute(s, expr, err) \
    ((s)->engine->execute((s), (expr), (err)))

#define swan_create_kernel(s, err) \
    ((s)->engine->create_kernel((s), (err)))


#endif  /* SWANSON_ENGINE_H */
