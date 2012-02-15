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
#include <swanson/s0.h>
#include <swanson/swanson0.h>

/**
 * @addtogroup engine Execution engine
 *
 * <tt>#%include \<swanson/engine.h\></tt>
 */

struct swan_engine {
    /* Executes the given top-level expression */
    int
    (*execute)(struct swan *s, struct swan_expression *expr);

    /* Creates a Swanson0 kernel */
    struct swan_scope *
    (*create_kernel)(struct swan *s);
};


#define swan_execute(s, expr) \
    ((s)->engine->execute((s), (expr)))

#define swan_create_kernel(s) \
    ((s)->engine->create_kernel((s)))


struct s0_value *
swan_prelude_new(struct swan *s);


#endif  /* SWANSON_ENGINE_H */
