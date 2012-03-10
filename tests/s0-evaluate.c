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

#include "swanson/ast.h"
#include "swanson/ds.h"
#include "swanson/metamodel.h"
#include "swanson/prelude.h"
#include "swanson/state.h"

#define BUF_SIZE  65536
static char  BUF[BUF_SIZE];

static void
read_file(struct swan *s, const char *filename, struct cork_buffer *buf)
{
    FILE  *in = NULL;
    bool  should_close = false;
    size_t  bytes_read;

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

    if (ferror(in)) {
        goto io_error;
    }

    if (should_close) {
        fclose(in);
    }

    return;

io_error:
    fprintf(stderr, "%s\n", strerror(errno));
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    struct swan  s;
    struct cork_buffer  buf = CORK_BUFFER_INIT();
    struct swan_thing  *prelude;
    struct swan_ast  *ast;

    if (argc != 2) {
        fprintf(stderr, "Usage: s0-evaluate [s0 file]\n");
        exit(EXIT_FAILURE);
    }

    swan_init(&s);

    /* Read in the S0 file */
    read_file(&s, argv[1], &buf);

    /* Then parse it */
    ast = swan_ast_parse(&s, buf.buf, buf.size);
    if (cork_error_occurred()) {
        goto error;
    }

    if (cork_dllist_is_empty(&ast->elements)) {
        printf("File is empty\n");
        exit(EXIT_FAILURE);
    }

    /* Create the prelude */
    prelude = swan_prelude_new(&s);
    swan_ast_add_upvalue(&s, ast, "prelude", prelude);

    /* Finally, evaluate it and print it */
    ei_check(swan_ast_evaluate(&s, ast));

    swan_done(&s);
    return 0;

error:
    fprintf(stderr, "Error: %s\n", cork_error_message());
    exit(EXIT_FAILURE);
}
