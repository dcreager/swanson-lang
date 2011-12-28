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


/* forward declarations */

struct s0_type;
typedef cork_array(struct s0_type *)  s0_type_array;

struct s0_value;
typedef cork_array(struct s0_value *)  s0_value_array;

struct s0_scope;

struct s0_instruction;
typedef cork_array(struct s0_instruction *)  s0_instruction_array;


/*-----------------------------------------------------------------------
 * Types and type constructors
 */

enum s0_type_kind {
    S0_TYPE_RECURSIVE,
    S0_TYPE_TYPE,
    S0_TYPE_LITERAL,
    S0_TYPE_FUNCTION,
    S0_TYPE_LOCATION,
    S0_TYPE_INTERFACE,
    S0_TYPE_CLASS,
    S0_TYPE_BLOCK
};

struct s0_type {
    enum s0_type_kind  kind;
    union {
        struct {
            s0_type_array  params;
            s0_type_array  results;
        } function;
        struct { struct s0_type  *referent; } location;
        struct { struct cork_hash_table  entries; } interface;
        struct {
            const char  *name;
            struct cork_hash_table  entries;
        } cls;
        struct { struct s0_type  *result; } block;

        /* This is a placeholder for a recursive reference within the
         * definition of a type.  The resolved field starts off NULL.
         * Once the recursive type is fully defined, you use
         * s0_recursive_type_resolve to fill in the recursive target. */
        struct { struct s0_type  *resolved; } recursive;
    } _;
};

struct s0_type *
s0_literal_type_new(struct swan *s, struct cork_error *err);

struct s0_type *
s0_type_type_new(struct swan *s, struct cork_error *err);

struct s0_type *
s0_function_type_new(struct swan *s, struct cork_error *err);

/* Creates new reference to type */
int
s0_function_type_add_param(struct swan *s, struct s0_type *self,
                           struct s0_type *type, struct cork_error *err);

/* Creates new reference to type */
int
s0_function_type_add_result(struct swan *s, struct s0_type *self,
                            struct s0_type *type, struct cork_error *err);

/* Creates new reference to referent */
struct s0_type *
s0_location_type_new(struct swan *s, struct s0_type *referent,
                     struct cork_error *err);

struct s0_type *
s0_interface_type_new(struct swan *s, struct cork_error *err);

/* Creates new reference to type */
int
s0_interface_type_add(struct swan *s, struct s0_type *self,
                      const char *name, struct s0_type *type,
                      struct cork_error *err);

struct s0_type *
s0_class_type_new(struct swan *s, const char *name, struct cork_error *err);

/* Creates new reference to value */
int
s0_class_type_add(struct swan *s, struct s0_type *self,
                  const char *name, struct s0_value *value,
                  struct cork_error *err);

/* Creates new reference to referent */
struct s0_type *
s0_block_type_new(struct swan *s, struct s0_type *result,
                  struct cork_error *err);

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
    S0_ID_TAG_TYPE,
    S0_ID_TAG_LOCAL,
    S0_ID_TAG_UPVALUE,
    S0_ID_TAG_PARAM,
    S0_ID_TAG_LABEL
};

#define s0_id_tag_name(tag) \
    (((tag) == S0_ID_TAG_TYPE)?    '%': \
     ((tag) == S0_ID_TAG_LOCAL)?   '$': \
     ((tag) == S0_ID_TAG_UPVALUE)? '^': \
     ((tag) == S0_ID_TAG_PARAM)?   '!': \
     ((tag) == S0_ID_TAG_LABEL)?   '_': \
      '?')

/* 3 bits are used for the tag, which gives us 8 possible tags */
#define s0_tagged_id(tag, id) (((tag) & 0x7) | ((id) << 3))
#define s0_tagged_id_tag(tid) ((tid) & 0x7)
#define s0_tagged_id_id(tid)  ((tid) >> 3)

typedef cork_array(s0_tagged_id)  s0_tagged_id_array;


#define S0_OPCODES(_) \
    _(TRECURSIVE) \
    _(TLITERAL) \
    _(TFUNCTION) \
    _(TLOCATION) \
    _(TINTERFACE) \
    _(TCLASS) \
    _(TBLOCK) \
    _(TTYPE) \
    _(LITERAL) \
    _(MACRO) \
    _(CALL) \
    _(RETURN) \

#if 0
    _(GET) \
    _(LABEL) \
    _(JTRUE) \
    _(JFALSE) \
    _(JUMP) \

#endif

enum s0_opcode {
#define OPCODE_ENUM(name)  S0_##name,
    S0_OPCODES(OPCODE_ENUM)
    S0_OPCODE_COUNT
#undef OPCODE_ENUM
};

struct s0_tinterface_entry {
    const char  *key;
    s0_tagged_id  entry;
};

typedef cork_array(struct s0_tinterface_entry)  s0_tinterface_entries;

struct s0_instruction {
    enum s0_opcode  op;
    s0_tagged_id  dest;
    union {
        struct {
            s0_tagged_id_array  params;
            s0_tagged_id_array  results;
        } tfunction;
        struct { s0_tagged_id  referent; }  tlocation;
        struct { s0_tinterface_entries  entries; }  tinterface;
        struct {
            const char  *name;
            s0_tinterface_entries  entries;
        } tclass;
        struct { s0_tagged_id  result; }  tblock;
        struct { const char  *contents; }  literal;
        struct {
            const char  *name;
            s0_tagged_id_array  upvalues;
            s0_tagged_id_array  params;
            s0_tagged_id_array  results;
            s0_instruction_array  body;
        } macro;
        struct {
            s0_tagged_id  callee;
            s0_tagged_id_array  params;
            s0_tagged_id_array  results;
        } call;
        struct { s0_tagged_id_array  results; }  ret;
    } _;
    struct cork_dllist_item  siblings;
};

struct s0_instruction *
s0i_trecursive_new(struct swan *s, s0_id dest, struct cork_error *err);

struct s0_instruction *
s0i_tliteral_new(struct swan *s, s0_id dest, struct cork_error *err);

struct s0_instruction *
s0i_tfunction_new(struct swan *s, s0_id dest, struct cork_error *err);

struct s0_instruction *
s0i_tlocation_new(struct swan *s, s0_id dest, s0_tagged_id referent,
                  struct cork_error *err);

struct s0_instruction *
s0i_tinterface_new(struct swan *s, s0_id dest, struct cork_error *err);

/* key must have been allocated using cork_strdup */
int
s0i_tinterface_add_entry(struct swan *s, struct s0_instruction *self,
                         const char *key, s0_tagged_id entry,
                         struct cork_error *err);

struct s0_instruction *
s0i_tclass_new(struct swan *s, s0_id dest, const char *name,
               struct cork_error *err);

/* key must have been allocated using cork_strdup */
int
s0i_tclass_add_entry(struct swan *s, struct s0_instruction *self,
                     const char *key, s0_tagged_id entry,
                     struct cork_error *err);

struct s0_instruction *
s0i_tblock_new(struct swan *s, s0_id dest, s0_tagged_id result,
               struct cork_error *err);

struct s0_instruction *
s0i_ttype_new(struct swan *s, s0_id dest, struct cork_error *err);

struct s0_instruction *
s0i_literal_new(struct swan *s, s0_id dest, const char *contents,
                struct cork_error *err);

struct s0_instruction *
s0i_macro_new(struct swan *s, s0_id dest, const char *name,
              struct cork_error *err);

struct s0_instruction *
s0i_call_new(struct swan *s, struct cork_error *err);

struct s0_instruction *
s0i_return_new(struct swan *s, struct cork_error *err);


struct s0_position {
    size_t  line;
    size_t  column;
};


struct s0_basic_block *
s0_parse(struct swan *s, struct cork_slice *src, struct cork_error *err);


/*-----------------------------------------------------------------------
 * Basic blocks
 */

struct s0_basic_block {
    const char  *name;
    s0_value_array  upvalues;
    s0_type_array  params;
    s0_type_array  results;
    s0_instruction_array  body;
};

struct s0_basic_block *
s0_basic_block_new(struct swan *s, const char *name, struct cork_error *err);

/* Creates new reference to value */
#define s0_basic_block_add_upvalue(s, self, value, err) \
    (cork_array_append(swan_alloc((s)), &(self)->upvalues, \
                       cork_gc_incref(swan_gc((s)), (value)), (err)))

/* Creates new reference to type */
#define s0_basic_block_add_param(s, self, type, err) \
    (cork_array_append(swan_alloc((s)), &(self)->params, \
                       cork_gc_incref(swan_gc((s)), (type)), (err)))

/* Creates new reference to type */
#define s0_basic_block_add_result(s, self, type, err) \
    (cork_array_append(swan_alloc((s)), &(self)->results, \
                       cork_gc_incref(swan_gc((s)), (type)), (err)))

/* Steals reference to body */
#define s0_basic_block_set_body(s, self, _body, err) \
    ((self)->body = (_body), 0)

/* Steals reference to instr */
#define s0_basic_block_add_instruction(s, self, instr, err) \
    (cork_array_append(swan_alloc((s)), &(self)->body, \
                       (instr), (err)))

int
s0_basic_block_evaluate(struct swan *s, struct s0_basic_block *basic_block,
                        s0_value_array *params, s0_value_array *results,
                        struct cork_error *err);


/*-----------------------------------------------------------------------
 * Values
 */

enum s0_value_kind {
    S0_VALUE_TYPE,
    S0_VALUE_LITERAL,
    S0_VALUE_MACRO
};

struct s0_value {
    enum s0_value_kind  kind;
    union {
        struct s0_type  *type;
        const char  *literal;
        struct s0_basic_block  *macro;
    } _;
    /* The type of the value */
    struct s0_type  *type;
};


/* Creates new reference to type */
struct s0_value *
s0_type_value_new(struct swan *s, struct s0_type *type,
                  struct cork_error *err);

struct s0_value *
s0_literal_value_new(struct swan *s, const char *contents,
                     struct cork_error *err);

/* Creates new reference to macro */
struct s0_value *
s0_macro_value_new(struct swan *s, struct s0_basic_block *block,
                   struct cork_error *err);

struct s0_type *
s0_value_get_type(struct swan *s, struct s0_value *value,
                  struct cork_error *err);


/*-----------------------------------------------------------------------
 * Scopes
 */

struct s0_scope {
    const char  *name;
    struct cork_hash_table  entries;
};

struct s0_scope *
s0_scope_new(struct swan *s, const char *name, struct cork_error *err);

struct s0_scope *
s0_scope_new_top_level(struct swan *s, struct cork_error *err);

/* steals reference to value */
int
s0_scope_add(struct swan *s, struct s0_scope *scope,
             s0_tagged_id id, struct s0_value *value,
             struct cork_error *err);

struct s0_value *
s0_scope_get(struct swan *s, struct s0_scope *scope,
             s0_tagged_id id, struct cork_error *err);


/*-----------------------------------------------------------------------
 * Error handling
 */

/* Hash of "swanson.s0" */
#define S0_ERROR  0x4972f292

enum s0_error {
    S0_REDEFINED,
    S0_UNDEFINED,
    S0_EVALUATION_ERROR,
    S0_SYNTAX_ERROR
};

int
s0_interface_redefinition_set(struct cork_alloc *alloc,
                              struct cork_error *err,
                              const char *id);

int
s0_recursive_redefinition_set(struct cork_alloc *alloc,
                              struct cork_error *err);

int
s0_scope_redefined_set(struct cork_alloc *alloc, struct cork_error *err,
                       s0_tagged_id id, const char *scope_name);

int
s0_scope_undefined_set(struct cork_alloc *alloc, struct cork_error *err,
                       s0_tagged_id id, const char *scope_name);


#endif  /* SWANSON_S0_H */
