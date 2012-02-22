/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <libcork/core.h>
#include <libcork/helpers/errors.h>
#include <libcork/helpers/gc.h>

#include "swanson/ast.h"
#include "swanson/metamodel.h"
#include "swanson/state.h"

_recurse_(swan_ast_call) {
    struct swan_ast_call  *self = obj;
    recurse(gc, self->results, ud);
    recurse(gc, self->thing, ud);
    recurse(gc, self->method, ud);
    recurse(gc, self->params, ud);
}

_gc_no_free_(swan_ast_call);

struct swan_ast_call *
swan_ast_call_new(struct swan *s, struct swan_sllist *results,
                  struct swan_static_string *thing,
                  struct swan_static_string *method,
                  struct swan_sllist *params)
{
    struct swan_ast_call  *self = cork_gc_new(swan_ast_call);
    self->results = results;
    self->thing = thing;
    self->method = method;
    self->params = params;
    return self;
}
