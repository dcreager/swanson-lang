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


/*-----------------------------------------------------------------------
 * Error handling
 */

static int
swan_general_bad_type(struct cork_alloc *alloc, struct cork_error *err,
                      struct cork_buffer *dest)
{
    const char  **reason = cork_error_extra(err);
    return cork_buffer_set_string(alloc, dest, *reason, NULL);
}

int
swan_general_bad_type_set(struct cork_alloc *alloc, struct cork_error *err,
                          const char *reason)
{
    return cork_error_set_extra(alloc, err,
                                SWAN_GENERAL_ERROR,
                                SWAN_GENERAL_BAD_TYPE,
                                swan_general_bad_type,
                                reason);
}

static int
s0_interface_redefinition(struct cork_alloc *alloc,
                               struct cork_error *err,
                               struct cork_buffer *dest)
{
    const char  **id = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL,
         "Interface already has an entry named \"%s\"", *id);
}

int
s0_interface_redefinition_set(struct cork_alloc *alloc,
                                   struct cork_error *err,
                                   const char *id)
{
    return cork_error_set_extra(alloc, err,
                                S0_ERROR,
                                S0_REDEFINED,
                                s0_interface_redefinition,
                                id);
}

static int
s0_recursive_redefinition(struct cork_alloc *alloc,
                               struct cork_error *err,
                               struct cork_buffer *dest)
{
    return cork_buffer_set_string
        (alloc, dest, "Recursive type redefined", NULL);
}

int
s0_recursive_redefinition_set(struct cork_alloc *alloc,
                                   struct cork_error *err)
{
    return cork_error_set(alloc, err,
                          S0_ERROR,
                          S0_REDEFINED,
                          s0_recursive_redefinition);
}


/*-----------------------------------------------------------------------
 * Type lists
 */

static void
s0_type_list_recurse(struct cork_gc *gc, void *vself,
                     cork_gc_recurser recurse, void *ud)
{
    struct s0_type_list  *self = vself;
    recurse(gc, self->head, ud);
    recurse(gc, self->tail, ud);
}

static struct cork_gc_obj_iface  s0_type_list_gc = {
    NULL, s0_type_list_recurse
};

struct s0_type_list *
s0_type_list_new(struct swan *s, struct s0_type *head,
                 struct s0_type_list *tail, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);

    struct s0_type_list  *self = NULL;
    e_check_gc_new(s0_type_list, self, "type list");
    self->head = cork_gc_incref(swan_gc(s), head);
    self->tail = tail;
    return self;

error:
    if (self == NULL) {
        cork_gc_decref(swan_gc(s), tail);
    } else {
        cork_gc_decref(swan_gc(s), self);
    }

    return NULL;
}


/*-----------------------------------------------------------------------
 * Types
 */

static enum cork_hash_table_map_result
s0_type_free_keys(struct cork_alloc *alloc,
                  struct cork_hash_table_entry *entry, void *ud)
{
    if (entry->key != NULL) {
        cork_strfree(alloc, entry->key);
    }
    return CORK_HASH_TABLE_MAP_DELETE;
}

static void
s0_type_free(struct cork_gc *gc, void *vself)
{
    struct s0_type  *self = vself;

    switch (self->kind) {
        case S0_TYPE_INTERFACE:
            /* The types in the hash table will be decref'ed
             * automatically by the recurse method.  We need to free the
             * keys, though, before freeing the hash table itself. */
            cork_hash_table_map
                (gc->alloc, &self->_.interface.entries,
                 s0_type_free_keys, NULL);
            cork_hash_table_done(gc->alloc, &self->_.interface.entries);
            break;

        default:
            break;
    }
}

static void
s0_type_recurse(struct cork_gc *gc, void *vself,
                cork_gc_recurser recurse, void *ud)
{
    struct s0_type  *self = vself;
    switch (self->kind) {
        case S0_TYPE_FUNCTION:
            recurse(gc, self->_.function.params, ud);
            recurse(gc, self->_.function.results, ud);
            break;

        case S0_TYPE_LOCATION:
            recurse(gc, self->_.location.referent, ud);
            break;

        case S0_TYPE_INTERFACE:
        {
            struct cork_hash_table_iterator  iter;
            struct cork_hash_table_entry  *entry;

            cork_hash_table_iterator_init(&self->_.interface.entries, &iter);
            while ((entry = cork_hash_table_iterator_next
                        (&self->_.interface.entries, &iter)) != NULL) {
                recurse(gc, entry->value, ud);
            }
            break;
        }

        case S0_TYPE_BLOCK:
            recurse(gc, self->_.block.result, ud);
            break;

        case S0_TYPE_RECURSIVE:
            recurse(gc, self->_.recursive.resolved, ud);
            break;

        default:
            break;
    }
}

static struct cork_gc_obj_iface  s0_type_gc = {
    s0_type_free, s0_type_recurse
};


struct s0_type *
s0_literal_type_new(struct swan *s, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_type  *self = NULL;
    e_check_gc_new(s0_type, self, "literal type");
    self->kind = S0_TYPE_LITERAL;
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

struct s0_type *
s0_function_type_new(struct swan *s,
                     struct s0_type_list *params, struct s0_type_list *results,
                     struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_type  *self = NULL;
    e_check_gc_new(s0_type, self, "function type");
    self->kind = S0_TYPE_FUNCTION;
    self->_.function.params = params;
    self->_.function.results = results;
    return self;

error:
    if (self == NULL) {
        cork_gc_decref(swan_gc(s), params);
        cork_gc_decref(swan_gc(s), results);
    } else {
        cork_gc_decref(swan_gc(s), self);
    }

    return NULL;
}

struct s0_type *
s0_location_type_new(struct swan *s, struct s0_type *referent,
                     struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_type  *self = NULL;
    e_check_gc_new(s0_type, self, "location type");
    self->kind = S0_TYPE_LOCATION;
    self->_.location.referent = cork_gc_incref(swan_gc(s), referent);
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

static bool
string_comparator(const void *vk1, const void *vk2)
{
    const char  *k1 = vk1;
    const char  *k2 = vk2;
    return strcmp(k1, k2) == 0;
}

static cork_hash
string_hasher(const void *vk)
{
    const char  *k = vk;
    size_t  len = strlen(k);
    return cork_hash_buffer(0, k, len);
}

struct s0_type *
s0_interface_type_new(struct swan *s, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_type  *self = NULL;
    e_check_gc_new(s0_type, self, "interface type");
    self->kind = S0_TYPE_INTERFACE;
    ei_check(cork_hash_table_init
             (swan_alloc(s), &self->_.interface.entries, 0,
              string_hasher, string_comparator, err));
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

int
s0_interface_type_add(struct swan *s, struct s0_type *self,
                      const char *name, struct s0_type *type,
                      struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    bool  is_new;
    struct cork_hash_table_entry  *entry = NULL;

    if (self->kind != S0_TYPE_INTERFACE) {
        swan_general_bad_type_set
            (swan_alloc(s), err, "Can only add entries to interface types");
        return -1;
    }

    rip_check(entry = cork_hash_table_get_or_create
              (swan_alloc(s), &self->_.interface.entries,
               (char *) name, &is_new, err));

    if (!is_new) {
        s0_interface_redefinition_set(swan_alloc(s), err, name);
        return -1;
    }

    ri_check_alloc(entry->key = (char *) cork_strdup(swan_alloc(s), name),
                   "interface entry name");
    entry->value = cork_gc_incref(swan_gc(s), type);
    return 0;
}

struct s0_type *
s0_block_type_new(struct swan *s, struct s0_type *result,
                  struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_type  *self = NULL;
    e_check_gc_new(s0_type, self, "block type");
    self->kind = S0_TYPE_BLOCK;
    self->_.block.result = cork_gc_incref(swan_gc(s), result);
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

struct s0_type *
s0_recursive_type_new(struct swan *s, struct cork_error *err)
{
    struct cork_alloc  *alloc = swan_alloc(s);
    struct cork_gc  *gc = swan_gc(s);
    struct s0_type  *self = NULL;
    e_check_gc_new(s0_type, self, "recursive type");
    self->kind = S0_TYPE_RECURSIVE;
    self->_.recursive.resolved = NULL;
    return self;

error:
    cork_gc_decref(swan_gc(s), self);
    return NULL;
}

int
s0_recursive_type_resolve(struct swan *s, struct s0_type *self,
                          struct s0_type *resolved, struct cork_error *err)
{
    if (self->kind != S0_TYPE_RECURSIVE) {
        swan_general_bad_type_set
            (swan_alloc(s), err, "Cannot resolve non-recursive type");
        return -1;
    }

    if (self->_.recursive.resolved != NULL) {
        s0_recursive_redefinition_set(swan_alloc(s), err);
        return -1;
    }

    self->_.recursive.resolved = cork_gc_incref(swan_gc(s), resolved);
    return 0;
}
