/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <string.h>

#include <libcork/core.h>
#include <libcork/core/checkers.h>

#include "swanson/s0.h"
#include "swanson/state.h"


int
s0_c_function_init(struct swan *s, struct s0_c_function *self,
                   const char *name, s0_c_func func,
                   struct s0_type *input, struct s0_type *output,
                   struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    ri_check_alloc(self->name = cork_strdup(swan_alloc(s), name),
                   "C function name");
    self->func = func;
    self->input = input;
    self->output = output;
    return 0;
}

void
s0_c_function_done(struct cork_gc *gc, struct s0_c_function *self)
{
    cork_strfree(gc->alloc, self->name);
}

void
s0_c_function_recurse(struct cork_gc *gc, struct s0_c_function *self,
                      cork_gc_recurser recurse, void *ud)
{
    recurse(gc, self->input, ud);
    recurse(gc, self->output, ud);
}


static void
s0_c_function_gc_free(struct cork_gc *gc, void *vself)
{
    s0_c_function_done(gc, vself);
}

static void
s0_c_function_gc_recurse(struct cork_gc *gc, void *vself,
                         cork_gc_recurser recurse, void *ud)
{
    s0_c_function_recurse(gc, vself, recurse, ud);
}

static struct cork_gc_obj_iface  s0_c_function_gc = {
    s0_c_function_gc_free, s0_c_function_gc_recurse
};

struct s0_c_function *
s0_c_function_new(struct swan *s, const char *name, s0_c_func func,
                  struct s0_type *input, struct s0_type *output,
                  struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_c_function  *self;
    rp_check_gc_new(s0_c_function, self, "C function");
    memset(self, 0, sizeof(struct s0_c_function));
    ei_check(s0_c_function_init(s, self, name, func, input, output, err));
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}
