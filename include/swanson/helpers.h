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
 * Classes and methods
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
struct swan_object * \
name##__eval(struct swan *s, struct swan_method *method, \
             size_t param_count, struct swan_object **params, \
             size_t result_count, struct swan_object **results)


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

#define _ensure_class_(dest, src, prefix) \
    do { \
        if (CORK_UNLIKELY(!swan_object_instance_of(src, prefix))) { \
            _abort_method_("expected " #src " to be instance of %s, got %s", \
                           prefix##___class_name, (src)->c->name); \
        } \
        dest = swan_object_to_##prefix(src); \
    } while (0)


#define _method_(name) \
_eval_(name); \
static struct swan_method  name##__method = { name##__eval };

#define _default_methods_(name) \
_method_(name##__class); \
_eval_(name##__class) { \
    _ensure_param_count_(1); \
    _ensure_result_count_(1); \
    return &params[0]->c->parent; \
}


#define _eval_const_(name, value) \
_eval_(name) { \
    _ensure_param_count_(0); \
    _ensure_result_count_(1); \
    return (value); \
}

#define _eval_obj_(name, value)  _eval_const_(name, &(value).parent)


#define _get_method_(class_name) \
static struct swan_method * \
class_name##__get_method(struct swan *s, struct swan_class *vc, \
                         const char *name)

#define _return_op_(class_name, method_name, op) \
    do { \
        if (strcmp(name, op) == 0) { \
            return &class_name##__##method_name##__method; \
        } \
    } while (0)

#define _return_method_(class_name, method_name) \
    _return_op_(class_name, method_name, #method_name)

#define _return_default_methods_(class_name) \
    _return_method_(class_name, class)

#define _class_with_meta_(name, meta_name) \
_get_method_(name); \
static struct swan_class  name##__class = { \
    { &meta_name }, \
    name##___class_id, name##___class_name, name##__get_method \
};

#define _class_(name) \
_class_with_meta_(name, swan_static_metaclass);


CORK_ATTR_UNUSED
static struct swan_object *
swan_object_new_basic(struct swan *s, struct swan_class *c)
{
    struct swan_object  *self = cork_new(struct swan_object);
    self->c = c;
    return self;
}


#endif  /* SWANSON_HELPERS_H */
