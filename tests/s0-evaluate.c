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
#include <libcork/core/checkers.h>
#include <libcork/ds.h>

#include "swanson/s0.h"
#include "swanson/state.h"

#define BUF_SIZE  65536
static char  BUF[BUF_SIZE];

static struct cork_buffer *
read_file(struct swan *s, const char *filename, struct cork_error *err)
{
    FILE  *in = NULL;
    bool  should_close = false;
    size_t  bytes_read;
    struct cork_buffer  *buf;

    buf = cork_buffer_new(swan_alloc(s), err);

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
        rpi_check(cork_buffer_append
                  (swan_alloc(s), buf, BUF, bytes_read, err));
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
print_value(struct swan *s, struct s0_basic_block *block,
            struct s0_value *value, struct cork_error *err)
{
    struct cork_dllist_item  *last_item =
        cork_dllist_tail(&block->body);
    struct s0_instruction  *last_instruction = cork_container_of
        (last_item, struct s0_instruction, siblings);
    s0_tagged_id  last_id =
        last_instruction->dest;

    switch (value->kind) {
        case S0_VALUE_TYPE:
        {
            struct cork_buffer  type = CORK_BUFFER_INIT(swan_alloc(s));
            struct cork_buffer  givens = CORK_BUFFER_INIT(swan_alloc(s));
            ei_check(s0_type_print
                     (s, value->_.type, &type, &givens, err));
            printf("%c%"PRIuPTR" = %s\n",
                   s0_id_tag_name(s0_tagged_id_tag(last_id)),
                   s0_tagged_id_id(last_id), (char *) type.buf);
            if (givens.buf != NULL) {
                printf("\n%s\n", (char *) givens.buf);
            }
            cork_buffer_done(swan_alloc(s), &type);
            cork_buffer_done(swan_alloc(s), &givens);
            return 0;

error:
            cork_buffer_done(swan_alloc(s), &type);
            cork_buffer_done(swan_alloc(s), &givens);
            return -1;
        }

        default:
            cork_unknown_error_set(swan_alloc(s), err);
            return -1;
    }
}

int
main(int argc, char **argv)
{
    struct cork_alloc  *alloc = cork_allocator_new_debug();
    struct cork_error  err = CORK_ERROR_INIT(alloc);
    struct cork_buffer  error_buf = CORK_BUFFER_INIT(alloc);
    struct swan  s;
    struct cork_buffer  *buf;
    struct cork_slice  slice;
    struct s0_basic_block  *block;
    struct s0_value  *value;

    if (argc != 2) {
        fprintf(stderr, "Usage: s0-evaluate [s0 file]\n");
        exit(EXIT_FAILURE);
    }

    swan_init(&s, alloc, NULL);

    /* Read in the S0 file */
    ep_check(buf = read_file(&s, argv[1], &err));

    /* Then parse it */
    ei_check(cork_buffer_to_slice(alloc, buf, &slice, &err));
    ep_check(block = s0_parse(&s, &slice, &err));

    /* Finally, evaluate it and print it */
    ep_check(value = s0_basic_block_evaluate(&s, block, &err));
    ei_check(print_value(&s, block, value, &err));

    swan_done(&s);
    cork_error_done(alloc, &err);
    cork_allocator_free(alloc);

    return 0;

error:
    cork_error_message(alloc, &err, &error_buf);
    fprintf(stderr, "%s\n", (char *) error_buf.buf);
    exit(EXIT_FAILURE);
}
