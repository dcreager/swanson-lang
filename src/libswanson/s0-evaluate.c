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
#include <libcork/core/checkers.h>
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
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while (0)
#else
#define DEBUG(...) /* no debug messages */
#endif


/*-----------------------------------------------------------------------
 * Error reporting
 */

struct s0_wrong_kind_extra {
    s0_tagged_id  id;
    const char  *expected;
};

static int
s0_wrong_kind(struct cork_alloc *alloc, struct cork_error *err,
              struct cork_buffer *dest)
{
    struct s0_wrong_kind_extra  *extra = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL, "Expected %s for %c%"PRIuPTR,
         extra->expected, s0_id_tag_name(s0_tagged_id_tag(extra->id)),
         s0_tagged_id_id(extra->id));
}

static int
s0_wrong_kind_set(struct cork_alloc *alloc, struct cork_error *err,
                  s0_tagged_id id, const char *expected)
{
    struct s0_wrong_kind_extra  extra = { id, expected };
    return cork_error_set_extra(alloc, err,
                                S0_ERROR,
                                S0_EVALUATION_ERROR,
                                s0_wrong_kind,
                                extra);
}


struct s0_wrong_count_extra {
    const char  *macro_name;
    const char  *kind;
};

static int
s0_wrong_count(struct cork_alloc *alloc, struct cork_error *err,
                      struct cork_buffer *dest)
{
    struct s0_wrong_count_extra  *extra = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL, "Wrong number of %s when calling %s",
         extra->kind, extra->macro_name);
}

static int
s0_wrong_count_set(struct cork_alloc *alloc, struct cork_error *err,
                   const char *macro_name, const char *kind)
{
    struct s0_wrong_count_extra  extra = { macro_name, kind };
    return cork_error_set_extra(alloc, err,
                                S0_ERROR,
                                S0_EVALUATION_ERROR,
                                s0_wrong_count,
                                extra);
}


/*-----------------------------------------------------------------------
 * Evaluation
 */

static struct s0_type *
s0_evaluate_expect_type(struct swan *s, struct s0_scope *scope,
                        s0_tagged_id id, struct cork_error *err)
{
    struct s0_value  *value;
    rpp_check(value = s0_scope_get(s, scope, id, err));
    if (value->kind == S0_VALUE_TYPE) {
        return value->_.type;
    } else {
        s0_wrong_kind_set(swan_alloc(s), err, id, "type");
        return NULL;
    }
}

/* Saves a type into the scope table.  The identifier must either be
 * un-bound, or bound to a recursive type that hasn't been resolved yet.
 * Creates a new reference to type. */
static struct s0_value *
s0_evaluate_save_type(struct swan *s, struct s0_scope *scope,
                      s0_tagged_id id, struct s0_type *type,
                      struct cork_error *err)
{
    /* Reference counting: Both s0_type_value_new and
     * s0_recursive_type_resolve will create a new reference to type. */

    struct s0_value  *value;
    struct s0_type  *old_type;

    /* It's not an error if id isn't already bound to something, so we
     * use a NULL err parameter. */
    value = s0_scope_get(s, scope, id, NULL);

    if (value == NULL) {
        /* Great, this is a new binding. */
        rpp_check(value = s0_type_value_new(s, type, err));
        ei_check(s0_scope_add(s, scope, id, value, err));
        return value;

error:
        cork_gc_decref(swan_gc(s), value);
        return NULL;
    }

    if (value->kind != S0_VALUE_TYPE) {
        /* id is already bound to something other than a type. */
        s0_scope_redefined_set(swan_alloc(s), err, id, scope->name);
        return NULL;
    }

    old_type = value->_.type;
    if (old_type->kind != S0_TYPE_RECURSIVE) {
        /* id is already bound to a non-recursive type. */
        s0_scope_redefined_set(swan_alloc(s), err, id, scope->name);
        return NULL;
    }

    rpi_check(s0_recursive_type_resolve(s, old_type, type, err));
    return value;
}

static int
s0_evaluate_TRECURSIVE(struct swan *s, struct s0_scope *scope,
                       struct s0_instruction *instr,
                       s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating TRECURSIVE", scope->name);
    struct s0_type  *type;
    struct s0_value  *value;
    rip_check(type = s0_recursive_type_new(s, err));
    ep_check(value = s0_type_value_new(s, type, err));
    /* We don't use s0_evaluate_save_type because that would allow you
     * to TRECURSIVE a type twice. */
    ei_check(s0_scope_add(s, scope, instr->dest, value, err));
    cork_gc_decref(swan_gc(s), type);
    return 0;

error:
    cork_gc_decref(swan_gc(s), type);
    cork_gc_decref(swan_gc(s), value);
    return -1;
}

static int
s0_evaluate_TLITERAL(struct swan *s, struct s0_scope *scope,
                     struct s0_instruction *instr,
                     s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating TLITERAL", scope->name);
    struct s0_type  *type;
    struct s0_value  *value;
    rip_check(type = s0_literal_type_new(s, err));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type, err));
    cork_gc_decref(swan_gc(s), type);
    return 0;

error:
    cork_gc_decref(swan_gc(s), type);
    return -1;
}

static int
s0_evaluate_TFUNCTION(struct swan *s, struct s0_scope *scope,
                      struct s0_instruction *instr,
                      s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating TFUNCTION", scope->name);
    size_t  i;
    struct s0_type  *type = NULL;
    struct s0_value  *value;

    rip_check(type = s0_function_type_new(s, err));

    /* Construct the param type list */
    for (i = 0; i < cork_array_size(&instr->_.tfunction.params); i++) {
        s0_tagged_id  param =
            cork_array_at(&instr->_.tfunction.params, i);
        struct s0_type  *param_type;
        ep_check(param_type = s0_evaluate_expect_type(s, scope, param, err));
        ei_check(s0_function_type_add_param(s, type, param_type, err));
    }

    /* Construct the result type list */
    for (i = 0; i < cork_array_size(&instr->_.tfunction.results); i++) {
        s0_tagged_id  result =
            cork_array_at(&instr->_.tfunction.results, i);
        struct s0_type  *result_type;
        ep_check(result_type = s0_evaluate_expect_type(s, scope, result, err));
        ei_check(s0_function_type_add_result(s, type, result_type, err));
    }

    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type, err));
    cork_gc_decref(swan_gc(s), type);
    return 0;

error:
    cork_gc_decref(swan_gc(s), type);
    return -1;
}

static int
s0_evaluate_TLOCATION(struct swan *s, struct s0_scope *scope,
                      struct s0_instruction *instr,
                      s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating TLOCATION", scope->name);
    struct s0_type  *referent;
    struct s0_type  *type;
    struct s0_value  *value;

    rip_check(referent = s0_evaluate_expect_type
              (s, scope, instr->_.tlocation.referent, err));
    rip_check(type = s0_location_type_new(s, referent, err));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type, err));
    cork_gc_decref(swan_gc(s), type);
    return 0;

error:
    cork_gc_decref(swan_gc(s), type);
    return -1;
}

static int
s0_evaluate_TINTERFACE(struct swan *s, struct s0_scope *scope,
                       struct s0_instruction *instr,
                       s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating TINTERFACE", scope->name);
    size_t  i;
    struct s0_type  *type = NULL;
    struct s0_value  *value;

    rip_check(type = s0_interface_type_new(s, err));
    for (i = 0; i < cork_array_size(&instr->_.tinterface.entries); i++) {
        struct s0_tinterface_entry  entry =
            cork_array_at(&instr->_.tinterface.entries, i);
        struct s0_type  *entry_type;
        ep_check(entry_type = s0_evaluate_expect_type
                 (s, scope, entry.entry, err));
        ei_check(s0_interface_type_add(s, type, entry.key, entry_type, err));
    }

    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type, err));
    cork_gc_decref(swan_gc(s), type);
    return 0;

error:
    cork_gc_decref(swan_gc(s), type);
    return -1;
}

static int
s0_evaluate_TBLOCK(struct swan *s, struct s0_scope *scope,
                   struct s0_instruction *instr,
                   s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating TBLOCK", scope->name);
    struct s0_type  *result;
    struct s0_type  *type;
    struct s0_value  *value;

    rip_check(result = s0_evaluate_expect_type
              (s, scope, instr->_.tblock.result, err));
    rip_check(type = s0_block_type_new(s, result, err));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type, err));
    cork_gc_decref(swan_gc(s), type);
    return 0;

error:
    cork_gc_decref(swan_gc(s), type);
    return -1;
}

static int
s0_evaluate_TTYPE(struct swan *s, struct s0_scope *scope,
                  struct s0_instruction *instr,
                  s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating TTYPE", scope->name);
    struct s0_type  *type;
    struct s0_value  *value;
    rip_check(type = s0_type_type_new(s, err));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type, err));
    cork_gc_decref(swan_gc(s), type);
    return 0;

error:
    cork_gc_decref(swan_gc(s), type);
    return -1;
}

static int
s0_evaluate_LITERAL(struct swan *s, struct s0_scope *scope,
                    struct s0_instruction *instr,
                    s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating LITERAL", scope->name);
    struct s0_value  *value;
    rip_check(value = s0_literal_value_new(s, instr->_.literal.contents, err));
    rii_check(s0_scope_add(s, scope, instr->dest, value, err));
    return 0;
}

static int
s0_evaluate_MACRO(struct swan *s, struct s0_scope *scope,
                  struct s0_instruction *instr,
                  s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating MACRO", scope->name);
    size_t  i;
    struct s0_basic_block  *block;
    struct s0_value  *value;

    rip_check(block = s0_basic_block_new(s, instr->_.macro.name, err));

    /* Construct the upvalue list */
    for (i = 0; i < cork_array_size(&instr->_.macro.upvalues); i++) {
        s0_tagged_id  upvalue_id = cork_array_at(&instr->_.macro.upvalues, i);
        struct s0_value  *upvalue;
        ep_check(upvalue = s0_scope_get(s, scope, upvalue_id, err));
        ei_check(s0_basic_block_add_upvalue(s, block, upvalue, err));
    }

    /* Construct the param type list */
    for (i = 0; i < cork_array_size(&instr->_.macro.params); i++) {
        s0_tagged_id  param = cork_array_at(&instr->_.macro.params, i);
        struct s0_type  *param_type;
        ep_check(param_type = s0_evaluate_expect_type(s, scope, param, err));
        ei_check(s0_basic_block_add_param(s, block, param_type, err));
    }

    /* Construct the result type list */
    for (i = 0; i < cork_array_size(&instr->_.macro.results); i++) {
        s0_tagged_id  result = cork_array_at(&instr->_.macro.results, i);
        struct s0_type  *result_type;
        ep_check(result_type = s0_evaluate_expect_type(s, scope, result, err));
        ei_check(s0_basic_block_add_result(s, block, result_type, err));
    }

    /* Construct the body */
    for (i = 0; i < cork_array_size(&instr->_.macro.body); i++) {
        struct s0_instruction  *body_instr =
            cork_array_at(&instr->_.macro.body, i);
        ei_check(s0_basic_block_add_instruction
                 (s, block, cork_gc_incref(swan_gc(s), body_instr), err));
    }

    ep_check(value = s0_macro_value_new(s, block, err));
    ei_check(s0_scope_add(s, scope, instr->dest, value, err));
    cork_gc_decref(swan_gc(s), block);
    return 0;

error:
    cork_gc_decref(swan_gc(s), block);
    return -1;
}

static int
s0_evaluate_call_macro(struct swan *s, struct s0_scope *scope,
                       struct s0_value *macro, struct s0_instruction *instr,
                       s0_value_array *dest, struct cork_error *err)
{
    size_t  i;
    s0_value_array  params;
    s0_value_array  results;

    cork_array_init(swan_alloc(s), &params);
    cork_array_init(swan_alloc(s), &results);

    /* Construct the param list */
    if (cork_array_size(&macro->_.macro->params) !=
        cork_array_size(&instr->_.call.params)) {
        s0_wrong_count_set
            (swan_alloc(s), err, macro->_.macro->name, "parameters");
        goto error;
    }

    for (i = 0; i < cork_array_size(&instr->_.call.params); i++) {
        s0_tagged_id  param_id = cork_array_at(&instr->_.call.params, i);
        struct s0_value  *param;
        ep_check(param = s0_scope_get(s, scope, param_id, err));
        /* We don't need to incref or decref the parameters; the scope
         * has references to each parameter, and the scope stays alive
         * across the call to s0_basic_block evaluate. */
        ei_check(cork_array_append(swan_alloc(s), &params, param, err));
    }

    /* Call the macro */
    ei_check(s0_basic_block_evaluate
             (s, macro->_.macro, &params, &results, err));

    /* Save the results into the current scope */
    if (cork_array_size(&results) != cork_array_size(&instr->_.call.results)) {
        s0_wrong_count_set
            (swan_alloc(s), err, macro->_.macro->name, "results");
        goto error;
    }

    for (i = 0; i < cork_array_size(&results); i++) {
        s0_tagged_id  result_id = cork_array_at(&instr->_.call.results, i);
        struct s0_value  *result = cork_array_at(&results, i);
        DEBUG("    Saving result %p %lu %p", scope, (unsigned long) result_id, result);
        ei_check(s0_scope_add(s, scope, result_id, result, err));
    }

    cork_array_done(swan_alloc(s), &params);
    cork_array_done(swan_alloc(s), &results);
    return 0;

error:
    cork_array_done(swan_alloc(s), &params);
    cork_array_done(swan_alloc(s), &results);
    return -1;
}

static int
s0_evaluate_CALL(struct swan *s, struct s0_scope *scope,
                 struct s0_instruction *instr,
                 s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating CALL", scope->name);
    struct s0_value  *callee;
    rip_check(callee = s0_scope_get(s, scope, instr->_.call.callee, err));

    switch (callee->kind) {
        case S0_VALUE_MACRO:
            return s0_evaluate_call_macro(s, scope, callee, instr, dest, err);

        default:
            s0_wrong_kind_set
                (swan_alloc(s), err, instr->_.call.callee, "macro");
            return -1;
    }
}

static int
s0_evaluate_RETURN(struct swan *s, struct s0_scope *scope,
                   struct s0_instruction *instr,
                   s0_value_array *dest, struct cork_error *err)
{
    DEBUG("--- %s: Evaluating RETURN", scope->name);
    size_t  i;

    /* Construct the result list */
    for (i = 0; i < cork_array_size(&instr->_.ret.results); i++) {
        s0_tagged_id  result_id =
            cork_array_at(&instr->_.ret.results, i);
        DEBUG("    Returning %p %lu", scope, (unsigned long) result_id);
        struct s0_value  *result;
        rip_check(result = s0_scope_get(s, scope, result_id, err));
        rii_check(cork_array_append
                  (swan_alloc(s), dest,
                   cork_gc_incref(swan_gc(s), result), err));
    }

    return 0;
}

static int
s0_evaluate_instruction(struct swan *s, struct s0_scope *scope,
                        struct s0_instruction *instr,
                        s0_value_array *dest, struct cork_error *err)
{
    switch (instr->op) {
#define EVALUATE_OPCODE(name) \
        case S0_##name: \
            return s0_evaluate_##name(s, scope, instr, dest, err);
        S0_OPCODES(EVALUATE_OPCODE)
#undef EVALUATE_OPCODE

        default:
            cork_unknown_error_set(swan_alloc(s), err);
            return -1;
    }
}

int
s0_basic_block_evaluate(struct swan *s, struct s0_basic_block *self,
                        s0_value_array *params, s0_value_array *results,
                        struct cork_error *err)
{
    size_t  i;
    struct s0_scope  *scope = NULL;
    rip_check(scope = s0_scope_new(s, self->name, err));

    /* Add the upvalues and parameters to the scope */
    for (i = 0; i < cork_array_size(&self->upvalues); i++) {
        struct s0_value  *upvalue = cork_array_at(&self->upvalues, i);
        ei_check(s0_scope_add
                 (s, scope, s0_tagged_id(S0_ID_TAG_UPVALUE, i),
                  cork_gc_incref(swan_gc(s), upvalue), err));
    }

    for (i = 0; i < cork_array_size(params); i++) {
        struct s0_value  *param = cork_array_at(params, i);
        ei_check(s0_scope_add
                 (s, scope, s0_tagged_id(S0_ID_TAG_PARAM, i),
                  cork_gc_incref(swan_gc(s), param), err));
    }

    /* Then evaluate each instruction */
    for (i = 0; i < cork_array_size(&self->body); i++) {
        struct s0_instruction  *instr = cork_array_at(&self->body, i);
        ei_check(s0_evaluate_instruction(s, scope, instr, results, err));
    }

    cork_gc_decref(swan_gc(s), scope);
    return 0;

error:
    cork_gc_decref(swan_gc(s), scope);
    return -1;
}
