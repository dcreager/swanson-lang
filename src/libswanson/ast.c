/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <string.h>

#include <libcork/core.h>
#include <libcork/ds.h>
#include <libcork/helpers/errors.h>
#include <libcork/helpers/gc.h>

#include "swanson/ast.h"
#include "swanson/ds.h"
#include "swanson/metamodel.h"
#include "swanson/state.h"

_recurse_(swan_ast_call) {
    struct swan_ast_call  *self = obj;
    size_t  i;
    recurse(gc, self->thing, ud);
    recurse(gc, self->method, ud);
    for (i = 0; i < cork_array_size(&self->params); i++) {
        recurse(gc, cork_array_at(&self->params, i), ud);
    }
    for (i = 0; i < cork_array_size(&self->results); i++) {
        recurse(gc, cork_array_at(&self->results, i), ud);
    }
}

_free_(swan_ast_call) {
    struct swan_ast_call  *self = obj;
    cork_array_done(&self->params);
    cork_array_done(&self->results);
}

_gc_(swan_ast_call);

struct swan_ast_call *
swan_ast_call_new(struct swan *s)
{
    struct swan_ast_call  *self = cork_gc_new(swan_ast_call);
    memset(self, 0, sizeof(struct swan_ast_call));
    self->thing = NULL;
    self->method = NULL;
    cork_array_init(&self->params);
    cork_array_init(&self->results);
    return self;
}

void
swan_ast_call_add_param(struct swan *s, struct swan_ast_call *self,
                        struct swan_static_string *id)
{
    cork_array_append(&self->params, id);
}

void
swan_ast_call_add_result(struct swan *s, struct swan_ast_call *self,
                         struct swan_static_string *id)
{
    cork_array_append(&self->results, id);
}


_recurse_(swan_ast) {
    struct swan_ast  *self = obj;
    struct cork_dllist_item  *curr;
    struct cork_dllist_item  *next;

    for (curr = cork_dllist_start(&self->calls);
         !cork_dllist_is_end(&self->calls, curr); curr = next) {
        struct swan_ast_call  *call =
            cork_container_of(curr, struct swan_ast_call, list);
        next = curr->next;
        recurse(gc, call, ud);
    }

#if 0
    for (curr = cork_dllist_start(&self->upvalues);
         !cork_dllist_is_end(&self->upvalues, curr); curr = next) {
        struct swan_ast_upvalue  *upvalue =
            cork_container_of(curr, struct swan_ast_upvalue, list);
        next = curr->next;
        recurse(gc, upvalue->value, ud);
    }
#endif
}

_free_(swan_ast) {
    struct swan_ast  *self = obj;
    struct cork_dllist_item  *curr;
    struct cork_dllist_item  *next;

    for (curr = cork_dllist_start(&self->upvalues);
         !cork_dllist_is_end(&self->upvalues, curr); curr = next) {
        struct swan_ast_upvalue  *upvalue =
            cork_container_of(curr, struct swan_ast_upvalue, list);
        next = curr->next;
        cork_strfree(upvalue->name);
        free(upvalue);
    }
}

_gc_(swan_ast);

struct swan_ast *
swan_ast_new(struct swan *s)
{
    struct swan_ast  *self = cork_gc_new(swan_ast);
    cork_dllist_init(&self->calls);
    cork_dllist_init(&self->upvalues);
    return self;
}

void
swan_ast_add_call(struct swan *s, struct swan_ast *self,
                  struct swan_ast_call *call)
{
    cork_dllist_add(&self->calls, &call->list);
}

void
swan_ast_add_upvalue(struct swan *s, struct swan_ast *self,
                     const char *name, struct swan_thing *value)
{
    struct swan_ast_upvalue  *upvalue = cork_new(struct swan_ast_upvalue);
    upvalue->name = cork_strdup(name);
    upvalue->value = value;
    cork_dllist_add(&self->upvalues, &upvalue->list);
}
