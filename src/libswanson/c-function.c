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


void
s0_c_function_init(struct swan *s, struct s0_c_function *self,
                   const char *name, s0_c_func func,
                   struct s0_type *input, struct s0_type *output)
{
    self->name = cork_strdup(name);
    self->func = func;
    self->input = input;
    self->output = output;
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
                  struct s0_type *input, struct s0_type *output)
{
    struct s0_c_function  *self = cork_gc_new(s0_c_function);
    s0_c_function_init(s, self, name, func, input, output);
    return self;
}
