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
#include <libcork/helpers/errors.h>
#include <libcork/helpers/gc.h>

#include "swanson/s0.h"
#include "swanson/state.h"


int
s0_c_function_init(struct swan *s, struct s0_c_function *self,
                   const char *name, s0_c_func func,
                   struct s0_type *input, struct s0_type *output,
                   struct cork_error *err)
{
    ri_check_alloc(self->name = cork_strdup(name), "C function name");
    self->func = func;
    self->input = input;
    self->output = output;
    return 0;
}


_free_(s0_c_function) {
    struct s0_c_function  *self = obj;
    cork_strfree(self->name);
}

_recurse_(s0_c_function) {
    struct s0_c_function  *self = obj;
    recurse(gc, self->input, ud);
    recurse(gc, self->output, ud);
}

_gc_(s0_c_function);

struct s0_c_function *
s0_c_function_new(struct swan *s, const char *name, s0_c_func func,
                  struct s0_type *input, struct s0_type *output,
                  struct cork_error *err)
{
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
