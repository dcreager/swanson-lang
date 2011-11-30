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
#include "swanson/s0.h"
#include "swanson/state.h"


static void
s0_type_list_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct s0_type_list  *self = vself;
    recurse(self->head, ud);
    recurse(self->tail, ud);
}

static struct cork_gc_obj_iface  s0_type_list_gc = {
    NULL, s0_type_list_recurse
};

struct s0_type_list *
s0_type_list_new(struct swan *s, struct s0_type *head,
                 struct s0_type_list *tail, struct cork_error *err)
{
    struct s0_type_list  *self = NULL;
    e_pcheck(self = cork_gc_new(swan_gc(s), struct s0_type_list,
                                &s0_type_list_gc));
    self->head = cork_gc_incref(swan_gc(s), head);
    self->tail = tail;
    return self;

error:
    if (self == NULL) {
        cork_gc_decref(swan_gc(s), tail);
    } else {
        cork_gc_decref(swan_gc(s), self);
    }

    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new tagged ID list");
    return NULL;
}


static struct cork_gc_obj_iface  s0_literal_type_gc = { NULL, NULL };

struct s0_type *
s0_literal_type_new(struct swan *s, struct cork_error *err)
{
    struct s0_literal_type  *self = NULL;
    e_pcheck(self = cork_gc_new(swan_gc(s), struct s0_literal_type,
                                &s0_literal_type_gc));
    self->parent.kind = S0_KIND_LITERAL;
    return &self->parent;

error:
    cork_gc_decref(swan_gc(s), self);
    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new literal type");
    return NULL;
}


static void
s0_function_type_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct s0_function_type  *self = vself;
    recurse(self->params, ud);
    recurse(self->results, ud);
}

static struct cork_gc_obj_iface  s0_function_type_gc = {
    NULL, s0_function_type_recurse
};

struct s0_type *
s0_function_type_new(struct swan *s,
                     struct s0_type_list *params, struct s0_type_list *results,
                     struct cork_error *err)
{
    struct s0_function_type  *self = NULL;
    e_pcheck(self = cork_gc_new(swan_gc(s), struct s0_function_type,
                                &s0_function_type_gc));
    self->parent.kind = S0_KIND_FUNCTION;
    self->params = params;
    self->results = results;
    return &self->parent;

error:
    if (self == NULL) {
        cork_gc_decref(swan_gc(s), params);
        cork_gc_decref(swan_gc(s), results);
    } else {
        cork_gc_decref(swan_gc(s), self);
    }

    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new function type");
    return NULL;
}


static void
s0_location_type_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct s0_location_type  *self = vself;
    recurse(self->referent, ud);
}

static struct cork_gc_obj_iface  s0_location_type_gc = {
    NULL, s0_location_type_recurse
};

struct s0_type *
s0_location_type_new(struct swan *s, struct s0_type *referent,
                     struct cork_error *err)
{
    struct s0_location_type  *self = NULL;
    e_pcheck(self = cork_gc_new(swan_gc(s), struct s0_location_type,
                                &s0_location_type_gc));
    self->parent.kind = S0_KIND_LOCATION;
    self->referent = cork_gc_incref(swan_gc(s), referent);
    return &self->parent;

error:
    cork_gc_decref(swan_gc(s), self);
    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new location type");
    return NULL;
}


static enum cork_hash_table_map_result
s0_interface_type_free_keys(struct cork_hash_table_entry *entry, void *ud)
{
    struct cork_alloc  *alloc = ud;
    if (entry->key != NULL) {
        cork_strfree(alloc, entry->key);
    }
    return CORK_HASH_TABLE_MAP_DELETE;
}

static void
s0_interface_type_free(struct cork_gc *gc, void *vself)
{
    struct s0_interface_type  *self = vself;

    /* The types in the hash table will be decref'ed automatically by
     * the recurse method.  We need to free the keys, though, before
     * freeing the hash table itself. */
    cork_hash_table_map(&self->entries, s0_interface_type_free_keys,
                        gc->alloc);
    cork_hash_table_done(&self->entries);
}

static void
s0_interface_type_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct s0_interface_type  *self = vself;
    struct cork_hash_table_iterator  iter;
    struct cork_hash_table_entry  *entry;

    cork_hash_table_iterator_init(&self->entries, &iter);
    while ((entry = cork_hash_table_iterator_next(&self->entries, &iter))
           != NULL) {
        recurse(entry->value, ud);
    }
}

static struct cork_gc_obj_iface  s0_interface_type_gc = {
    s0_interface_type_free, s0_interface_type_recurse
};

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
    struct s0_interface_type  *self = NULL;
    e_pcheck(self = cork_gc_new(swan_gc(s), struct s0_interface_type,
                                &s0_interface_type_gc));
    self->parent.kind = S0_KIND_INTERFACE;
    cork_hash_table_init(swan_alloc(s), &self->entries, 0,
                         string_hasher, string_comparator);
    return &self->parent;

error:
    cork_gc_decref(swan_gc(s), self);
    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new interface type");
    return NULL;
}

int
s0_interface_type_add(struct swan *s, struct s0_type *self,
                      const char *name, struct s0_type *type,
                      struct cork_error *err)
{
    struct s0_interface_type  *iself;
    bool  is_new;
    struct cork_hash_table_entry  *entry = NULL;

    if (self->kind != S0_KIND_INTERFACE) {
        cork_error_set(err, SWAN_GENERAL_ERROR,
                       SWAN_GENERAL_ERROR_BAD_TYPE,
                       "Cannot only add entries to interface types");
        return -1;
    }

    iself = cork_container_of(self, struct s0_interface_type, parent);
    e_pcheck(entry = cork_hash_table_get_or_create
             (&iself->entries, (char *) name, &is_new));

    if (!is_new) {
        cork_error_set(err, SWAN_S0_ERROR,
                       SWAN_S0_ERROR_REDEFINITION,
                       "Interface already has an entry named \"%s\"",
                       name);
        return -1;
    }

    e_pcheck(entry->key = (char *) cork_strdup(swan_alloc(s), name));
    entry->value = cork_gc_incref(swan_gc(s), type);
    return 0;

error:
    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new interface entry");
    return -1;
}


static void
s0_recursive_type_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct s0_recursive_type  *self = vself;
    recurse(self->resolved, ud);
}

static struct cork_gc_obj_iface  s0_recursive_type_gc = {
    NULL, s0_recursive_type_recurse
};

struct s0_type *
s0_recursive_type_new(struct swan *s, struct cork_error *err)
{
    struct s0_recursive_type  *self = NULL;
    e_pcheck(self = cork_gc_new(swan_gc(s), struct s0_recursive_type,
                                &s0_recursive_type_gc));
    self->parent.kind = S0_KIND_RECURSIVE;
    self->resolved = NULL;
    return &self->parent;

error:
    cork_gc_decref(swan_gc(s), self);
    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new recursive type");
    return NULL;
}

int
s0_recursive_type_resolve(struct swan *s, struct s0_type *self,
                          struct s0_type *resolved, struct cork_error *err)
{
    struct s0_recursive_type  *rself;

    if (self->kind != S0_KIND_RECURSIVE) {
        cork_error_set(err, SWAN_GENERAL_ERROR,
                       SWAN_GENERAL_ERROR_BAD_TYPE,
                       "Cannot resolve non-recursive type");
        return -1;
    }

    rself = cork_container_of(self, struct s0_recursive_type, parent);
    if (rself->resolved != NULL) {
        cork_error_set(err, SWAN_S0_ERROR,
                       SWAN_S0_ERROR_REDEFINITION,
                       "Recursive type redefined");
        return -1;
    }

    rself->resolved = cork_gc_incref(swan_gc(s), resolved);
    return 0;
}
