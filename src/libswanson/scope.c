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

#include "swanson/s0.h"
#include "swanson/state.h"

/*-----------------------------------------------------------------------
 * Error handling
 */

struct s0_scope_error_extra {
    s0_tagged_id  id;
    const char  *scope_name;
};

static int
s0_scope_redefined(struct cork_alloc *alloc, struct cork_error *err,
                   struct cork_buffer *dest)
{
    struct s0_scope_error_extra  *extra = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL, "%c%"PRIuPTR" redefined in scope %s",
         s0_id_tag_name(s0_tagged_id_tag(extra->id)),
         s0_tagged_id_id(extra->id), extra->scope_name);
}

int
s0_scope_redefined_set(struct cork_alloc *alloc, struct cork_error *err,
                       s0_tagged_id id, const char *scope_name)
{
    struct s0_scope_error_extra  extra = { id, scope_name };
    return cork_error_set_extra(alloc, err,
                                S0_ERROR,
                                S0_REDEFINED,
                                s0_scope_redefined,
                                extra);
}

static int
s0_scope_undefined(struct cork_alloc *alloc, struct cork_error *err,
                   struct cork_buffer *dest)
{
    struct s0_scope_error_extra  *extra = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL, "No entry named %c%"PRIuPTR" in scope %s",
         s0_id_tag_name(s0_tagged_id_tag(extra->id)),
         s0_tagged_id_id(extra->id), extra->scope_name);
}

int
s0_scope_undefined_set(struct cork_alloc *alloc, struct cork_error *err,
                       s0_tagged_id id, const char *scope_name)
{
    struct s0_scope_error_extra  extra = { id, scope_name };
    return cork_error_set_extra(alloc, err,
                                S0_ERROR,
                                S0_UNDEFINED,
                                s0_scope_undefined,
                                extra);
}


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

static void
s0_scope_recurse(struct cork_gc *gc, void *vself,
                 cork_gc_recurser recurse, void *ud)
{
    struct s0_scope  *self = vself;
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;

    cork_hash_table_iterator_init(&self->entries, &iter);
    while ((entry = cork_hash_table_iterator_next(&self->entries, &iter))
           != NULL) {
        recurse(gc, entry->value, ud);
    }
}

static void
s0_scope_free(struct cork_gc *gc, void *vself)
{
    struct swan  *s = cork_container_of(gc, struct swan, gc);
    struct s0_scope  *self = vself;

    cork_hash_table_done(swan_alloc(s), &self->entries);
    if (self->name != NULL) {
        cork_strfree(swan_alloc(s), self->name);
    }
}

static struct cork_gc_obj_iface  s0_scope_gc = {
    s0_scope_free, s0_scope_recurse
};

struct s0_scope *
s0_scope_new(struct swan *s, const char *name, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_scope  *self = NULL;
    rp_check_gc_new(s0_scope, self, "scope");

    ei_check(cork_hash_table_init
             (alloc, &self->entries, 0,
              constant_hasher, constant_comparator, err));
    e_check_alloc(self->name = cork_strdup(swan_alloc(s), name), "scope name");
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

int
s0_scope_add(struct swan *s, struct s0_scope *self,
             s0_tagged_id id, struct s0_value *value,
             struct cork_error *err)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry =
        cork_hash_table_get_or_create
        (swan_alloc(s), &self->entries, (void *) id, &is_new, err);

    if (!is_new) {
        s0_scope_redefined_set(swan_alloc(s), err, id, self->name);
        cork_gc_decref(swan_gc(s), value);
        return -1;
    }

    entry->key = (void *) id;
    entry->value = value;
    return 0;
}

struct s0_value *
s0_scope_get(struct swan *s, struct s0_scope *self,
             s0_tagged_id id, struct cork_error *err)
{
    struct s0_value  *result =
        cork_hash_table_get(swan_alloc(s), &self->entries, (void *) id);
    if (result == NULL) {
        s0_scope_undefined_set(swan_alloc(s), err, id, self->name);
        return NULL;
    } else {
        return result;
    }
}

struct s0_scope *
s0_scope_new_top_level(struct swan *s, struct cork_error *err)
{
    /* Eventually we'll add the prelude and kernel to the scope here */
    return s0_scope_new(s, "<top-level>", err);
}
