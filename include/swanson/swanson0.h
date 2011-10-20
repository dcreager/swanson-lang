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
    SWAN_SCOPE_ERROR_REDEFINED,
    SWAN_SCOPE_ERROR_UNDEFINED
};

struct swan_scope {
    struct swan_obj  parent;
    const char  *name;
    struct cork_hash_table  entries;
};

struct swan_scope *
swan_scope_new(struct swan *s, const char *name, struct cork_error *err);

int
swan_scope_add(struct swan *s, struct swan_scope *self,
               const char *name, struct swan_obj *obj,
               struct cork_error *err);

struct swan_obj *
swan_scope_get(struct swan *s, struct swan_scope *self,
               const char *name, struct cork_error *err);


#endif  /* SWANSON_SWANSON0_H */
