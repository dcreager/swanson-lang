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

static int
s0_empty_file(struct cork_alloc *alloc, struct cork_error *err,
              struct cork_buffer *dest)
{
    return cork_buffer_set_string
        (alloc, dest, "Empty S0 file", NULL);
}

static int
s0_empty_file_set(struct cork_alloc *alloc, struct cork_error *err)
{
    return cork_error_set(alloc, err,
                          S0_ERROR,
                          S0_EVALUATION_ERROR,
                          s0_empty_file);
}


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
        return value->contents.type;
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
    struct s0_recursive_type  *recursive;

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

    old_type = value->contents.type;
    if (old_type->kind != S0_TYPE_RECURSIVE) {
        /* id is already bound to a non-recursive type. */
        s0_scope_redefined_set(swan_alloc(s), err, id, scope->name);
        return NULL;
    }

    recursive = cork_container_of
        (old_type, struct s0_recursive_type, parent);
    rpi_check(s0_recursive_type_resolve(s, old_type, type, err));
    return value;
}

static struct s0_value *
s0_evaluate_TRECURSIVE(struct swan *s, struct s0_scope *scope,
                       struct s0_instruction *instr, struct cork_error *err)
{
    DEBUG("--- Evaluating TRECURSIVE");
    struct s0_type  *type;
    struct s0_value  *value;
    rpp_check(type = s0_recursive_type_new(s, err));
    ep_check(value = s0_type_value_new(s, type, err));
    /* We don't use s0_evaluate_save_type because that would allow you
     * to TRECURSIVE a type twice. */
    ei_check(s0_scope_add(s, scope, instr->dest, value, err));
    cork_gc_decref(swan_gc(s), type);
    return value;

error:
    cork_gc_decref(swan_gc(s), type);
    cork_gc_decref(swan_gc(s), value);
    return NULL;
}

static struct s0_value *
s0_evaluate_TLITERAL(struct swan *s, struct s0_scope *scope,
                     struct s0_instruction *instr, struct cork_error *err)
{
    DEBUG("--- Evaluating TLITERAL");
    struct s0_type  *type;
    struct s0_value  *value;
    rpp_check(type = s0_literal_type_new(s, err));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type, err));
    cork_gc_decref(swan_gc(s), type);
    return value;

error:
    cork_gc_decref(swan_gc(s), type);
    return NULL;
}

static struct s0_value *
s0_evaluate_TFUNCTION(struct swan *s, struct s0_scope *scope,
                      struct s0_instruction *instr, struct cork_error *err)
{
    DEBUG("--- Evaluating TFUNCTION");
    size_t  i;
    struct s0_type_list  *params = NULL;
    struct s0_type_list  *results = NULL;
    struct s0_type  *type = NULL;
    struct s0_value  *value;

    /* Construct the param type list */
    for (i = cork_array_size(&instr->args.tfunction.params); i-- > 0; ) {
        s0_tagged_id  param =
            cork_array_at(&instr->args.tfunction.params, i);
        struct s0_type  *param_type;
        ep_check(param_type = s0_evaluate_expect_type(s, scope, param, err));
        ep_check(params = s0_type_list_new(s, param_type, params, err));
    }

    /* Construct the result type list */
    for (i = cork_array_size(&instr->args.tfunction.results); i-- > 0; ) {
        s0_tagged_id  result =
            cork_array_at(&instr->args.tfunction.results, i);
        struct s0_type  *result_type;
        ep_check(result_type = s0_evaluate_expect_type(s, scope, result, err));
        ep_check(results = s0_type_list_new(s, result_type, results, err));
    }

    ep_check(type = s0_function_type_new(s, params, results, err));
    /* type just stole our references to params an results */
    params = NULL;
    results = NULL;

    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type, err));
    cork_gc_decref(swan_gc(s), type);
    return value;

error:
    cork_gc_decref(swan_gc(s), params);
    cork_gc_decref(swan_gc(s), results);
    cork_gc_decref(swan_gc(s), type);
    return NULL;
}

static struct s0_value *
s0_evaluate_TLOCATION(struct swan *s, struct s0_scope *scope,
                      struct s0_instruction *instr, struct cork_error *err)
{
    DEBUG("--- Evaluating TLOCATION");
    struct s0_type  *referent;
    struct s0_type  *type;
    struct s0_value  *value;

    rpp_check(referent = s0_evaluate_expect_type
              (s, scope, instr->args.tlocation.referent, err));
    rpp_check(type = s0_location_type_new(s, referent, err));
    ep_check(value = s0_evaluate_save_type(s, scope, instr->dest, type, err));
    cork_gc_decref(swan_gc(s), type);
    return value;

error:
    cork_gc_decref(swan_gc(s), type);
    return NULL;
}

static struct s0_value *
s0_evaluate_instruction(struct swan *s, struct s0_scope *scope,
                        struct s0_instruction *instr, struct cork_error *err)
{
    switch (instr->op) {
#define EVALUATE_OPCODE(name, val) \
        case S0_##name: \
            return s0_evaluate_##name(s, scope, instr, err);
        S0_OPCODES(EVALUATE_OPCODE)
#undef EVALUATE_OPCODE

        default:
            cork_unknown_error_set(swan_alloc(s), err);
            return NULL;
    }
}

struct s0_value *
s0_basic_block_evaluate(struct swan *s, struct s0_basic_block *block,
                        struct cork_error *err)
{
    struct s0_scope  *scope;
    struct s0_value  *last_value = NULL;
    struct cork_dllist_item  *curr;

    rpp_check(scope = s0_scope_new(s, "<top-level>", err));

    for (curr = cork_dllist_start(&block->body);
         !cork_dllist_is_end(&block->body, curr); curr = curr->next) {
        struct s0_instruction  *instr =
            cork_container_of(curr, struct s0_instruction, siblings);
        ep_check(last_value = s0_evaluate_instruction(s, scope, instr, err));
    }

    if (last_value == NULL) {
        s0_empty_file_set(swan_alloc(s), err);
        goto error;
    }

    /* We'll have created a bunch of values, whose references are all
     * held by the scope object.  Once we decref the scope, all of those
     * values will disappear.  We want to hang onto the last value
     * created (and anything reachable from it), since that will be the
     * final result of the evaluation.  So we incref it before
     * destroying the scope. */
    cork_gc_incref(swan_gc(s), last_value);
    cork_gc_decref(swan_gc(s), scope);
    return last_value;

error:
    cork_gc_decref(swan_gc(s), scope);
    return NULL;
}
