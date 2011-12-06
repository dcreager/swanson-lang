/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_S0_H
#define SWANSON_S0_H


#include <libcork/core.h>
#include <libcork/ds.h>

#include <swanson/state.h>


/*-----------------------------------------------------------------------
 * Error handling
 */

/* Hash of "swanson.s0" */
#define SWAN_S0_ERROR  0x4972f292

enum swan_s0_error {
    SWAN_S0_REDEFINITION
};

int
swan_s0_interface_redefinition_set(struct cork_alloc *alloc,
                                   struct cork_error *err,
                                   const char *id);

int
swan_s0_recursive_redefinition_set(struct cork_alloc *alloc,
                                   struct cork_error *err);


/*-----------------------------------------------------------------------
 * Types and type constructors
 */

enum s0_kind {
    S0_KIND_LITERAL = 0,
    S0_KIND_FUNCTION = 1,
    S0_KIND_LOCATION = 2,
    S0_KIND_INTERFACE = 3,
    S0_KIND_BLOCK = 4,
    S0_KIND_RECURSIVE = 5
};

struct s0_type {
    enum s0_kind  kind;
};


struct s0_type_list {
    struct s0_type  *head;
    struct s0_type_list  *tail;
};

/* Creates new reference to head; steals reference to tail */
struct s0_type_list *
s0_type_list_new(struct swan *s, struct s0_type *head,
                 struct s0_type_list *tail, struct cork_error *err);


struct s0_literal_type {
    struct s0_type  parent;
};

struct s0_type *
s0_literal_type_new(struct swan *s, struct cork_error *err);


struct s0_function_type {
    struct s0_type  parent;
    struct s0_type_list  *params;
    struct s0_type_list  *results;
};

/* Steals references to params and results */
struct s0_type *
s0_function_type_new(struct swan *s,
                     struct s0_type_list *params, struct s0_type_list *results,
                     struct cork_error *err);


struct s0_location_type {
    struct s0_type  parent;
    struct s0_type  *referent;
};

/* Creates new reference to referent */
struct s0_type *
s0_location_type_new(struct swan *s, struct s0_type *referent,
                     struct cork_error *err);


struct s0_interface_type {
    struct s0_type  parent;
    struct cork_hash_table  entries;
};

struct s0_type *
s0_interface_type_new(struct swan *s, struct cork_error *err);

/* Creates new reference to type */
int
s0_interface_type_add(struct swan *s, struct s0_type *self,
                      const char *name, struct s0_type *type,
                      struct cork_error *err);


struct s0_block_type {
    struct s0_type  parent;
    struct s0_type  *result;
};


/* Creates new reference to referent */
struct s0_type *
s0_block_type_new(struct swan *s, struct s0_type *result,
                  struct cork_error *err);


/* This is a placeholder for a recursive reference within the definition
 * of a type.  The resolved field starts off NULL.  Once the recursive
 * type is fully defined, you use s0_recursive_type_resolve to fill in
 * the recursive target. */

struct s0_recursive_type {
    struct s0_type  parent;
    struct s0_type  *resolved;
};

struct s0_type *
s0_recursive_type_new(struct swan *s, struct cork_error *err);

/* Creates new reference to resolved */
int
s0_recursive_type_resolve(struct swan *s, struct s0_type *self,
                          struct s0_type *resolved, struct cork_error *err);


/*-----------------------------------------------------------------------
 * Pretty-printing a type
 */

int
s0_type_print(struct swan *s, struct s0_type *type,
              struct cork_buffer *dest, struct cork_buffer *givens,
              struct cork_error *err);


#endif  /* SWANSON_S0_H */
