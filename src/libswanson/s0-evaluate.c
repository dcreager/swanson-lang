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
#include <libcork/helpers/errors.h>
#include <libcork/ds.h>

#include "swanson/s0.h"
#include "swanson/state.h"

#if !defined(EVALUATE_DEBUG)
#define EVALUATE_DEBUG 0
#endif

#if EVALUATE_DEBUG
#include <stdio.h>
#define DEBUG(...) \
    do { \
        fprintf(std__VA_ARGS__); \
        fprintf(std"\n"); \
    } while (0)
#else
#define DEBUG(...) /* no debug messages */
#endif


/*-----------------------------------------------------------------------
 * Evaluation
 */

static struct s0_type *
s0_evaluate_expect_type(struct swan *s, struct s0_scope *scope,
                        s0_tagged_id id)
{
    struct s0_value  *value;
    rpp_check(value = s0_scope_get(s, scope, id));
    if (value->kind == S0_VALUE_TYPE) {
        return value->_.type;
    } else {
        cork_error_set
            (S0_ERROR, S0_EVALUATION_ERROR,
             "Expected type for %c%"PRIuPTR,
             s0_id_tag_name(s0_tagged_id_tag(id)),
             s0_tagged_id_id(id));
        return NULL;
    }
}

/* Saves a type into the scope table.  The identifier must either be
 * un-bound, or bound to a recursive type that hasn't been resolved yet.
 * Creates a new reference to type. */
static struct s0_value *
s0_evaluate_save_type(struct swan *s, struct s0_scope *scope,
                      s0_tagged_id id, struct s0_type *type)
{
    /* Reference counting: Both s0_type_value_new and
     * s0_recursive_type_resolve will create a new reference to type. */

    struct s0_value  *value;
    struct s0_type  *old_type;
    value = s0_scope_get(s, scope, id);

    if (value == NULL) {
        /* Great, this is a new binding. */
        cork_error_clear();
        rpp_check(value = s0_type_value_new(s, type));
        ei_check(s0_scope_add(s, scope, id, value));
        return value;

error:
        cork_gc_decref(value);
        return NULL;
    }

    if (value->kind != S0_VALUE_TYPE) {
        /* id is already bound to something other than a type. */
        cork_error_set
            (S0_ERROR, S0_REDEFINED,
             "%c%"PRIuPTR" redefined in scope %s",
             s0_id_tag_name(s0_tagged_id_tag(id)),
             s0_tagged_id_id(id), scope->name);
        return NULL;
    }

    old_type = value->_.type;
    if (old_type->kind != S0_TYPE_RECURSIVE) {
        /* id is already bound to a non-recursive type. */
        cork_error_set
            (S0_ERROR, S0_REDEFINED,
             "%c%"PRIuPTR" redefined in scope %s",
             s0_id_tag_name(s0_tagged_id_tag(id)),
             s0_tagged_id_id(id), scope->name);
        return NULL;
    }

    rpi_check(s0_recursive_type_resolve(s, old_type, type));
    return value;
}

static int
s0_evaluate_TRECURSIVE(struct swan *s, struct s0_scope *scope,
                       struct s0_instruction *instr,
                       struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TRECURSIVE", scope->name);
    struct s0_type  *type;
    struct s0_value  *value;
    rip_check(type = s0_recursive_type_new(s));
    ep_check(value = s0_type_value_new(s, type));
    /* We don't use s0_evaluate_save_type because that would allow you
     * to TRECURSIVE a type twice. */
    ei_check(s0_scope_add(s, scope, instr->dest, value));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    cork_gc_decref(value);
    return -1;
}

static int
s0_evaluate_TTYPE(struct swan *s, struct s0_scope *scope,
                  struct s0_instruction *instr,
                  struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TTYPE", scope->name);
    struct s0_type  *type;
    struct s0_value  *value;
    rip_check(type = s0_type_type_new(s));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    return -1;
}

static int
s0_evaluate_TLITERAL(struct swan *s, struct s0_scope *scope,
                     struct s0_instruction *instr,
                     struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TLITERAL", scope->name);
    struct s0_type  *type;
    struct s0_value  *value;
    rip_check(type = s0_literal_type_new(s));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    return -1;
}

static int
s0_evaluate_TANY(struct swan *s, struct s0_scope *scope,
                 struct s0_instruction *instr,
                 struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TANY", scope->name);
    struct s0_type  *type;
    struct s0_value  *value;
    rip_check(type = s0_any_type_new(s));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    return -1;
}

static int
s0_evaluate_TPRODUCT(struct swan *s, struct s0_scope *scope,
                      struct s0_instruction *instr,
                      struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TPRODUCT", scope->name);
    size_t  i;
    struct s0_type  *type = NULL;
    struct s0_value  *value;

    rip_check(type = s0_product_type_new(s));

    /* Construct the element type list */
    for (i = 0; i < cork_array_size(&instr->_.tproduct.elements); i++) {
        s0_tagged_id  element =
            cork_array_at(&instr->_.tproduct.elements, i);
        struct s0_type  *element_type;
        ep_check(element_type = s0_evaluate_expect_type
                 (s, scope, element));
        ei_check(s0_product_type_add(s, type, element_type));
    }

    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    return -1;
}

static int
s0_evaluate_TFUNCTION(struct swan *s, struct s0_scope *scope,
                      struct s0_instruction *instr,
                      struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TFUNCTION", scope->name);
    struct s0_type  *input;
    struct s0_type  *output;
    struct s0_type  *type;
    struct s0_value  *value;

    rip_check(input = s0_evaluate_expect_type
              (s, scope, instr->_.tfunction.input));
    rip_check(output = s0_evaluate_expect_type
              (s, scope, instr->_.tfunction.output));
    rip_check(type = s0_function_type_new(s, input, output));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    return -1;
}

static int
s0_evaluate_TLOCATION(struct swan *s, struct s0_scope *scope,
                      struct s0_instruction *instr,
                      struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TLOCATION", scope->name);
    struct s0_type  *referent;
    struct s0_type  *type;
    struct s0_value  *value;

    rip_check(referent = s0_evaluate_expect_type
              (s, scope, instr->_.tlocation.referent));
    rip_check(type = s0_location_type_new(s, referent));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    return -1;
}

static int
s0_evaluate_TINTERFACE(struct swan *s, struct s0_scope *scope,
                       struct s0_instruction *instr,
                       struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TINTERFACE", scope->name);
    size_t  i;
    struct s0_type  *type = NULL;
    struct s0_value  *value;

    rip_check(type = s0_interface_type_new(s));
    for (i = 0; i < cork_array_size(&instr->_.tinterface.entries); i++) {
        struct s0_tinterface_entry  entry =
            cork_array_at(&instr->_.tinterface.entries, i);
        struct s0_type  *entry_type;
        ep_check(entry_type = s0_evaluate_expect_type
                 (s, scope, entry.entry));
        ei_check(s0_interface_type_add(s, type, entry.key, entry_type));
    }

    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    return -1;
}

static int
s0_evaluate_TCLASS(struct swan *s, struct s0_scope *scope,
                   struct s0_instruction *instr,
                   struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TCLASS", scope->name);
    size_t  i;
    struct s0_type  *type = NULL;
    struct s0_value  *value;

    ep_check(type = s0_class_type_new(s, instr->_.tclass.name));
    for (i = 0; i < cork_array_size(&instr->_.tclass.entries); i++) {
        struct s0_tinterface_entry  entry =
            cork_array_at(&instr->_.tclass.entries, i);
        struct s0_value  *entry_value;
        ep_check(entry_value = s0_scope_get(s, scope, entry.entry));
        ei_check(s0_class_type_add(s, type, entry.key, entry_value));
    }

    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    return -1;
}

static int
s0_evaluate_TBLOCK(struct swan *s, struct s0_scope *scope,
                   struct s0_instruction *instr,
                   struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TBLOCK", scope->name);
    struct s0_type  *result;
    struct s0_type  *type;
    struct s0_value  *value;

    rip_check(result = s0_evaluate_expect_type
              (s, scope, instr->_.tblock.result));
    rip_check(type = s0_block_type_new(s, result));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type));
    cork_gc_decref(type);
    return 0;

error:
    cork_gc_decref(type);
    return -1;
}

static int
s0_evaluate_LITERAL(struct swan *s, struct s0_scope *scope,
                    struct s0_instruction *instr,
                    struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating LITERAL", scope->name);
    struct s0_value  *value;
    rip_check(value = s0_literal_value_new(s, instr->_.literal.contents));
    rii_check(s0_scope_add(s, scope, instr->dest, value));
    return 0;
}

static int
s0_evaluate_TUPLE(struct swan *s, struct s0_scope *scope,
                  struct s0_instruction *instr,
                  struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating TUPLE", scope->name);
    size_t  i;
    struct s0_value  *value;

    rip_check(value = s0_tuple_value_new(s));

    /* Construct the element list */
    for (i = 0; i < cork_array_size(&instr->_.tuple.elements); i++) {
        s0_tagged_id  element_id =
            cork_array_at(&instr->_.tuple.elements, i);
        struct s0_value  *element;
        ep_check(element = s0_scope_get(s, scope, element_id));
        ei_check(s0_tuple_value_add(s, value, element));
    }

    rii_check(s0_scope_add(s, scope, instr->dest, value));
    return 0;

error:
    cork_gc_decref(value);
    return -1;
}

static int
s0_evaluate_GETTUPLE(struct swan *s, struct s0_scope *scope,
                     struct s0_instruction *instr,
                     struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating GETTUPLE", scope->name);
    struct s0_value  *tuple;
    struct s0_value  *value;

    rip_check(tuple = s0_scope_get(s, scope, instr->_.gettuple.src));
    rip_check(value = s0_tuple_value_get
              (s, tuple, instr->_.gettuple.index));
    rii_check(s0_scope_add
              (s, scope, instr->dest, cork_gc_incref(value)));
    return 0;
}

static int
s0_evaluate_GETCLASS(struct swan *s, struct s0_scope *scope,
                     struct s0_instruction *instr,
                     struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating GETCLASS", scope->name);
    struct s0_type  *cls;
    struct s0_value  *value;

    rip_check(cls = s0_evaluate_expect_type
              (s, scope, instr->_.getclass.src));
    rip_check(value = s0_class_type_get
              (s, cls, instr->_.getclass.index));
    rii_check(s0_scope_add
              (s, scope, instr->dest, cork_gc_incref(value)));
    return 0;
}

static int
s0_evaluate_MACRO(struct swan *s, struct s0_scope *scope,
                  struct s0_instruction *instr,
                  struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating MACRO", scope->name);
    size_t  i;
    struct s0_basic_block  *block;
    struct s0_value  *upvalue = NULL;
    struct s0_type  *input;
    struct s0_type  *output;
    struct s0_value  *value;

    if (instr->_.macro.upvalue != S0_ID_NULL) {
        rip_check(upvalue = s0_scope_get
                  (s, scope, instr->_.macro.upvalue));
    }
    rip_check(input = s0_evaluate_expect_type
              (s, scope, instr->_.macro.input));
    rip_check(output = s0_evaluate_expect_type
              (s, scope, instr->_.macro.output));

    rip_check(block = s0_basic_block_new
              (s, instr->_.macro.name, upvalue, input, output));

    /* Construct the body */
    for (i = 0; i < cork_array_size(&instr->_.macro.body); i++) {
        struct s0_instruction  *body_instr =
            cork_array_at(&instr->_.macro.body, i);
        ei_check(s0_basic_block_add_instruction
                 (s, block, cork_gc_incref(body_instr)));
    }

    ep_check(value = s0_macro_value_new(s, block));
    ei_check(s0_scope_add(s, scope, instr->dest, value));
    cork_gc_decref(block);
    return 0;

error:
    cork_gc_decref(block);
    return -1;
}

static int
s0_evaluate_CALL(struct swan *s, struct s0_scope *scope,
                 struct s0_instruction *instr,
                 struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating CALL", scope->name);
    struct s0_value  *callee;
    struct s0_value  *input;
    struct s0_value  *output;

    /* Grab the thing we're trying to call */
    rip_check(callee = s0_scope_get(s, scope, instr->_.call.callee));

    /* We don't need to incref or decref the parameter; the scope has a
     * reference to it, and the scope stays alive across the call to
     * s0_value_evaluate. */
    rip_check(input = s0_scope_get(s, scope, instr->_.call.input));

    /* Make the call */
    rip_check(output = s0_value_evaluate(s, callee, input));

    /* Save the results into the current scope */
    rii_check(s0_scope_add(s, scope, instr->dest, output));
    return 0;
}

static int
s0_evaluate_RETURN(struct swan *s, struct s0_scope *scope,
                   struct s0_instruction *instr,
                   struct s0_value **dest)
{
    DEBUG("--- %s: Evaluating RETURN", scope->name);
    struct s0_value  *result;
    rip_check(result = s0_scope_get(s, scope, instr->_.ret.result));
    *dest = cork_gc_incref(result);
    return 0;
}

static int
s0_evaluate_instruction(struct swan *s, struct s0_scope *scope,
                        struct s0_instruction *instr,
                        struct s0_value **dest)
{
    switch (instr->op) {
#define EVALUATE_OPCODE(name) \
        case S0_##name: \
            return s0_evaluate_##name(s, scope, instr, dest);
        S0_OPCODES(EVALUATE_OPCODE)
#undef EVALUATE_OPCODE

        default:
            cork_abort("%s", "Unknown S0 opcode");
    }
}

struct s0_value *
s0_basic_block_evaluate(struct swan *s, struct s0_basic_block *self,
                        struct s0_value *input)
{
    size_t  i;
    struct s0_scope  *scope = NULL;
    struct s0_value  *output = NULL;
    rpp_check(scope = s0_scope_new(s, self->name));

    /* Add the upvalue and parameter to the scope */
    if (self->upvalue != NULL) {
        ei_check(s0_scope_add
                 (s, scope, s0_tagged_id(S0_ID_TAG_UPVALUE, 0),
                  cork_gc_incref(self->upvalue)));
    }

    if (input != NULL) {
        ei_check(s0_scope_add
                 (s, scope, s0_tagged_id(S0_ID_TAG_PARAM, 0),
                  cork_gc_incref(input)));
    }

    /* Then evaluate each instruction */
    for (i = 0; i < cork_array_size(&self->body); i++) {
        struct s0_instruction  *instr = cork_array_at(&self->body, i);
        ei_check(s0_evaluate_instruction(s, scope, instr, &output));
    }

    if (output == NULL) {
        cork_error_set
            (S0_ERROR, S0_EVALUATION_ERROR,
             "No result when calling %s", self->name);
        return NULL;
    }

    cork_gc_decref(scope);
    return output;

error:
    cork_gc_decref(scope);
    return NULL;
}
