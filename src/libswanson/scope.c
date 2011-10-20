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
#include <libcork/ds.h>

#include "swanson/checkers.h"
#include "swanson/state.h"
#include "swanson/swanson0.h"

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

struct recurse_state {
    cork_gc_recurser  recurse;
    void  *ud;
};

static enum cork_hash_table_map_result
swan_scope_recurse_entries(struct cork_hash_table_entry *entry, void *ud)
{
    struct recurse_state  *state = ud;
    state->recurse(entry->value, state->ud);
    return CORK_HASH_TABLE_MAP_CONTINUE;
}

static void
swan_scope_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct swan_scope  *self = vself;
    struct recurse_state  state = { recurse, ud };
    cork_hash_table_map(&self->entries, swan_scope_recurse_entries, &state);
}

static enum cork_hash_table_map_result
swan_scope_free_entries(struct cork_hash_table_entry *entry, void *ud)
{
    struct swan  *s = ud;
    cork_strfree(swan_alloc(s), entry->key);
    /* We don't have to decref the value, since the recurse function
     * will do that for us. */
    return CORK_HASH_TABLE_MAP_DELETE;
}

static void
swan_scope_free(struct cork_gc *gc, void *vself)
{
    struct swan  *s = cork_container_of(gc, struct swan, gc);
    struct swan_scope  *self = vself;

    cork_hash_table_map(&self->entries, swan_scope_free_entries, s);
    cork_hash_table_done(&self->entries);
    if (self->name != NULL) {
        cork_strfree(swan_alloc(s), self->name);
    }
}

static struct cork_gc_obj_iface  swan_scope_gc = {
    swan_scope_free, swan_scope_recurse
};

struct swan_scope *
swan_scope_new(struct swan *s, const char *name, struct cork_error *err)
{
    struct swan_scope  *self = NULL;

    self = cork_gc_new(swan_gc(s), struct swan_scope, &swan_scope_gc);
    if (self == NULL) {
        goto error;
    }
    self->parent.cls = SWAN_SCOPE_CLASS;

    e_bcheck(cork_hash_table_init
             (swan_alloc(s), &self->entries, 0,
              swan_scope_hasher, swan_scope_comparator));

    self->name = cork_strdup(swan_alloc(s), name);
    if (self->name == NULL) {
        goto error;
    }

    return self;

error:
    if (self != NULL) {
        cork_gc_decref(swan_gc(s), self);
    }

    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new scope %s", name);
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
        (&self->entries, (void *) name, &is_new);

    if (!is_new) {
        cork_error_set(err, SWAN_SCOPE_ERROR,
                       SWAN_SCOPE_ERROR_REDEFINED,
                       "%s redefined in scope %s",
                       name, self->name);
        cork_gc_decref(swan_gc(s), obj);
        return -1;
    }

    entry->key = (void *) cork_strdup(swan_alloc(s), name);
    entry->value = obj;
    return 0;
}

struct swan_obj *
swan_scope_get(struct swan *s, struct swan_scope *self,
               const char *name, struct cork_error *err)
{
    struct swan_obj  *result =
        cork_hash_table_get(&self->entries, name);
    if (result == NULL) {
        cork_error_set(err, SWAN_SCOPE_ERROR,
                       SWAN_SCOPE_ERROR_UNDEFINED,
                       "No entry named %s in scope %s",
                       name, self->name);
    }
    return result;
}
