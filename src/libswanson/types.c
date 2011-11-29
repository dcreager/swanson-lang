/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

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


static void
s0_interface_entry_free(struct cork_gc *gc, void *vself)
{
    struct s0_interface_entry  *self = vself;
    if (self->name != NULL) {
        cork_strfree(gc->alloc, self->name);
    }
}

static void
s0_interface_entry_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct s0_interface_entry  *self = vself;
    recurse(self->type, ud);
}

static struct cork_gc_obj_iface  s0_interface_entry_gc = {
    s0_interface_entry_free, s0_interface_entry_recurse
};

struct s0_interface_entry *
s0_interface_entry_new(struct swan *s, const char *name, struct s0_type *type,
                       struct s0_interface_entry *tail, struct cork_error *err)
{
    const char  *copied_name = NULL;
    struct s0_interface_entry  *self = NULL;

    e_pcheck(copied_name = cork_strdup(swan_alloc(s), name));
    e_pcheck(self = cork_gc_new(swan_gc(s), struct s0_interface_entry,
                                &s0_interface_entry_gc));
    self->name = copied_name;
    self->type = cork_gc_incref(swan_gc(s), type);
    self->tail = tail;
    return self;

error:
    if (self == NULL) {
        cork_gc_decref(swan_gc(s), tail);
        if (copied_name != NULL) {
            cork_strfree(swan_alloc(s), copied_name);
        }
    } else {
        cork_gc_decref(swan_gc(s), self);
    }

    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new interface entry");
    return NULL;
}


static void
s0_interface_type_recurse(void *vself, cork_gc_recurser recurse, void *ud)
{
    struct s0_interface_type  *self = vself;
    recurse(self->entries, ud);
}

static struct cork_gc_obj_iface  s0_interface_type_gc = {
    NULL, s0_interface_type_recurse
};

struct s0_type *
s0_interface_type_new(struct swan *s, struct s0_interface_entry *entries,
                      struct cork_error *err)
{
    struct s0_interface_type  *self = NULL;
    e_pcheck(self = cork_gc_new(swan_gc(s), struct s0_interface_type,
                                &s0_interface_type_gc));
    self->parent.kind = S0_KIND_INTERFACE;
    self->entries = entries;
    return &self->parent;

error:
    if (self == NULL) {
        cork_gc_decref(swan_gc(s), entries);
    } else {
        cork_gc_decref(swan_gc(s), self);
    }
    cork_error_set(err, SWAN_GENERAL_ERROR,
                   SWAN_GENERAL_ERROR_CANNOT_ALLOCATE,
                   "Cannot allocate new interface type");
    return NULL;
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
                       SWAN_GENERAL_ERROR_UNKNOWN,
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
