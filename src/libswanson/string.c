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

#include "swanson/checkers.h"
#include "swanson/state.h"
#include "swanson/swanson0.h"

static void
swan_string_free(struct cork_gc *gc, void *vself)
{
    struct swan  *s = cork_container_of(gc, struct swan, gc);
    struct swan_string  *self = vself;
    if (self->value != NULL) {
        cork_free(swan_alloc(s), (void *) self->value, self->length);
    }
}

static struct cork_gc_obj_iface  swan_string_gc = {
    swan_string_free, NULL
};

struct swan_string *
swan_string_new(struct swan *s, const char *value, size_t length,
                struct cork_error *err)
{
    struct swan_string  *self = NULL;

    self = cork_gc_new(swan_gc(s), struct swan_string, &swan_string_gc);
    if (self == NULL) {
        goto error;
    }
    self->parent.cls = SWAN_STRING_CLASS;

    self->value = cork_malloc(swan_alloc(s), length);
    if (self->value == NULL) {
        goto error;
    }

    memcpy((void *) self->value, value, length);
    self->length = length;

    return self;

error:
    if (self != NULL) {
        cork_gc_decref(swan_gc(s), self);
    }

    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new string %s", value);
    return NULL;
}

bool
swan_string_equal(struct swan_string *s1, struct swan_string *s2)
{
    return (s1->length == s2->length) &&
        (memcmp(s1->value, s2->value, s1->length) == 0);
}
