/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <libcork/core.h>
#include <libcork/core/checkers.h>

#include "swanson/state.h"
#include "swanson/swanson0.h"


/*-----------------------------------------------------------------------
 * Macros
 */

static void
swan_macro_free(struct cork_gc *gc, void *vself)
{
    struct swan  *s = cork_container_of(gc, struct swan, gc);
    struct swan_macro  *self = vself;
    if (self->name != NULL) {
        cork_strfree(swan_alloc(s), self->name);
    }
}

static struct cork_gc_obj_iface  swan_macro_gc = {
    swan_macro_free, NULL
};

struct swan_macro *
swan_macro_new(struct swan *s, const char *name,
               swan_macro_execute_func execute,
               struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);

    struct swan_macro  *self = NULL;
    e_check_gc_inew(swan_macro, &swan_macro_gc, self, "macro");
    e_check_alloc(self->name = cork_strdup(swan_alloc(s), name), "macro name");
    self->parent.cls = SWAN_MACRO_CLASS;
    self->execute = execute;
    return self;

error:
    if (self != NULL) {
        cork_gc_decref(swan_gc(s), self);
    }

    return NULL;
}


struct swan_scope *
swan_check_arg_scope(struct swan *s, va_list args,
                     const char *context, size_t arg_num,
                     struct cork_error *err)
{
    struct swan_obj  *arg = va_arg(args, struct swan_obj *);
    if (!swan_is_scope(arg)) {
        cork_error_set
            (swan_alloc(s), err, SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Expected scope object for argument %zu in %s",
             arg_num, context);
        return NULL;
    }
    return swan_obj_scope(arg);
}

struct swan_string *
swan_check_arg_string(struct swan *s, va_list args,
                      const char *context, size_t arg_num,
                      struct cork_error *err)
{
    struct swan_obj  *arg = va_arg(args, struct swan_obj *);
    if (!swan_is_string(arg)) {
        cork_error_set
            (swan_alloc(s), err, SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Expected string constant for argument %zu in %s",
             arg_num, context);
        return NULL;
    }
    return swan_obj_string(arg);
}

struct swan_macro *
swan_check_arg_macro(struct swan *s, va_list args,
                     const char *context, size_t arg_num,
                     struct cork_error *err)
{
    struct swan_obj  *arg = va_arg(args, struct swan_obj *);
    if (!swan_is_macro(arg)) {
        cork_error_set
            (swan_alloc(s), err, SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Expected macro for argument %zu in %s",
             arg_num, context);
        return NULL;
    }
    return swan_obj_macro(arg);
}

struct swan_expression *
swan_check_arg_expression(struct swan *s, va_list args,
                          const char *context, size_t arg_num,
                          struct cork_error *err)
{
    struct swan_obj  *arg = va_arg(args, struct swan_obj *);
    if (!swan_is_expression(arg)) {
        cork_error_set
            (swan_alloc(s), err, SWAN_MACRO_ERROR, SWAN_MACRO_INVALID_ARGUMENT,
             "Expected expression for argument %zu in %s",
             arg_num, context);
        return NULL;
    }
    return swan_obj_expression(arg);
}
