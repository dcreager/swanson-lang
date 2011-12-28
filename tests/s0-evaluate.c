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
print_type(struct swan *s, struct s0_type *type, struct cork_error *err)
{
    struct cork_buffer  type_buf = CORK_BUFFER_INIT(swan_alloc(s));
    struct cork_buffer  givens_buf = CORK_BUFFER_INIT(swan_alloc(s));
    ei_check(s0_type_print(s, type, &type_buf, &givens_buf, err));
    printf("%s\n", (char *) type_buf.buf);
    if (givens_buf.buf != NULL) {
        printf("\n%s\n", (char *) givens_buf.buf);
    }
    cork_buffer_done(swan_alloc(s), &type_buf);
    cork_buffer_done(swan_alloc(s), &givens_buf);
    return 0;

error:
    cork_buffer_done(swan_alloc(s), &type_buf);
    cork_buffer_done(swan_alloc(s), &givens_buf);
    return -1;
}

static int
print_value(struct swan *s, struct s0_basic_block *block,
            struct s0_value *value, struct cork_error *err)
{
    struct s0_instruction  *last_instruction =
        cork_array_at(&block->body, cork_array_size(&block->body) - 1);
    s0_tagged_id  last_id = last_instruction->_.ret.result;

    switch (value->kind) {
        case S0_VALUE_TYPE:
        {
            printf("%c%"PRIuPTR" = ",
                   s0_id_tag_name(s0_tagged_id_tag(last_id)),
                   s0_tagged_id_id(last_id));
            return print_type(s, value->_.type, err);
        }

        case S0_VALUE_LITERAL:
        {
            printf("%c%"PRIuPTR" = LITERAL:\n%s\n",
                   s0_id_tag_name(s0_tagged_id_tag(last_id)),
                   s0_tagged_id_id(last_id), value->_.literal);
            return 0;
        }

        case S0_VALUE_MACRO:
        {
            struct s0_type  *type;
            rip_check(type = s0_value_get_type(s, value, err));
            printf("%c%"PRIuPTR" = MACRO\n"
                   "%c%"PRIuPTR" :: ",
                   s0_id_tag_name(s0_tagged_id_tag(last_id)),
                   s0_tagged_id_id(last_id),
                   s0_id_tag_name(s0_tagged_id_tag(last_id)),
                   s0_tagged_id_id(last_id));
            return print_type(s, type, err);
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
    struct s0_value  *result;

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

    if (cork_array_is_empty(&block->body)) {
        printf("File is empty\n");
        exit(EXIT_FAILURE);
    }

    /* Finally, evaluate it and print it */
    ep_check(result = s0_basic_block_evaluate(&s, block, NULL, &err));
    ei_check(print_value(&s, block, result, &err));

    swan_done(&s);
    cork_error_done(alloc, &err);
    cork_allocator_free(alloc);

    return 0;

error:
    cork_error_message(alloc, &err, &error_buf);
    fprintf(stderr, "%s\n", (char *) error_buf.buf);
    exit(EXIT_FAILURE);
}
