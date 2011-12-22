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
#define S0_ERROR  0x4972f292

enum s0_error {
    S0_REDEFINITION,
    S0_SYNTAX_ERROR
};

int
s0_interface_redefinition_set(struct cork_alloc *alloc,
                              struct cork_error *err,
                              const char *id);

int
s0_recursive_redefinition_set(struct cork_alloc *alloc,
                              struct cork_error *err);


/*-----------------------------------------------------------------------
 * Types and type constructors
 */

enum s0_type_kind {
    S0_TYPE_RECURSIVE = 0,
    S0_TYPE_TYPE = 1,
    S0_TYPE_LITERAL = 2,
    S0_TYPE_FUNCTION = 3,
    S0_TYPE_LOCATION = 4,
    S0_TYPE_INTERFACE = 5,
    S0_TYPE_BLOCK = 6
};

struct s0_type {
    enum s0_type_kind  kind;
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


/*-----------------------------------------------------------------------
 * S0 AST
 */

/* All identifiers in S0 are simple numbers, with a single-character
 * prefix indicating the “kind” of entity:
 *
 *   %: types
 *   $: locals
 *   ^: upvalues
 *   !: function/macro parameters
 *   _: label
 */

typedef uintptr_t  s0_id;
typedef uintptr_t  s0_tagged_id;

enum s0_id_tag {
    S0_ID_TAG_TYPE = 0,
    S0_ID_TAG_LOCAL = 1,
    S0_ID_TAG_UPVALUE = 2,
    S0_ID_TAG_PARAM = 3,
    S0_ID_TAG_LABEL = 4
};

/* 3 bits are used for the tag, which gives us 8 possible tags */
#define s0_tagged_id(tag, id) (((tag) & 0x7) | ((id) << 3))
#define s0_tagged_id_tag(tid) ((tid) & 0x7)
#define s0_tagged_id_id(tid)  ((tid) >> 3)


#define S0_OPCODES(_) \
    _(TRECURSIVE, 0) \
    _(TLITERAL, 1) \
    _(TFUNCTION, 2) \
    _(TLOCATION, 3) \
    _(TINTERFACE, 4) \
    _(LITERAL, 5) \
    _(PRELUDE, 6) \
    _(GET, 7) \
    _(CALL, 8) \
    _(RETURN, 9) \
    _(LABEL, 10) \
    _(JTRUE, 11) \
    _(JFALSE, 12) \
    _(JUMP, 13) \

enum s0_opcode {
#define OPCODE_ENUM(name, val)  S0_##name = val,
    S0_OPCODES(OPCODE_ENUM)
    S0_OPCODE_COUNT
#undef OPCODE_ENUM
};

struct s0_instruction {
    enum s0_opcode  op;
    union {
        struct {
            s0_tagged_id  dest;
        } tliteral;
    } args;
    struct cork_dllist_item  siblings;
};

struct s0_instruction *
s0_tliteral_new(struct swan *s, s0_id dest, struct cork_error *err);


struct s0_basic_block {
    struct cork_dllist  body;
};

struct s0_basic_block *
s0_basic_block_new(struct swan *s, struct cork_error *err);

/* steals reference to instr */
int
s0_basic_block_add(struct swan *s, struct s0_basic_block *block,
                   struct s0_instruction *instr, struct cork_error *err);


/*-----------------------------------------------------------------------
 * Parsing an S0 file
 */

struct s0_position {
    size_t  line;
    size_t  column;
};

struct s0_basic_block *
s0_parse(struct swan *s, struct cork_slice *src, struct cork_error *err);


#endif  /* SWANSON_S0_H */
