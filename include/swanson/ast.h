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


#include <libcork/ds.h>

#include <swanson/metamodel.h>
#include <swanson/prelude.h>
#include <swanson/state.h>


/*-----------------------------------------------------------------------
 * S0 abstract syntax tree
 */

struct swan_ast_element {
    struct swan_thing  parent;
    /* This is a hack, but a temporary one */
    cork_hash  element_type;
    struct cork_dllist_item  list;
};


#define swan_ast_string___breed_id  0x729bf289
#define swan_ast_string___breed_name  "swan_ast_string"

struct swan_ast_string {
    struct swan_ast_element  parent;
    struct swan_static_string  *result;
    struct swan_static_string  *contents;
};

#define swan_thing_to_swan_ast_string(t) \
    (cork_container_of((t), struct swan_ast_string, parent.parent))

/* Steals references to all params */
struct swan_ast_string *
swan_ast_string_new(struct swan *s, struct swan_static_string *result,
                    struct swan_static_string *contents);


#define swan_ast_call___breed_id  0x5c1ee7c9
#define swan_ast_call___breed_name  "swan_ast_call"

typedef cork_array(struct swan_static_string *)  swan_static_string_array;

struct swan_ast_call {
    struct swan_ast_element  parent;
    struct swan_static_string  *thing;
    struct swan_static_string  *method;
    swan_static_string_array  params;
    swan_static_string_array  results;
};

#define swan_thing_to_swan_ast_call(t) \
    (cork_container_of((t), struct swan_ast_call, parent.parent))

/* Steals references to all params */
struct swan_ast_call *
swan_ast_call_new(struct swan *s);

#define swan_ast_call_set_thing(s, ast, t) \
    ((ast)->thing = (t))

#define swan_ast_call_set_method(s, ast, m) \
    ((ast)->method = (m))

void
swan_ast_call_add_param(struct swan *s, struct swan_ast_call *call,
                        struct swan_static_string *id);

void
swan_ast_call_add_result(struct swan *s, struct swan_ast_call *call,
                         struct swan_static_string *id);


#define swan_ast___breed_id  0x91d9df69
#define swan_ast___breed_name  "swan_ast"

struct swan_ast_upvalue {
    struct swan_ast_upvalue  *tail;
    const char  *name;
    struct swan_thing  *value;
    struct cork_dllist_item  list;
};

struct swan_ast {
    struct swan_thing  parent;
    struct cork_dllist  elements;
    struct cork_dllist  upvalues;
};


struct swan_ast *
swan_ast_new(struct swan *s);

/* Steals reference to call */
void
swan_ast_add_element(struct swan *s, struct swan_ast *ast,
                     struct swan_ast_element *element);

/* Creates new reference to value */
void
swan_ast_add_upvalue(struct swan *s, struct swan_ast *ast,
                     const char *name, struct swan_thing *value);

struct swan_ast *
swan_ast_parse(struct swan *s, const char *buf, size_t size);

int
swan_ast_evaluate(struct swan *s, struct swan_ast *ast);


#endif  /* SWANSON_AST_H */
