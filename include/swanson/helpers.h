/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_HELPERS_H
#define SWANSON_HELPERS_H

#include <stdlib.h>
#include <string.h>

#include <libcork/helpers/errors.h>

#include <swanson/metamodel.h>


/*-----------------------------------------------------------------------
 * Breeds and methods
 */

#if defined(NDEBUG)
#define _abort_method_(fmt, ...)  ((void) 0)
#else  /* !defined(NDEBUG) */
#include <stdio.h>
#define _abort_method__(func, file, line, fmt, ...) \
    do { \
        fprintf(stderr, "[%s:%u] %s " fmt "\n", \
                file, line, func, __VA_ARGS__); \
        abort(); \
    } while (0)
#define _abort_method_(fmt, ...) \
    _abort_method__(__func__, __FILE__, __LINE__, fmt, __VA_ARGS__)
#endif


#define _eval_(name) \
struct swan_thing * \
name##__eval(struct swan *s, struct swan_method *method, \
             size_t param_count, struct swan_thing **params, \
             size_t result_count, struct swan_thing **results)


#define _ensure_param_count_(expected) \
    if (CORK_UNLIKELY(param_count != (expected))) { \
        _abort_method_("expects %zu parameters, caller provided %zu", \
                       (size_t) (expected), param_count); \
    }

#define _ensure_result_count_(expected) \
    if (CORK_UNLIKELY(result_count != (expected))) { \
        _abort_method_("produces %zu results, caller expects %zu", \
                       (size_t) (expected), result_count); \
    }

#define _ensure_breed_(dest, src, prefix) \
    do { \
        if (CORK_UNLIKELY(!swan_thing_instance_of(src, prefix))) { \
            _abort_method_("expected " #src " to be instance of %s, got %s", \
                           prefix##___breed_name, (src)->b->name); \
        } \
        dest = swan_thing_to_##prefix(src); \
    } while (0)


#define _method_(name) \
_eval_(name); \
static struct swan_method  name##__method = { name##__eval };

#define _default_methods_(name) \
_method_(name##__breed); \
_eval_(name##__breed) { \
    _ensure_param_count_(1); \
    _ensure_result_count_(1); \
    return &params[0]->b->parent; \
}


#define _eval_const_(name, value) \
_eval_(name) { \
    _ensure_param_count_(0); \
    _ensure_result_count_(1); \
    return (value); \
}

#define _eval_obj_(name, value)  _eval_const_(name, &(value).parent)


#define _get_method_(breed_name) \
static struct swan_method * \
breed_name##__get_method(struct swan *s, struct swan_breed *vb, \
                         const char *name)

#define _return_op_(breed_name, method_name, op) \
    do { \
        if (strcmp(name, op) == 0) { \
            return &breed_name##__##method_name##__method; \
        } \
    } while (0)

#define _return_method_(breed_name, method_name) \
    _return_op_(breed_name, method_name, #method_name)

#define _return_default_methods_(breed_name) \
    _return_method_(breed_name, breed)

#define _breed_with_meta_(name, meta_name) \
_get_method_(name); \
static struct swan_breed  name##__breed = { \
    { &meta_name }, \
    name##___breed_id, name##___breed_name, name##__get_method \
};

#define _breed_(name) \
_breed_with_meta_(name, swan_static_metabreed);


CORK_ATTR_UNUSED
static struct swan_thing *
swan_thing_new_basic(struct swan *s, struct swan_breed *b)
{
    struct swan_thing  *self = cork_new(struct swan_thing);
    self->b = b;
    return self;
}


#endif  /* SWANSON_HELPERS_H */
