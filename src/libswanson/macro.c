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

#include "swanson/checkers.h"
#include "swanson/state.h"
#include "swanson/swanson0.h"

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
    struct swan_macro  *self = NULL;
    e_pcheck(self = cork_gc_new(swan_gc(s), struct swan_macro,
                                &swan_macro_gc));
    e_pcheck(self->name = cork_strdup(swan_alloc(s), name));
    self->parent.cls = SWAN_MACRO_CLASS;
    self->execute = execute;
    return self;

error:
    if (self != NULL) {
        cork_gc_decref(swan_gc(s), self);
    }

    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new macro %s", name);
    return NULL;
}
