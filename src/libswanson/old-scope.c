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

#include "swanson/state.h"
#include "swanson/swanson0.h"


/*-----------------------------------------------------------------------
 * Scopes
 */

static bool
swan_scope_comparator(const void *vk1, const void *vk2)
{
    const char  *k1 = vk1;
    const char  *k2 = vk2;
    return strcmp(k1, k2) == 0;
}

static cork_hash
swan_scope_hasher(const void *vk)
{
    const char  *k = vk;
    size_t  len = strlen(k);
    return cork_hash_buffer(0, k, len);
}

_recurse_(swan_scope) {
    struct swan_scope  *self = obj;
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;

    cork_hash_table_iterator_init(&self->entries, &iter);
    while ((entry = cork_hash_table_iterator_next(&iter)) != NULL) {
        recurse(gc, entry->value, ud);
    }

    recurse(gc, self->parent_scope, ud);
}

static enum cork_hash_table_map_result
swan_scope_free_entries(struct cork_hash_table_entry *entry, void *ud)
{
    cork_strfree(entry->key);
    /* We don't have to decref the value, since the recurse function
     * will do that for us. */
    return CORK_HASH_TABLE_MAP_DELETE;
}

_free_(swan_scope) {
    struct swan_scope  *self = obj;
    cork_hash_table_map
        (&self->entries, swan_scope_free_entries, NULL);
    cork_hash_table_done(&self->entries);
    if (self->name != NULL) {
        cork_strfree(self->name);
    }
}

_gc_(swan_scope);

struct swan_scope *
swan_scope_new(struct swan *s, const char *name, struct swan_scope *parent,
               struct cork_error *err)
{
    struct cork_gc  *gc = swan_gc(s);
    struct swan_scope  *self = NULL;
    e_check_gc_new(swan_scope, self, "scope");
    self->parent.cls = SWAN_SCOPE_CLASS;
    self->parent_scope = cork_gc_incref(swan_gc(s), parent);

    cork_hash_table_init
        (&self->entries, 0, swan_scope_hasher, swan_scope_comparator);
    e_check_alloc(self->name = cork_strdup(name), "scope name");
    return self;

error:
    if (self != NULL) {
        cork_gc_decref(swan_gc(s), self);
    }
    return NULL;
}

int
swan_scope_add(struct swan *s, struct swan_scope *self,
               const char *name, struct swan_obj *obj,
               struct cork_error *err)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry =
        cork_hash_table_get_or_create
        (&self->entries, (void *) name, &is_new, err);

    if (!is_new) {
        cork_error_set
            (err, SWAN_SCOPE_ERROR, SWAN_SCOPE_REDEFINED,
             "%s redefined in scope %s", name, self->name);
        cork_gc_decref(swan_gc(s), obj);
        return -1;
    }

    entry->key = (void *) cork_strdup(name);
    entry->value = obj;
    return 0;
}

static struct swan_obj *
swan_scope_get_(struct swan *s, const char *scope_name, struct swan_scope *self,
                const char *name, struct cork_error *err)
{
    struct swan_obj  *result =
        cork_hash_table_get(&self->entries, name);
    if (result == NULL) {
        if (self->parent_scope == NULL) {
            cork_error_set
                (err, SWAN_SCOPE_ERROR, SWAN_SCOPE_REDEFINED,
                 "No entry named %s in scope %s", name, self->name);
        } else {
            return swan_scope_get_(s, scope_name, self->parent_scope, name, err);
        }
    }
    return result;
}

struct swan_obj *
swan_scope_get(struct swan *s, struct swan_scope *self,
               const char *name, struct cork_error *err)
{
    return swan_scope_get_(s, self->name, self, name, err);
}
