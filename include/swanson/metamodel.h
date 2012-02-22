/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_METAMODEL_H
#define SWANSON_METAMODEL_H


#include <stdio.h>
#include <stdlib.h>

#include <libcork/core.h>
#include <libcork/ds.h>

#include <swanson/state.h>


/*-----------------------------------------------------------------------
 * Error handling
 */

/* Hash of "swanson.metamodel" */
#define SWAN_METAMODEL_ERROR  0x31564802

enum swan_metamodel_error {
    SWAN_METAMODEL_PARSE_ERROR,
    SWAN_METAMODEL_REDEFINED,
    SWAN_METAMODEL_UNDEFINED
};

void
swan_metamodel_redefined(const char *breed_name, const char *method_name);

void
swan_metamodel_undefined(const char *breed_name, const char *method_name);


/*-----------------------------------------------------------------------
 * Things, breedes, and metabreedes
 */

/* Forward-declared in swanson/state.h */
/* struct swan_breed; */

struct swan_thing {
    struct swan_breed  *b;
};

struct swan_method;

typedef cork_hash  swan_breed_id;

struct swan_breed {
    struct swan_thing  parent;
    swan_breed_id  id;
    const char  *name;

    struct swan_method *
    (*get_method)(struct swan *s, struct swan_breed *c, const char *name);
};

#define swan_breed_get_method(s, b, name) \
    ((b)->get_method((s), (b), (name)))

#define swan_thing_instance_of(t, breed_prefix) \
    ((t)->b->id == breed_prefix##___breed_id)


/* First result returned as C function's return value.  Others are
 * placed into results.  results must have room for (result_count-1)
 * pointers. */
typedef struct swan_thing *
(*swan_method_evaluate)(struct swan *s, struct swan_method *method,
                        size_t param_count, struct swan_thing **params,
                        size_t result_count, struct swan_thing **results);

struct swan_method {
    swan_method_evaluate  evaluate;
};

#define swan_method_evaluate(s, m, pc, ps, rc, rs) \
    ((m)->evaluate((s), (m), (pc), (ps), (rc), (rs)))


CORK_ATTR_UNUSED
static struct swan_thing *
swan_thing_call_method(struct swan *s, struct swan_breed *b, const char *name,
                       size_t param_count, struct swan_thing **params,
                       size_t result_count, struct swan_thing **results)
{
    struct swan_method  *method = swan_breed_get_method(s, b, name);
    if (method == NULL) {
        return NULL;
    }
    return swan_method_evaluate
        (s, method, param_count, params, result_count, results);
}


#endif  /* SWANSON_METAMODEL_H */
