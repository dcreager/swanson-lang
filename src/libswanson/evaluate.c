/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <libcork/core.h>
#include <libcork/ds.h>
#include <libcork/helpers/errors.h>
#include <libcork/helpers/gc.h>

#include "swanson/ast.h"
#include "swanson/metamodel.h"
#include "swanson/state.h"


#if !defined(EVAL_DEBUG)
#define EVAL_DEBUG 0
#endif

#if EVAL_DEBUG
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
 * Symbol tables
 */

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

static void
swan_stable_init(struct cork_hash_table *stable)
{
    cork_hash_table_init(stable, 0, string_hasher, string_comparator);
}

static enum cork_hash_table_map_result
swan_stable_free_entries(struct cork_hash_table_entry *entry, void *ud)
{
    //cork_gc_decref(entry->value);
    return CORK_HASH_TABLE_MAP_DELETE;
}

static void
swan_stable_done(struct cork_hash_table *stable)
{
    cork_hash_table_map(stable, swan_stable_free_entries, NULL);
    cork_hash_table_done(stable);
}

static int
swan_stable_add(struct cork_hash_table *stable,
                const char *name, struct swan_thing *t)
{
    bool  is_new;
    struct cork_hash_table_entry  *entry =
        cork_hash_table_get_or_create(stable, (void *) name, &is_new);

    if (!is_new) {
        cork_error_set
            (SWAN_METAMODEL_ERROR, SWAN_METAMODEL_REDEFINED,
             "%s redefined", name);
        cork_gc_decref(t);
        return -1;
    }

    entry->key = (void *) name;
    entry->value = t;
    return 0;
}

static struct swan_thing *
swan_stable_get(struct cork_hash_table *stable, const char *name)
{
    struct swan_thing  *result = cork_hash_table_get(stable, name);
    if (result == NULL) {
        cork_error_set
            (SWAN_METAMODEL_ERROR, SWAN_METAMODEL_UNDEFINED,
             "No local named %s", name);
        return NULL;
    } else {
        return result;
    }
}


typedef cork_array(struct swan_thing *)  swan_thing_array;

static int
swan_ast_evaluate_call(struct swan *s, struct swan_ast_call *call,
                       swan_thing_array *params, swan_thing_array *results,
                       struct cork_hash_table *stable)
{
    size_t  i;
    size_t  param_count;
    size_t  result_count;
    struct swan_thing  *target;

    DEBUG("  Call");
    DEBUG("    Target: %s", call->thing->contents);
    DEBUG("    Method: %s", call->method->contents);
    rip_check(target = swan_stable_get(stable, call->thing->contents));

    param_count = cork_array_size(&call->params);
    result_count = cork_array_size(&call->results);

    cork_array_ensure_size(params, param_count);
    cork_array_ensure_size(results, result_count);

    for (i = 0; i < param_count; i++) {
        const char  *id =
            cork_array_at(&call->params, i)->contents;
        rip_check(cork_array_at(params, i) = swan_stable_get(stable, id));
    }

    rii_check(swan_thing_call_method
              (s, target->b, call->method->contents,
               param_count, &cork_array_at(params, 0),
               result_count, &cork_array_at(results, 0)));

    for (i = 0; i < result_count; i++) {
        const char  *id =
            cork_array_at(&call->results, i)->contents;
        rii_check(swan_stable_add(stable, id, cork_array_at(results, i)));
    }

    return 0;
}

static int
swan_ast_evaluate_string(struct swan *s, struct swan_ast_string *string,
                         struct cork_hash_table *stable)
{
    DEBUG("  String");
    DEBUG("    Dest:     %s", string->result->contents);
    DEBUG("    Contents: %s", string->contents->contents);

    return swan_stable_add
        (stable, string->result->contents, &string->contents->parent);
}


int
swan_ast_evaluate(struct swan *s, struct swan_ast *self)
{
    CORK_ATTR_UNUSED size_t  call_index = 0;
    struct cork_hash_table  stable;
    struct cork_dllist_item  *curr;
    swan_thing_array  params;
    swan_thing_array  results;

    /* Set up things for all of the method calls */
    cork_array_init(&params);
    cork_array_init(&results);

    /* Add the block's upvalues to the symbol table */
    swan_stable_init(&stable);
    for (curr = cork_dllist_start(&self->upvalues);
         !cork_dllist_is_end(&self->upvalues, curr); curr = curr->next) {
        struct swan_ast_upvalue  *upvalue =
            cork_container_of(curr, struct swan_ast_upvalue, list);
        ei_check(swan_stable_add(&stable, upvalue->name, upvalue->value));
    }

    /* Make each method call */
    for (curr = cork_dllist_start(&self->elements);
         !cork_dllist_is_end(&self->elements, curr); curr = curr->next) {
        DEBUG("Element %zu", call_index++);
        struct swan_ast_element  *element =
            cork_container_of(curr, struct swan_ast_element, list);

        switch (element->element_type) {
            case swan_ast_call___breed_id:
            {
                struct swan_ast_call  *call =
                    cork_container_of(element, struct swan_ast_call, parent);
                ei_check(swan_ast_evaluate_call
                         (s, call, &params, &results, &stable));
                break;
            }

            case swan_ast_string___breed_id:
            {
                struct swan_ast_string  *string =
                    cork_container_of(element, struct swan_ast_string, parent);
                ei_check(swan_ast_evaluate_string(s, string, &stable));
                break;
            }

            default:
                cork_unknown_error();
                abort();
        }
    }

    swan_stable_done(&stable);
    cork_array_done(&params);
    cork_array_done(&results);
    return 0;

error:
    swan_stable_done(&stable);
    cork_array_done(&params);
    cork_array_done(&results);
    return -1;
}
