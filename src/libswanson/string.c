/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <stdlib.h>
#include <string.h>

#include <libcork/core.h>
#include <libcork/core/checkers.h>

#include "swanson/state.h"
#include "swanson/swanson0.h"

static void
swan_string_free(struct cork_gc *gc, void *vself)
{
    struct swan_string  *self = vself;
    if (self->value != NULL) {
        free((void *) self->value);
    }
}

static struct cork_gc_obj_iface  swan_string_gc = {
    swan_string_free, NULL
};

struct swan_string *
swan_string_new(struct swan *s, const char *value, size_t length,
                struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);

    struct swan_string  *self = NULL;
    e_check_gc_new(swan_string, self, "string");
    self->parent.cls = SWAN_STRING_CLASS;

    ep_check(self->value = malloc(length));
    memcpy((void *) self->value, value, length);
    self->length = length;

    return self;

error:
    if (self != NULL) {
        cork_gc_decref(swan_gc(s), self);
    }

    return NULL;
}

bool
swan_string_equal(struct swan_string *s1, struct swan_string *s2)
{
    return (s1->length == s2->length) &&
        (memcmp(s1->value, s2->value, s1->length) == 0);
}
