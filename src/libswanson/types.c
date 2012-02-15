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
 * Types
 */

static enum cork_hash_table_map_result
s0_type_free_keys(struct cork_hash_table_entry *entry, void *ud)
{
    if (entry->key != NULL) {
        cork_strfree(entry->key);
    }
    return CORK_HASH_TABLE_MAP_DELETE;
}

_free_(s0_type) {
    struct s0_type  *self = obj;

    switch (self->kind) {
        case S0_TYPE_PRODUCT:
            cork_array_done(&self->_.product);
            break;

        case S0_TYPE_INTERFACE:
            /* The types in the hash table will be decref'ed
             * automatically by the recurse method.  We need to free the
             * keys, though, before freeing the hash table itself. */
            cork_hash_table_map
                (&self->_.interface.entries, s0_type_free_keys, NULL);
            cork_hash_table_done(&self->_.interface.entries);
            break;

        case S0_TYPE_CLASS:
            /* The values in the hash table will be decref'ed
             * automatically by the recurse method.  We need to free the
             * keys, though, before freeing the hash table itself. */
            cork_hash_table_map
                (&self->_.cls.entries, s0_type_free_keys, NULL);
            cork_hash_table_done(&self->_.cls.entries);
            cork_strfree(self->_.cls.name);
            break;

        default:
            break;
    }
}

_recurse_(s0_type) {
    struct s0_type  *self = obj;
    switch (self->kind) {
        case S0_TYPE_PRODUCT:
        {
            size_t  i;
            for (i = 0; i < cork_array_size(&self->_.product); i++) {
                struct s0_type  *element = cork_array_at(&self->_.product, i);
                recurse(gc, element, ud);
            }
            break;
        }

        case S0_TYPE_FUNCTION:
            recurse(gc, self->_.function.input, ud);
            recurse(gc, self->_.function.output, ud);
            break;

        case S0_TYPE_LOCATION:
            recurse(gc, self->_.location.referent, ud);
            break;

        case S0_TYPE_INTERFACE:
        {
            struct cork_hash_table_iterator  iter;
            struct cork_hash_table_entry  *entry;

            cork_hash_table_iterator_init(&self->_.interface.entries, &iter);
            while ((entry = cork_hash_table_iterator_next(&iter)) != NULL) {
                recurse(gc, entry->value, ud);
            }
            break;
        }

        case S0_TYPE_CLASS:
        {
            struct cork_hash_table_iterator  iter;
            struct cork_hash_table_entry  *entry;

            cork_hash_table_iterator_init(&self->_.cls.entries, &iter);
            while ((entry = cork_hash_table_iterator_next(&iter)) != NULL) {
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

_gc_(s0_type);


struct s0_type *
s0_type_type_new(struct swan *s)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    self->kind = S0_TYPE_TYPE;
    return self;
}

struct s0_type *
s0_literal_type_new(struct swan *s)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    self->kind = S0_TYPE_LITERAL;
    return self;
}

struct s0_type *
s0_any_type_new(struct swan *s)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    self->kind = S0_TYPE_ANY;
    return self;
}

struct s0_type *
s0_product_type_new(struct swan *s)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    self->kind = S0_TYPE_PRODUCT;
    cork_array_init(&self->_.product);
    return self;
}

int
s0_product_type_add(struct swan *s, struct s0_type *self,
                    struct s0_type *type)
{
    if (self->kind != S0_TYPE_PRODUCT) {
        cork_error_set
            (SWAN_GENERAL_ERROR, SWAN_GENERAL_BAD_TYPE,
             "Can only add elements to product types");
        return -1;
    }

    return cork_array_append
        (&self->_.product,
         cork_gc_incref(type));
}

struct s0_type *
s0_function_type_new(struct swan *s, struct s0_type *input,
                     struct s0_type *output)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    self->kind = S0_TYPE_FUNCTION;
    self->_.function.input = cork_gc_incref(input);
    self->_.function.output = cork_gc_incref(output);
    return self;
}

struct s0_type *
s0_location_type_new(struct swan *s, struct s0_type *referent)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    self->kind = S0_TYPE_LOCATION;
    self->_.location.referent = cork_gc_incref(referent);
    return self;
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
s0_interface_type_new(struct swan *s)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    self->kind = S0_TYPE_INTERFACE;
    cork_hash_table_init
        (&self->_.interface.entries, 0, string_hasher, string_comparator);
    return self;
}

int
s0_interface_type_add(struct swan *s, struct s0_type *self,
                      const char *name, struct s0_type *type)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry = NULL;

    if (self->kind != S0_TYPE_INTERFACE) {
        cork_error_set
            (SWAN_GENERAL_ERROR, SWAN_GENERAL_BAD_TYPE,
             "Can only add entries to interface types");
        return -1;
    }

    rip_check(entry = cork_hash_table_get_or_create
              (&self->_.interface.entries,
               (char *) name, &is_new));

    if (!is_new) {
        cork_error_set
            (S0_ERROR, S0_REDEFINED,
             "Interface already has an entry named \"%s\"", name);
        return -1;
    }

    entry->key = (char *) cork_strdup(name);
    entry->value = cork_gc_incref(type);
    return 0;
}

struct s0_type *
s0_class_type_new(struct swan *s, const char *name)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    memset(self, 0, sizeof(struct s0_type));
    self->kind = S0_TYPE_CLASS;
    self->_.cls.name = cork_strdup(name);
    cork_hash_table_init
        (&self->_.cls.entries, 0, string_hasher, string_comparator);
    return self;
}

int
s0_class_type_add(struct swan *s, struct s0_type *self,
                  const char *name, struct s0_value *value)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry = NULL;

    if (self->kind != S0_TYPE_CLASS) {
        cork_error_set
            (SWAN_GENERAL_ERROR, SWAN_GENERAL_BAD_TYPE,
             "Can only add entries to class types");
        return -1;
    }

    rip_check(entry = cork_hash_table_get_or_create
              (&self->_.cls.entries,
               (char *) name, &is_new));

    if (!is_new) {
        cork_error_set
            (S0_ERROR, S0_REDEFINED,
             "Class already has an entry named \"%s\"", name);
        return -1;
    }

    entry->key = (char *) cork_strdup(name);
    entry->value = cork_gc_incref(value);
    return 0;
}

struct s0_value *
s0_class_type_get(struct swan *s, struct s0_type *self,
                  const char *name)
{
    struct cork_hash_table_entry  *entry;

    if (self->kind != S0_TYPE_CLASS) {
        cork_error_set
            (SWAN_GENERAL_ERROR, SWAN_GENERAL_BAD_TYPE,
             "Can only get entries from class types");
        return NULL;
    }

    entry = cork_hash_table_get_entry
        (&self->_.cls.entries, name);
    if (entry == NULL) {
        cork_error_set
            (S0_ERROR, S0_UNDEFINED,
             "No entry named \"%s\" in class \"%s\"",
             name, self->_.cls.name);
        return NULL;
    }

    return entry->value;
}

struct s0_type *
s0_block_type_new(struct swan *s, struct s0_type *result)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    self->kind = S0_TYPE_BLOCK;
    self->_.block.result = cork_gc_incref(result);
    return self;
}

struct s0_type *
s0_recursive_type_new(struct swan *s)
{
    struct s0_type  *self = cork_gc_new(s0_type);
    self->kind = S0_TYPE_RECURSIVE;
    self->_.recursive.resolved = NULL;
    return self;
}

int
s0_recursive_type_resolve(struct swan *s, struct s0_type *self,
                          struct s0_type *resolved)
{
    if (self->kind != S0_TYPE_RECURSIVE) {
        cork_error_set
            (S0_ERROR, S0_REDEFINED,
             "Recursive type redefined");
        return -1;
    }

    if (self->_.recursive.resolved != NULL) {
        cork_error_set
            (S0_ERROR, S0_REDEFINED,
             "Recursive type redefined");
        return -1;
    }

    self->_.recursive.resolved = cork_gc_incref(resolved);
    return 0;
}
