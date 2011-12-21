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
#include <libcork/ds.h>

#include "swanson/state.h"
#include "swanson/swanson0.h"

/*-----------------------------------------------------------------------
 * Error handling
 */

struct swan_scope_error_extra {
    const char  *id;
    const char  *scope_name;
};

static int
swan_scope_redefined(struct cork_alloc *alloc, struct cork_error *err,
                     struct cork_buffer *dest)
{
    struct swan_scope_error_extra  *extra = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL, "%s redefined in scope %s",
         extra->id, extra->scope_name);
}

int
swan_scope_redefined_set(struct cork_alloc *alloc, struct cork_error *err,
                         const char *id, const char *scope_name)
{
    struct swan_scope_error_extra  extra = { id, scope_name };
    return cork_error_set_extra(alloc, err,
                                SWAN_SCOPE_ERROR,
                                SWAN_SCOPE_REDEFINED,
                                swan_scope_redefined,
                                extra);
}

static int
swan_scope_undefined(struct cork_alloc *alloc, struct cork_error *err,
                     struct cork_buffer *dest)
{
    struct swan_scope_error_extra  *extra = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL, "No entry named %s in scope %s",
         extra->id, extra->scope_name);
}

int
swan_scope_undefined_set(struct cork_alloc *alloc, struct cork_error *err,
                         const char *id, const char *scope_name)
{
    struct swan_scope_error_extra  extra = { id, scope_name };
    return cork_error_set_extra(alloc, err,
                                SWAN_SCOPE_ERROR,
                                SWAN_SCOPE_UNDEFINED,
                                swan_scope_undefined,
                                extra);
}


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

static void
swan_scope_recurse(struct cork_gc *gc, void *vself,
                   cork_gc_recurser recurse, void *ud)
{
    struct swan_scope  *self = vself;
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;

    cork_hash_table_iterator_init(&self->entries, &iter);
    while ((entry = cork_hash_table_iterator_next(&self->entries, &iter))
           != NULL) {
        recurse(gc, entry->value, ud);
    }

    recurse(gc, self->parent_scope, ud);
}

static enum cork_hash_table_map_result
swan_scope_free_entries(struct cork_alloc *alloc,
                        struct cork_hash_table_entry *entry, void *ud)
{
    cork_strfree(alloc, entry->key);
    /* We don't have to decref the value, since the recurse function
     * will do that for us. */
    return CORK_HASH_TABLE_MAP_DELETE;
}

static void
swan_scope_free(struct cork_gc *gc, void *vself)
{
    struct swan  *s = cork_container_of(gc, struct swan, gc);
    struct swan_scope  *self = vself;

    cork_hash_table_map
        (swan_alloc(s), &self->entries, swan_scope_free_entries, NULL);
    cork_hash_table_done(swan_alloc(s), &self->entries);
    if (self->name != NULL) {
        cork_strfree(swan_alloc(s), self->name);
    }
}

static struct cork_gc_obj_iface  swan_scope_gc = {
    swan_scope_free, swan_scope_recurse
};

struct swan_scope *
swan_scope_new(struct swan *s, const char *name, struct swan_scope *parent,
               struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);

    struct swan_scope  *self = NULL;
    e_check_gc_new(swan_scope, self, "scope");
    self->parent.cls = SWAN_SCOPE_CLASS;
    self->parent_scope = cork_gc_incref(swan_gc(s), parent);

    ei_check(cork_hash_table_init
             (alloc, &self->entries, 0,
              swan_scope_hasher, swan_scope_comparator, err));
    e_check_alloc(self->name = cork_strdup(swan_alloc(s), name), "scope name");
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
        (swan_alloc(s), &self->entries, (void *) name, &is_new, err);

    if (!is_new) {
        swan_scope_redefined_set(swan_alloc(s), err, name, self->name);
        cork_gc_decref(swan_gc(s), obj);
        return -1;
    }

    entry->key = (void *) cork_strdup(swan_alloc(s), name);
    entry->value = obj;
    return 0;
}

static struct swan_obj *
swan_scope_get_(struct swan *s, const char *scope_name, struct swan_scope *self,
                const char *name, struct cork_error *err)
{
    struct swan_obj  *result =
        cork_hash_table_get(swan_alloc(s), &self->entries, name);
    if (result == NULL) {
        if (self->parent_scope == NULL) {
            swan_scope_undefined_set(swan_alloc(s), err, name, self->name);
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
