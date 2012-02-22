/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_AST_H
#define SWANSON_AST_H


#include <swanson/metamodel.h>
#include <swanson/prelude.h>
#include <swanson/state.h>


/*-----------------------------------------------------------------------
 * S0 abstract syntax tree
 */

#define swan_ast_call___breed_id  0x5c1ee7c9
#define swan_ast_call___breed_name  "swan_ast_call"

struct swan_ast_call {
    struct swan_thing  parent;
    struct swan_sllist  *results;
    struct swan_static_string  *thing;
    struct swan_static_string  *method;
    struct swan_sllist  *params;
};

#define swan_thing_to_swan_ast_call(t) \
    (cork_container_of((t), struct swan_ast_call, parent))

/* Steals references to all params */
struct swan_ast_call *
swan_ast_call_new(struct swan *s, struct swan_sllist *results,
                  struct swan_static_string *thing,
                  struct swan_static_string *method,
                  struct swan_sllist *params);

struct swan_sllist *
swan_ast_call_parse(struct swan *s, const char *buf, size_t size);


#endif  /* SWANSON_AST_H */
