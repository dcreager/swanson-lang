/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libcork/core.h>
#include <libcork/helpers/errors.h>
#include <libcork/ds.h>

#include "swanson/engine.h"
#include "swanson/s0.h"
#include "swanson/state.h"

#define BUF_SIZE  65536
static char  BUF[BUF_SIZE];

static struct cork_buffer *
read_file(struct swan *s, const char *filename)
{
    FILE  *in = NULL;
    bool  should_close = false;
    size_t  bytes_read;
    struct cork_buffer  *buf;

    buf = cork_buffer_new();

    if (strcmp(filename, "-") == 0) {
        /* Read from stdin */
        in = stdin;
        should_close = false;
    } else {
        in = fopen(filename, "r");
        if (in == NULL) {
            goto io_error;
        }
        should_close = true;
    }

    while ((bytes_read = fread(BUF, 1, BUF_SIZE, in)) != 0) {
        cork_buffer_append(buf, BUF, bytes_read);
    }

    if (should_close) {
        fclose(in);
    }

    return buf;

io_error:
    fprintf(stderr, "%s\n", strerror(errno));
    exit(EXIT_FAILURE);
}


static int
check_types(struct swan *s, struct s0_value *value)
{
    if (value->kind != S0_VALUE_TUPLE ||
        cork_array_size(&value->_.tuple) != 2) {
        fprintf(stderr, "Expected a tuple of two types\n");
        exit(EXIT_FAILURE);
    }

    struct s0_value  *v0 = s0_tuple_value_get_fast(value, 0);
    struct s0_value  *v1 = s0_tuple_value_get_fast(value, 1);

    if (v0->kind != S0_VALUE_TYPE || v1->kind != S0_VALUE_TYPE) {
        fprintf(stderr, "Expected a tuple of two types\n");
        exit(EXIT_FAILURE);
    }

    bool  result = s0_type_satisfies(s, v0->_.type, v1->_.type);
    if (cork_error_occurred()) {
        return -1;
    }

    s0_type_array  types;
    s0_buffer_array  dests;
    struct cork_buffer  givens = CORK_BUFFER_INIT();

    cork_array_init(&types);
    cork_array_init(&dests);
    rii_check(cork_array_append(&types, v0->_.type));
    rii_check(cork_array_append(&types, v1->_.type));

    rii_check(s0_type_print_many(s, &types, &dests, &givens));
    struct cork_buffer  *b0 = cork_array_at(&dests, 0);
    struct cork_buffer  *b1 = cork_array_at(&dests, 1);

    printf("%s %s %s\n",
           (char *) b0->buf, result? "satisfies": "DOES NOT satsify",
           (char *) b1->buf);
    if (givens.buf != NULL) {
        printf("\n%s\n", (char *) givens.buf);
    }

    cork_buffer_done(&givens);
    cork_buffer_free(b0);
    cork_buffer_free(b1);
    cork_array_done(&types);
    cork_array_done(&dests);
    return 0;
}


int
main(int argc, char **argv)
{
    struct swan  s;
    struct cork_buffer  *buf;
    struct cork_slice  slice;
    struct s0_basic_block  *block;
    struct s0_value  *result;

    if (argc != 2) {
        fprintf(stderr, "Usage: type-satisfies [s0 file]\n");
        exit(EXIT_FAILURE);
    }

    swan_init(&s);

    /* Read in the S0 file */
    ep_check(buf = read_file(&s, argv[1]));

    /* Then parse it */
    ei_check(cork_buffer_to_slice(buf, &slice));
    ep_check(block = s0_parse(&s, &slice));

    if (cork_array_is_empty(&block->body)) {
        printf("File is empty\n");
        exit(EXIT_FAILURE);
    }

    /* Create the prelude */
    ep_check(block->upvalue = swan_prelude_new(&s));

    /* Finally, evaluate it and print it */
    ep_check(result = s0_basic_block_evaluate(&s, block, NULL));
    ei_check(check_types(&s, result));

    swan_done(&s);
    return 0;

error:
    fprintf(stderr, "%s\n", cork_error_message());
    exit(EXIT_FAILURE);
}

