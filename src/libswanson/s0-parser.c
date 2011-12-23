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
    if (extra->expected_tag == '\0') {
        return cork_buffer_printf
            (alloc, dest, NULL, "Expected identifier at %zu.%zu",
             extra->pos.line, extra->pos.column);
    } else {
        return cork_buffer_printf
            (alloc, dest, NULL, "Expected %c identifier at %zu.%zu",
             extra->expected_tag, extra->pos.line, extra->pos.column);
    }
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


struct s0_bad_symbol_extra {
    struct s0_position  pos;
    const char  *symbol;
};

static int
s0_bad_symbol(struct cork_alloc *alloc, struct cork_error *err,
              struct cork_buffer *dest)
{
    struct s0_bad_symbol_extra  *extra = cork_error_extra(err);
    return cork_buffer_printf
        (alloc, dest, NULL, "Expected %s at %zu.%zu",
         extra->symbol, extra->pos.line, extra->pos.column);
}

static int
s0_bad_symbol_set(struct cork_alloc *alloc, struct cork_error *err,
                  struct s0_position pos, const char *symbol)
{
    struct s0_bad_symbol_extra  extra = { pos, symbol };
    return cork_error_set_extra(alloc, err,
                                S0_ERROR,
                                S0_SYNTAX_ERROR,
                                s0_bad_symbol,
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


/* Tries to parse a particular symbol.  If that symbol doesn't appear
 * next in the input, don't consider it an error, and consume anything. */
static int
s0_parse_try_symbol(struct swan *s, struct s0_parser *sp,
                    const char *symbol, size_t symbol_length,
                    struct cork_error *err)
{
    rii_check(s0_parse_skip_leading(s, sp, err));
    DEBUG("[%4zu:%2zu] Trying to parse symbol %s",
          sp->pos.line, sp->pos.column, symbol);

    if ((sp->slice->size < symbol_length) ||
        (memcmp(sp->slice->buf, symbol, symbol_length) != 0)) {
        /* Nope, that symbol isn't next. */
        return -2;
    }

    return cork_slice_slice_offset
        (swan_alloc(s), sp->slice, symbol_length, err);
}

/* Tries to parse a particular symbol, raising a syntax error if it's
 * not present. */
static int
s0_parse_require_symbol(struct swan *s, struct s0_parser *sp,
                        const char *symbol, size_t symbol_length,
                        struct cork_error *err)
{
    int  rc = s0_parse_try_symbol(s, sp, symbol, symbol_length, err);
    if (rc == -2) {
        s0_bad_symbol_set(swan_alloc(s), err, sp->pos, symbol);
        return -1;
    } else {
        return rc;
    }
}


/* Tries to parse an operand name */
static int
s0_parse_operand_name(struct swan *s, struct s0_parser *sp,
                      enum s0_opcode *dest, struct cork_error *err)
{
    /* Don't need to skip whitespace; that's handled below in s0_parse */
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

/* Parse any identifier. */
static int
s0_parse_any_id(struct swan *s, struct s0_parser *sp,
                s0_tagged_id *dest, struct cork_error *err)
{
    rii_check(s0_parse_skip_leading(s, sp, err));
    DEBUG("[%4zu:%2zu] Trying to parse %c identifier",
          sp->pos.line, sp->pos.column, (int) tag);

    if (sp->slice->size == 0) {
        s0_bad_tag_set(swan_alloc(s), err, sp->pos, '\0');
        return -1;
    }

    int  tag;
    switch (*((char *) sp->slice->buf)) {
        case '%':
            tag = S0_ID_TAG_TYPE;
            break;

        default:
            s0_bad_tag_set(swan_alloc(s), err, sp->pos, '\0');
            return -1;
    }

    s0_id  id;
    rii_check(s0_parse_id_int(s, sp, '\0', &id, err));
    *dest = s0_tagged_id(tag, id);
    return 0;
}

/* Parse a list of identifiers. */
static int
s0_parse_id_list(struct swan *s, struct s0_parser *sp,
                 s0_tagged_id_array *dest, struct cork_error *err)
{
    rii_check(s0_parse_require_symbol(s, sp, "(", 1, err));
    bool  first = true;

    do {
        int  rc;

        /* At this point, we can try to read a close-paren to end the
         * list. */
        rc = s0_parse_try_symbol(s, sp, ")", 1, err);
        if (rc != -2) {
            return rc;
        }

        /* If this isn't the first element of the list, there needs to
         * be a comma next. */
        if (first) {
            first = false;
        } else {
            rii_check(s0_parse_require_symbol(s, sp, ",", 1, err));
        }

        /* And then we have to parse an identifier. */
        s0_tagged_id  id;
        rii_check(s0_parse_any_id(s, sp, &id, err));
        rii_check(cork_array_append(swan_alloc(s), dest, id, err));
    } while (true);

    /* Should be unreachable */
    cork_unknown_error_set(swan_alloc(s), err);
    return -1;
}


/* A bunch of parsers for each opcode.  Each of these assume that the
 * operand name has already been parsed, since we'll need to switch on
 * that before these functions are ever called. */

static int
s0_parse_TRECURSIVE(struct swan *s, struct s0_parser *sp,
                    struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, '%', &dest, err));
    rii_check(s0_parse_require_symbol(s, sp, ";", 1, err));
    rip_check(instr = s0_trecursive_new(s, dest, err));
    rii_check(s0_basic_block_add(s, sp->block, instr, err));
    return 0;
}

static int
s0_parse_TLITERAL(struct swan *s, struct s0_parser *sp,
                  struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, '%', &dest, err));
    rii_check(s0_parse_require_symbol(s, sp, ";", 1, err));
    rip_check(instr = s0_tliteral_new(s, dest, err));
    rii_check(s0_basic_block_add(s, sp->block, instr, err));
    return 0;
}

static int
s0_parse_TFUNCTION(struct swan *s, struct s0_parser *sp,
                   struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, '%', &dest, err));
    rii_check(s0_parse_require_symbol(s, sp, "=", 1, err));
    rip_check(instr = s0_tfunction_new(s, dest, err));
    ei_check(s0_parse_id_list(s, sp, &instr->args.tfunction.params, err));
    ei_check(s0_parse_require_symbol(s, sp, "->", 2, err));
    ei_check(s0_parse_id_list(s, sp, &instr->args.tfunction.results, err));
    ei_check(s0_parse_require_symbol(s, sp, ";", 1, err));
    ei_check(s0_basic_block_add(s, sp->block, instr, err));
    return 0;

error:
    cork_gc_decref(swan_gc(s), instr);
    return -1;
}

static int
s0_parse_TLOCATION(struct swan *s, struct s0_parser *sp,
                   struct cork_error *err)
{
    s0_id  dest;
    s0_tagged_id  referent;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, '%', &dest, err));
    rii_check(s0_parse_require_symbol(s, sp, "=", 1, err));
    rii_check(s0_parse_any_id(s, sp, &referent, err));
    rii_check(s0_parse_require_symbol(s, sp, ";", 1, err));
    rip_check(instr = s0_tlocation_new(s, dest, referent, err));
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
#define PARSE_OPCODE(name, val) \
        case S0_##name: \
            return s0_parse_##name(s, sp, err);
        S0_OPCODES(PARSE_OPCODE)
#undef PARSE_OPCODE

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

    ei_check(s0_parse_skip_leading(s, &sp, err));
    while (src->size > 0) {
        ei_check(s0_parse_instruction(s, &sp, err));
        ei_check(s0_parse_skip_leading(s, &sp, err));
    }
    return result;

error:
    cork_gc_decref(swan_gc(s), result);
    return NULL;
}
