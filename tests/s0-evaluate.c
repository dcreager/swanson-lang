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
print_type(struct swan *s, struct s0_type *type)
{
    struct cork_buffer  type_buf = CORK_BUFFER_INIT();
    struct cork_buffer  givens_buf = CORK_BUFFER_INIT();
    ei_check(s0_type_print(s, type, &type_buf, &givens_buf));
    printf("%s\n", (char *) type_buf.buf);
    if (givens_buf.buf != NULL) {
        printf("\n%s\n", (char *) givens_buf.buf);
    }
    cork_buffer_done(&type_buf);
    cork_buffer_done(&givens_buf);
    return 0;

error:
    cork_buffer_done(&type_buf);
    cork_buffer_done(&givens_buf);
    return -1;
}

static int
print_value(struct swan *s, struct cork_buffer *prefix,
            struct s0_value *value)
{
    switch (value->kind) {
        case S0_VALUE_TYPE:
        {
            printf("%s = ", (char *) prefix->buf);
            return print_type(s, value->_.type);
        }

        case S0_VALUE_LITERAL:
        {
            printf("%s = LITERAL:\n%s\n",
                   (char *) prefix->buf, value->_.literal);
            return 0;
        }

        case S0_VALUE_MACRO:
        {
            struct s0_type  *type;
            rip_check(type = s0_value_get_type(s, value));
            printf("%s = MACRO\n%s :: ",
                   (char *) prefix->buf, (char *) prefix->buf);
            return print_type(s, type);
        }

        case S0_VALUE_TUPLE:
        {
            size_t  i;
            size_t  count = cork_array_size(&value->_.tuple);
            printf("%s = TUPLE(%zu)\n", (char *) prefix->buf, count);
            for (i = 0; i < count; i++) {
                struct s0_value  *element = cork_array_at(&value->_.tuple, i);
                cork_buffer_printf(prefix, "[%zu]", i);
                printf("\n");
                rii_check(print_value(s, prefix, element));
            }
            return 0;
        }

        case S0_VALUE_C:
        {
            struct s0_type  *type;
            rip_check(type = s0_value_get_type(s, value));
            printf("%s = C FUNCTION\n%s :: ",
                   (char *) prefix->buf, (char *) prefix->buf);
            return print_type(s, type);
        }

        case S0_VALUE_OBJECT:
        {
            struct s0_type  *type;
            rip_check(type = s0_value_get_type(s, value));
            printf("%s = OBJECT\n%s :: ",
                   (char *) prefix->buf, (char *) prefix->buf);
            return print_type(s, type);
        }

        default:
            cork_abort("%s", "Unknown type of value");
    }
}

static int
print_result(struct swan *s, struct s0_basic_block *block,
             struct s0_value *value)
{
    struct s0_instruction  *last_instruction =
        cork_array_at(&block->body, cork_array_size(&block->body) - 1);
    s0_tagged_id  last_id = last_instruction->_.ret.result;

    struct cork_buffer  prefix = CORK_BUFFER_INIT();
    cork_buffer_printf
        (&prefix, "%c%"PRIuPTR,
         s0_id_tag_name(s0_tagged_id_tag(last_id)), s0_tagged_id_id(last_id));
    return print_value(s, &prefix, value);
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
        fprintf(stderr, "Usage: s0-evaluate [s0 file]\n");
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
    ep_check(block->upvalue = swan_old_prelude_new(&s));

    /* Finally, evaluate it and print it */
    ep_check(result = s0_basic_block_evaluate(&s, block, NULL));
    ei_check(print_result(&s, block, result));

    swan_done(&s);

    return 0;

error:
    fprintf(stderr, "%s\n", cork_error_message());
    exit(EXIT_FAILURE);
}
