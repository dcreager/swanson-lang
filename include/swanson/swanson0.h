/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_SWANSON0_H
#define SWANSON_SWANSON0_H

/**
 * @file
 * @brief Implementation of the @ref swanson0 module
 */

#include <stdarg.h>

#include <libcork/core.h>
#include <libcork/ds.h>

#include <swanson/state.h>

/**
 * @addtogroup swanson0 Swanson0 language
 *
 * <tt>#%include \<swanson/swanson0.h\></tt>
 */


typedef cork_hash  swan_class;

struct swan_obj {
    swan_class  cls;
};

#define swan_obj_class(obj)  ((obj)->cls)


/* Hash of "swanson.scope" */
#define SWAN_SCOPE_CLASS  0x2511f99c
#define swan_is_scope(obj) \
    (swan_obj_class((obj)) == SWAN_SCOPE_CLASS)
#define swan_obj_scope(obj) \
    (cork_container_of((obj), struct swan_scope, parent))
#define swan_scope_obj(obj) \
    (&((obj)->parent))


/* Hash of "swanson.scope" */
#define SWAN_SCOPE_ERROR  0x2511f99c
enum swan_scope_error {
    SWAN_SCOPE_REDEFINED,
    SWAN_SCOPE_UNDEFINED
};


struct swan_scope {
    struct swan_obj  parent;
    const char  *name;
    struct cork_hash_table  entries;
    struct swan_scope  *parent_scope;
};

struct swan_scope *
swan_scope_new(struct swan *s, const char *name, struct swan_scope *parent);

int
swan_scope_add(struct swan *s, struct swan_scope *scope,
               const char *name, struct swan_obj *obj);

struct swan_obj *
swan_scope_get(struct swan *s, struct swan_scope *scope,
               const char *name);


/* Hash of "swanson.string" */
#define SWAN_STRING_CLASS  0xe207edbd
#define swan_is_string(obj) \
    (swan_obj_class((obj)) == SWAN_STRING_CLASS)
#define swan_obj_string(obj) \
    (cork_container_of((obj), struct swan_string, parent))
#define swan_string_obj(obj) \
    (&((obj)->parent))

struct swan_string {
    struct swan_obj  parent;
    size_t  length;
    const char  *value;
};

struct swan_string *
swan_string_new(struct swan *s, const char *value, size_t length);

bool
swan_string_equal(struct swan_string *s1, struct swan_string *s2);


/* Hash of "swanson.macro" */
#define SWAN_MACRO_CLASS  0xc007f757
#define swan_is_macro(obj) \
    (swan_obj_class((obj)) == SWAN_MACRO_CLASS)
#define swan_obj_macro(obj) \
    (cork_container_of((obj), struct swan_macro, parent))
#define swan_macro_obj(obj) \
    (&((obj)->parent))

struct swan_macro;

typedef struct swan_expression *
(*swan_macro_execute_func)(struct swan *s, struct swan_macro *macro,
                           size_t num_args, ...);

struct swan_macro {
    struct swan_obj  parent;
    const char  *name;
    swan_macro_execute_func  execute;
};

struct swan_macro *
swan_macro_new(struct swan *s, const char *name,
               swan_macro_execute_func execute);

#define swan_macro_execute(s, macro, ...) \
    ((macro)->execute((s), (macro), __VA_ARGS__))

/* Hash of "swanson.macro" */
#define SWAN_MACRO_ERROR  0xc007f757
enum swan_macro_error {
    SWAN_MACRO_INVALID_ARGUMENT
};


/* Hash of "swanson.expression" */
#define SWAN_EXPRESSION_CLASS  0x4a5257c1
#define swan_is_expression(obj) \
    (swan_obj_class((obj)) == SWAN_EXPRESSION_CLASS)
#define swan_obj_expression(obj) \
    (cork_container_of((obj), struct swan_expression, parent))
#define swan_expression_obj(obj) \
    (&((obj)->parent))

struct swan_expression {
    struct swan_obj  parent;
    /* Additional fields as needed by the current engine */
};

int
swan_expression_init(struct swan *s, struct swan_expression *expr);


struct swan_scope *
swan_check_arg_scope(struct swan *s, va_list args,
                     const char *context, size_t arg_num);

struct swan_string *
swan_check_arg_string(struct swan *s, va_list args,
                      const char *context, size_t arg_num);

struct swan_macro *
swan_check_arg_macro(struct swan *s, va_list args,
                     const char *context, size_t arg_num);

struct swan_expression *
swan_check_arg_expression(struct swan *s, va_list args,
                          const char *context, size_t arg_num);


#endif  /* SWANSON_SWANSON0_H */
