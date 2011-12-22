/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <libcork/core.h>
#include <libcork/core/checkers.h>
#include <libcork/ds.h>

#include "swanson/s0.h"
#include "swanson/state.h"

#if !defined(PARSE_DEBUG)
#define PARSE_DEBUG 0
#endif

#if PARSE_DEBUG
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
 * Error handling
 */

struct s0_bad_operand_extra {
    struct s0_position  pos;
    const char  *operand;
    size_t  operand_length;
};

static int
s0_bad_operand(struct cork_alloc *alloc, struct cork_error *err,
               struct cork_buffer *dest)
{
    struct s0_bad_operand_extra  *extra = cork_error_extra(err);
    rii_check(cork_buffer_printf
              (alloc, dest, NULL, "Unknown S0 operand at %zu.%zu: ",
               extra->pos.line, extra->pos.column));
    rii_check(cork_buffer_append
              (alloc, dest, extra->operand, extra->operand_length, NULL));
    return 0;
}

static int
s0_bad_operand_set(struct cork_alloc *alloc, struct cork_error *err,
                   struct s0_position pos, const char *operand,
                   size_t operand_length)
{
    struct s0_bad_operand_extra  extra = { pos, operand, operand_length };
    return cork_error_set_extra(alloc, err,
                                S0_ERROR,
                                S0_SYNTAX_ERROR,
                                s0_bad_operand,
                                extra);
}


struct s0_bad_tag_extra {
    struct s0_position  pos;
    char  expected_tag;
};

static int
s0_bad_tag(struct cork_alloc *alloc, struct cork_error *err,
           struct cork_buffer *dest)
{
    struct s0_bad_tag_extra  *extra = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL, "Expected a %c identifier at %zu.%zu",
         extra->expected_tag, extra->pos.line, extra->pos.column);
}

static int
s0_bad_tag_set(struct cork_alloc *alloc, struct cork_error *err,
               struct s0_position pos, char expected_tag)
{
    struct s0_bad_tag_extra  extra = { pos, expected_tag };
    return cork_error_set_extra(alloc, err,
                                S0_ERROR,
                                S0_SYNTAX_ERROR,
                                s0_bad_tag,
                                extra);
}


/*-----------------------------------------------------------------------
 * Parser
 */

static const char  *OPERAND_NAMES[] = {
#define DEFINE_NAME(name, val)  #name,
    S0_OPCODES(DEFINE_NAME)
#undef DEFINE_NAME
    NULL
};


#define s0_position_init(pos) \
    do { \
        (pos)->column = 0; \
        (pos)->line = 0; \
    } while (0)

#define s0_position_advance_char(pos, ch) \
    do { \
        if ((ch) == '\n') { \
            (pos)->column = 0; \
            (pos)->line++; \
        } else { \
            (pos)->column++; \
        } \
    } while (0)

#define s0_advance_char(i, curr, pos) \
    do { \
        (i)++; \
        s0_position_advance_char((pos), *(curr)); \
        (curr)++; \
    } while (0)


struct s0_parser {
    struct cork_slice  *slice;
    struct s0_position  pos;
    struct cork_buffer  scratch;
    struct s0_basic_block  *block;
};


/* Skips over a comment. */
static int
s0_parse_skip_comment(struct swan *s, struct s0_parser *sp,
                      struct cork_error *err)
{
    size_t  i = 0;
    const char  *curr = sp->slice->buf;
    DEBUG("[%4zu:%2zu] Skipping comment", sp->pos.line, sp->pos.column);

    /* Assume that the first character is a #.  This will be checked
     * elsewhere. */
    s0_advance_char(i, curr, &sp->pos);

    /* Find the trailing newline. */
    while (i < sp->slice->size) {
        if (*curr == '\n') {
            /* Skip past the trailing newline before returning */
            s0_advance_char(i, curr, &sp->pos);
            break;
        } else {
            s0_advance_char(i, curr, &sp->pos);
        }
    }

    /* i is now the index of the first character after the comment. */
    return cork_slice_slice_offset(swan_alloc(s), sp->slice, i, err);
}

/* Skips any whitespace at the beginning of slice.  Afterwards, the
 * start of slice is guaranteed to be a non-whitespace character. */
static int
s0_parse_skip_whitespace(struct swan *s, struct s0_parser *sp,
                         struct cork_error *err)
{
    size_t  i = 0;
    const char  *curr = sp->slice->buf;
    DEBUG("[%4zu:%2zu] Skipping whitespace", sp->pos.line, sp->pos.column);

    /* Assume that the first character is whitespace.  This will be
     * checked elsewhere. */
    s0_advance_char(i, curr, &sp->pos);

    while (i < sp->slice->size && isspace(*curr)) {
        s0_advance_char(i, curr, &sp->pos);
    }

    /* i is now the index of the first non-whitespace character (which
     * might be one past the end of the slice if the slice is all
     * whitespace) */
    return cork_slice_slice_offset(swan_alloc(s), sp->slice, i, err);
}

/* Skips any "leading" text — any combination of whitespace or comments. */
static int
s0_parse_skip_leading(struct swan *s, struct s0_parser *sp,
                      struct cork_error *err)
{
    while (sp->slice->size > 0) {
        char  ch = *((char *) sp->slice->buf);
        if (ch == '#') {
            rii_check(s0_parse_skip_comment(s, sp, err));
        } else if (isspace(ch)) {
            rii_check(s0_parse_skip_whitespace(s, sp, err));
        } else {
            return 0;
        }
    }

    return 0;
}


/* Tries to parse an operand name */
static int
s0_parse_operand_name(struct swan *s, struct s0_parser *sp,
                      enum s0_opcode *dest, struct cork_error *err)
{
    rii_check(s0_parse_skip_leading(s, sp, err));
    DEBUG("[%4zu:%2zu] Trying to parse operand name",
          sp->pos.line, sp->pos.column);
    struct s0_position  start = sp->pos;

    size_t  i = 0;
    const char  *curr = sp->slice->buf;
    while (i < sp->slice->size && isalpha(*curr)) {
        s0_advance_char(i, curr, &sp->pos);
    }

    /* i now points just past a string of alpha characters */
    rii_check(cork_buffer_set
              (swan_alloc(s), &sp->scratch, sp->slice->buf, i, err));

    /* See if this represents a valid operand name */
    size_t  j;
    for (j = 0; j < S0_OPCODE_COUNT; j++) {
        if (strcasecmp(sp->scratch.buf, OPERAND_NAMES[j]) == 0) {
            *dest = j;
            DEBUG("[%4zu:%2zu]   %s",
                  start.line, start.column, OPERAND_NAMES[j]);
            return cork_slice_slice_offset(swan_alloc(s), sp->slice, i, err);
        }
    }

    s0_bad_operand_set(swan_alloc(s), err, start, sp->slice->buf, i);
    return -1;
}


/* Parse an integer */
static int
s0_parse_id_int(struct swan *s, struct s0_parser *sp,
                char tag, uintptr_t *dest, struct cork_error *err)
{
    size_t  i = 0;
    const char  *curr = sp->slice->buf;
    struct s0_position  start = sp->pos;

    /* We DON'T skip over whitespace, since we're going to parse an
     * integer immediately after an identifier tag. */

    /* We assume that the first character is a valid identifier tag. */
    s0_advance_char(i, curr, &sp->pos);

    while (i < sp->slice->size && isdigit(*curr)) {
        s0_advance_char(i, curr, &sp->pos);
    }

    /* i is now points just past the end of the identifier */

    /* Put the integer text into the scratch buffer so that it's NUL
     * terminated */
    rii_check(cork_buffer_set
              (swan_alloc(s), &sp->scratch, sp->slice->buf+1, i-1, err));

    /* Parse the integer value, onlying allowing decimal */
    char  *endptr;
    long  l_value = strtol(sp->scratch.buf, &endptr, 10);
    if (*endptr != '\0' || errno == ERANGE ||
        l_value < 0 || l_value > UINTPTR_MAX) {
        s0_bad_tag_set(swan_alloc(s), err, start, tag);
        return -1;
    }

    *dest = l_value;
    return cork_slice_slice_offset(swan_alloc(s), sp->slice, i, err);
}

/* Parse an identifier with the given tag. */
static int
s0_parse_id(struct swan *s, struct s0_parser *sp, char tag,
            s0_id *dest, struct cork_error *err)
{
    rii_check(s0_parse_skip_leading(s, sp, err));
    DEBUG("[%4zu:%2zu] Trying to parse %c identifier",
          sp->pos.line, sp->pos.column, (int) tag);

    if ((sp->slice->size == 0) || (*((char *) sp->slice->buf) != tag)) {
        s0_bad_tag_set(swan_alloc(s), err, sp->pos, tag);
        return -1;
    }

    return s0_parse_id_int(s, sp, tag, dest, err);
}


/* A bunch of parsers for each opcode.  Each of these assume that the
 * operand name has already been parsed, since we'll need to switch on
 * that before these functions are ever called. */

static int
s0_parse_tliteral(struct swan *s, struct s0_parser *sp,
                  struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, '%', &dest, err));
    rip_check(instr = s0_tliteral_new(s, dest, err));
    rii_check(s0_basic_block_add(s, sp->block, instr, err));
    return 0;
}


/* Parse a single instruction */
static int
s0_parse_instruction(struct swan *s, struct s0_parser *sp,
                     struct cork_error *err)
{
    enum s0_opcode  op;
    rii_check(s0_parse_operand_name(s, sp, &op, err));

    switch (op) {
        case S0_TLITERAL:
            return s0_parse_tliteral(s, sp, err);

        default:
            cork_unknown_error_set(swan_alloc(s), err);
            return -1;
    }
}


struct s0_basic_block *
s0_parse(struct swan *s, struct cork_slice *src, struct cork_error *err)
{
    struct s0_basic_block  *result;
    rpp_check(result = s0_basic_block_new(s, err));

    struct s0_parser  sp = {
        src, {0,0},
        CORK_BUFFER_INIT(swan_alloc(s)),
        result
    };

    ei_check(s0_parse_instruction(s, &sp, err));

error:
    cork_gc_decref(swan_gc(s), result);
    return NULL;
}
