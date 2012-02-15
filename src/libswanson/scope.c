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
#include <libcork/ds.h>

#include "swanson/s0.h"
#include "swanson/state.h"


/*-----------------------------------------------------------------------
 * Scopes
 */

static cork_hash
constant_hasher(const void *key)
{
    return (cork_hash) (uintptr_t) key;
}

static bool
constant_comparator(const void *key1, const void *key2)
{
    return ((uintptr_t) key1) == ((uintptr_t) key2);
}

_recurse_(s0_scope) {
    struct s0_scope  *self = obj;
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;

    cork_hash_table_iterator_init(&self->entries, &iter);
    while ((entry = cork_hash_table_iterator_next(&iter))
           != NULL) {
        recurse(gc, entry->value, ud);
    }
}

_free_(s0_scope) {
    struct s0_scope  *self = obj;
    cork_hash_table_done(&self->entries);
    if (self->name != NULL) {
        cork_strfree(self->name);
    }
}

_gc_(s0_scope);

struct s0_scope *
s0_scope_new(struct swan *s, const char *name)
{
    struct s0_scope  *self = cork_gc_new(s0_scope);
    cork_hash_table_init
        (&self->entries, 0, constant_hasher, constant_comparator);
    self->name = cork_strdup(name);
    return self;
}

int
s0_scope_add(struct swan *s, struct s0_scope *self,
             s0_tagged_id id, struct s0_value *value)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry =
        cork_hash_table_get_or_create
        (&self->entries, (void *) id, &is_new);

    if (!is_new) {
        cork_error_set
            (S0_ERROR, S0_REDEFINED,
             "%c%"PRIuPTR" redefined in scope %s",
             s0_id_tag_name(s0_tagged_id_tag(id)),
             s0_tagged_id_id(id), self->name);
        cork_gc_decref(value);
        return -1;
    }

    entry->key = (void *) id;
    entry->value = value;
    return 0;
}

struct s0_value *
s0_scope_get(struct swan *s, struct s0_scope *self,
             s0_tagged_id id)
{
    struct s0_value  *result =
        cork_hash_table_get(&self->entries, (void *) id);
    if (result == NULL) {
        cork_error_set
            (S0_ERROR, S0_UNDEFINED,
             "No entry named %c%"PRIuPTR" in scope %s",
             s0_id_tag_name(s0_tagged_id_tag(id)),
             s0_tagged_id_id(id), self->name);
        return NULL;
    } else {
        return result;
    }
}

struct s0_scope *
s0_scope_new_top_level(struct swan *s)
{
    /* Eventually we'll add the prelude and kernel to the scope here */
    return s0_scope_new(s, "<top-level>");
}
