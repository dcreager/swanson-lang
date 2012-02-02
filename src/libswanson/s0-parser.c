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
 * Parser
 */

static const char  *OPERAND_NAMES[] = {
#define DEFINE_NAME(name)  #name,
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
    s0_instruction_array  *body;
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
    return cork_slice_slice_offset(sp->slice, i, err);
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
    return cork_slice_slice_offset(sp->slice, i, err);
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


/* Tries to parse a particular token.  If that token doesn't appear next
 * in the input, don't consider it an error, and don't consume anything. */
static int
s0_parse_try_token(struct swan *s, struct s0_parser *sp,
                   const char *token, size_t token_length,
                   struct cork_error *err)
{
    rii_check(s0_parse_skip_leading(s, sp, err));
    DEBUG("[%4zu:%2zu] Trying to parse token %s",
          sp->pos.line, sp->pos.column, token);

    if ((sp->slice->size < token_length) ||
        (strncasecmp(sp->slice->buf, token, token_length) != 0)) {
        /* Nope, that token isn't next. */
        return -2;
    }

    return cork_slice_slice_offset
        (sp->slice, token_length, err);
}

/* Tries to parse a particular token, raising a syntax error if it's
 * not present. */
static int
s0_parse_require_token(struct swan *s, struct s0_parser *sp,
                       const char *token, size_t token_length,
                       struct cork_error *err)
{
    int  rc = s0_parse_try_token(s, sp, token, token_length, err);
    if (rc == -2) {
        cork_error_set
            (err, S0_ERROR, S0_SYNTAX_ERROR,
             "Expected %s at %zu.%zu",
             token, sp->pos.line, sp->pos.column);
        return -1;
    } else {
        return rc;
    }
}


/* Tries to parse a string literal.  We don't support the full range of
 * C escape sequences; the only things that need to be escaped are the
 * closing quote and the escape character itself.  Everything can be
 * included into the string literal directly.  The result will be in the
 * scratch buffer. */
static int
s0_parse_string(struct swan *s, struct s0_parser *sp,
                struct cork_error *err)
{
    rii_check(s0_parse_skip_leading(s, sp, err));
    DEBUG("[%4zu:%2zu] Trying to parse string literal",
          sp->pos.line, sp->pos.column);
    struct s0_position  start = sp->pos;

    size_t  i = 0;
    const char  *curr = sp->slice->buf;

    if (sp->slice->size == 0 || *curr != '"') {
        cork_error_set
            (err, S0_ERROR, S0_SYNTAX_ERROR,
             "Expected string literal at %zu.%zu",
             start.line, start.column);
        return -1;
    }

    /* Advance past the opening quote */
    s0_advance_char(i, curr, &sp->pos);
    cork_buffer_clear(&sp->scratch);

    while (i < sp->slice->size) {
        /* Closing quote? */
        if (*curr == '"') {
            s0_advance_char(i, curr, &sp->pos);
            return cork_slice_slice_offset
                (sp->slice, i, err);
        }

        /* Escaped character? */
        if (*curr == '\\') {
            s0_advance_char(i, curr, &sp->pos);
            if (sp->slice->size == 0) {
                cork_error_set
                    (err, S0_ERROR, S0_SYNTAX_ERROR,
                     "Unterminated string literal at %zu.%zu",
                     start.line, start.column);
                return -1;
            }
        }

        /* add the current character to the buffer */
        rii_check(cork_buffer_append
                  (&sp->scratch, curr, 1, err));
        s0_advance_char(i, curr, &sp->pos);
    }

    /* If we fall through the loop, then we've run out of characters
     * without seeing the end of the string. */
    cork_error_set
        (err, S0_ERROR, S0_SYNTAX_ERROR,
         "Unterminated string literal at %zu.%zu",
         start.line, start.column);
    return -1;
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
              (&sp->scratch, sp->slice->buf, i, err));

    /* See if this represents a valid operand name */
    size_t  j;
    for (j = 0; j < S0_OPCODE_COUNT; j++) {
        if (strcasecmp(sp->scratch.buf, OPERAND_NAMES[j]) == 0) {
            *dest = j;
            DEBUG("[%4zu:%2zu]   %s",
                  start.line, start.column, OPERAND_NAMES[j]);
            return cork_slice_slice_offset(sp->slice, i, err);
        }
    }

    cork_error_set
        (err, S0_ERROR, S0_SYNTAX_ERROR,
         "Unknown S0 operand at %zu.%zu: %.*s",
         start.line, start.column, (int) i, (char *) sp->slice->buf);
    return -1;
}


/* Parse an integer */
static int
s0_parse_int(struct swan *s, struct s0_parser *sp,
             size_t *dest, struct cork_error *err)
{
    DEBUG("[%4zu:%2zu] Trying to parse integer",
          sp->pos.line, sp->pos.column);
    size_t  i = 0;
    const char  *curr = sp->slice->buf;
    struct s0_position  start = sp->pos;

    /* We DON'T skip over whitespace, since we're going to parse an
     * integer immediately after an identifier tag. */

    while (i < sp->slice->size && isdigit(*curr)) {
        s0_advance_char(i, curr, &sp->pos);
    }

    /* i is now points just past the end of the identifier */

    /* Put the integer text into the scratch buffer so that it's NUL
     * terminated */
    rii_check(cork_buffer_set
              (&sp->scratch, sp->slice->buf, i, err));

    /* Parse the integer value, onlying allowing decimal */
    char  *endptr;
    long  l_value = strtol(sp->scratch.buf, &endptr, 10);
    if (*endptr != '\0' || errno == ERANGE ||
        l_value < 0 || l_value > SIZE_MAX) {
        cork_error_set
            (err, S0_ERROR, S0_SYNTAX_ERROR,
             "Expected integer at %zu.%zu",
             start.line, start.column);
        return -1;
    }

    *dest = l_value;
    return cork_slice_slice_offset(sp->slice, i, err);
}


/* Parse an integer for an identifier */
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
              (&sp->scratch, sp->slice->buf+1, i-1, err));

    /* Parse the integer value, onlying allowing decimal */
    char  *endptr;
    long  l_value = strtol(sp->scratch.buf, &endptr, 10);
    if (*endptr != '\0' || errno == ERANGE ||
        l_value < 0 || l_value > UINTPTR_MAX) {
        if (tag == '\0') {
            cork_error_set
                (err, S0_ERROR, S0_SYNTAX_ERROR,
                 "Expected %c identifier at %zu.%zu",
                 tag, start.line, start.column);
        } else {
            cork_error_set
                (err, S0_ERROR, S0_SYNTAX_ERROR,
                 "Expected identifier at %zu.%zu",
                 start.line, start.column);
        }
        return -1;
    }

    *dest = l_value;
    return cork_slice_slice_offset(sp->slice, i, err);
}

/* Parse an identifier with the given tag. */
static int
s0_parse_id(struct swan *s, struct s0_parser *sp, enum s0_id_tag tag,
            s0_id *dest, struct cork_error *err)
{
    char  tag_ch = s0_id_tag_name(tag);
    rii_check(s0_parse_skip_leading(s, sp, err));
    DEBUG("[%4zu:%2zu] Trying to parse %c identifier",
          sp->pos.line, sp->pos.column, (int) tag_ch);

    if ((sp->slice->size == 0) || (*((char *) sp->slice->buf) != tag_ch)) {
        if (tag == '\0') {
            cork_error_set
                (err, S0_ERROR, S0_SYNTAX_ERROR,
                 "Expected %c identifier at %zu.%zu",
                 tag, sp->pos.line, sp->pos.column);
        } else {
            cork_error_set
                (err, S0_ERROR, S0_SYNTAX_ERROR,
                 "Expected identifier at %zu.%zu",
                 sp->pos.line, sp->pos.column);
        }
        return -1;
    }

    return s0_parse_id_int(s, sp, tag_ch, dest, err);
}

/* Parse any identifier. */
static int
s0_parse_any_id(struct swan *s, struct s0_parser *sp,
                s0_tagged_id *dest, struct cork_error *err)
{
    rii_check(s0_parse_skip_leading(s, sp, err));
    DEBUG("[%4zu:%2zu] Trying to parse identifier",
          sp->pos.line, sp->pos.column);

    if (sp->slice->size == 0) {
        cork_error_set
            (err, S0_ERROR, S0_SYNTAX_ERROR,
             "Expected identifier at %zu.%zu",
             sp->pos.line, sp->pos.column);
        return -1;
    }

    int  tag;
    switch (*((char *) sp->slice->buf)) {
        case '%':
            tag = S0_ID_TAG_TYPE;
            break;

        case '$':
            tag = S0_ID_TAG_LOCAL;
            break;

        case '^':
            tag = S0_ID_TAG_UPVALUE;
            break;

        case '!':
            tag = S0_ID_TAG_PARAM;
            break;

        default:
            cork_error_set
                (err, S0_ERROR, S0_SYNTAX_ERROR,
                 "Expected identifier at %zu.%zu",
                 sp->pos.line, sp->pos.column);
            return -1;
    }

    s0_id  id;
    rii_check(s0_parse_id_int(s, sp, '\0', &id, err));
    *dest = s0_tagged_id(tag, id);
    return 0;
}

/* Parse a list of identifiers. */
static int
s0_parse_any_id_list(struct swan *s, struct s0_parser *sp,
                     s0_tagged_id_array *dest, struct cork_error *err)
{
    rii_check(s0_parse_require_token(s, sp, "(", 1, err));
    bool  first = true;

    do {
        int  rc;

        /* At this point, we can try to read a close-paren to end the
         * list. */
        rc = s0_parse_try_token(s, sp, ")", 1, err);
        if (rc != -2) {
            return rc;
        }

        /* If this isn't the first element of the list, there needs to
         * be a comma next. */
        if (first) {
            first = false;
        } else {
            rii_check(s0_parse_require_token(s, sp, ",", 1, err));
        }

        /* And then we have to parse an identifier. */
        s0_tagged_id  id;
        rii_check(s0_parse_any_id(s, sp, &id, err));
        rii_check(cork_array_append(dest, id, err));
    } while (true);

    /* Should be unreachable */
    cork_unknown_error_set(err);
    return -1;
}


/* Parse a list of interface entries. */
static int
s0_parse_interface_entries(struct swan *s, struct s0_parser *sp,
                           struct s0_instruction *dest, struct cork_error *err)
{
    rii_check(s0_parse_require_token(s, sp, "{", 1, err));
    bool  first = true;

    do {
        int  rc;

        /* At this point, we can try to read a close-brace to end the
         * list. */
        rc = s0_parse_try_token(s, sp, "}", 1, err);
        if (rc != -2) {
            return rc;
        }

        /* If this isn't the first element of the list, there needs to
         * be a comma next. */
        if (first) {
            first = false;
        } else {
            rii_check(s0_parse_require_token(s, sp, ",", 1, err));
        }

        /* And then we have to parse an individual entry. */
        s0_tagged_id  id;
        const char  *key;
        rii_check(s0_parse_string(s, sp, err));
        ri_check_alloc(key = cork_strdup(sp->scratch.buf),
                       "TINTERFACE entry key");

        rii_check(s0_parse_require_token(s, sp, ":", 1, err));
        rii_check(s0_parse_any_id(s, sp, &id, err));
        rii_check(s0i_tinterface_add_entry(s, dest, key, id, err));
    } while (true);

    /* Should be unreachable */
    cork_unknown_error_set(err);
    return -1;
}


/* Parse a list of class entries. */
static int
s0_parse_class_entries(struct swan *s, struct s0_parser *sp,
                       struct s0_instruction *dest, struct cork_error *err)
{
    rii_check(s0_parse_require_token(s, sp, "{", 1, err));
    bool  first = true;

    do {
        int  rc;

        /* At this point, we can try to read a close-brace to end the
         * list. */
        rc = s0_parse_try_token(s, sp, "}", 1, err);
        if (rc != -2) {
            return rc;
        }

        /* If this isn't the first element of the list, there needs to
         * be a comma next. */
        if (first) {
            first = false;
        } else {
            rii_check(s0_parse_require_token(s, sp, ",", 1, err));
        }

        /* And then we have to parse an individual entry. */
        s0_tagged_id  id;
        const char  *key;
        rii_check(s0_parse_string(s, sp, err));
        ri_check_alloc(key = cork_strdup(sp->scratch.buf),
                       "TCLASS entry key");

        rii_check(s0_parse_require_token(s, sp, ":", 1, err));
        rii_check(s0_parse_any_id(s, sp, &id, err));
        rii_check(s0i_tclass_add_entry(s, dest, key, id, err));
    } while (true);

    /* Should be unreachable */
    cork_unknown_error_set(err);
    return -1;
}

/* forward declaration */
static int
s0_parse_instruction(struct swan *s, struct s0_parser *sp,
                     struct cork_error *err);


/* A bunch of parsers for each opcode.  Each of these assume that the
 * operand name has already been parsed, since we'll need to switch on
 * that before these functions are ever called. */

static int
s0_parse_TRECURSIVE(struct swan *s, struct s0_parser *sp,
                    struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_trecursive_new(s, dest, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_TTYPE(struct swan *s, struct s0_parser *sp,
               struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_ttype_new(s, dest, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_TLITERAL(struct swan *s, struct s0_parser *sp,
                  struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_tliteral_new(s, dest, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_TANY(struct swan *s, struct s0_parser *sp,
              struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_tany_new(s, dest, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_TPRODUCT(struct swan *s, struct s0_parser *sp,
                  struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rip_check(instr = s0i_tproduct_new(s, dest, err));
    ei_check(s0_parse_any_id_list(s, sp, &instr->_.tproduct.elements, err));
    ei_check(s0_parse_require_token(s, sp, ";", 1, err));
    ei_check(cork_array_append(sp->body, instr, err));
    return 0;

error:
    cork_gc_decref(swan_gc(s), instr);
    return -1;
}

static int
s0_parse_TFUNCTION(struct swan *s, struct s0_parser *sp,
                   struct cork_error *err)
{
    s0_id  dest;
    s0_tagged_id  input;
    s0_tagged_id  output;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rii_check(s0_parse_any_id(s, sp, &input, err));
    rii_check(s0_parse_require_token(s, sp, "->", 2, err));
    rii_check(s0_parse_any_id(s, sp, &output, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_tfunction_new(s, dest, input, output, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_TLOCATION(struct swan *s, struct s0_parser *sp,
                   struct cork_error *err)
{
    s0_id  dest;
    s0_tagged_id  referent;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rii_check(s0_parse_any_id(s, sp, &referent, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_tlocation_new(s, dest, referent, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_TINTERFACE(struct swan *s, struct s0_parser *sp,
                    struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rip_check(instr = s0i_tinterface_new(s, dest, err));
    ei_check(s0_parse_interface_entries(s, sp, instr, err));
    ei_check(s0_parse_require_token(s, sp, ";", 1, err));
    ei_check(cork_array_append(sp->body, instr, err));
    return 0;

error:
    cork_gc_decref(swan_gc(s), instr);
    return -1;
}

static int
s0_parse_TCLASS(struct swan *s, struct s0_parser *sp,
                struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rii_check(s0_parse_string(s, sp, err));
    rip_check(instr = s0i_tclass_new(s, dest, sp->scratch.buf, err));
    ei_check(s0_parse_class_entries(s, sp, instr, err));
    ei_check(s0_parse_require_token(s, sp, ";", 1, err));
    ei_check(cork_array_append(sp->body, instr, err));
    return 0;

error:
    cork_gc_decref(swan_gc(s), instr);
    return -1;
}

static int
s0_parse_TBLOCK(struct swan *s, struct s0_parser *sp,
                struct cork_error *err)
{
    s0_id  dest;
    s0_tagged_id  result;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_TYPE, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rii_check(s0_parse_any_id(s, sp, &result, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_tblock_new(s, dest, result, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_LITERAL(struct swan *s, struct s0_parser *sp,
                 struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_LOCAL, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rii_check(s0_parse_string(s, sp, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_literal_new(s, dest, sp->scratch.buf, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_TUPLE(struct swan *s, struct s0_parser *sp,
               struct cork_error *err)
{
    s0_id  dest;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_LOCAL, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rip_check(instr = s0i_tuple_new(s, dest, err));
    ei_check(s0_parse_any_id_list(s, sp, &instr->_.tuple.elements, err));
    ei_check(s0_parse_require_token(s, sp, ";", 1, err));
    ei_check(cork_array_append(sp->body, instr, err));
    return 0;

error:
    cork_gc_decref(swan_gc(s), instr);
    return -1;
}

static int
s0_parse_GETTUPLE(struct swan *s, struct s0_parser *sp,
                  struct cork_error *err)
{
    s0_id  dest;
    s0_tagged_id  src;
    size_t  index;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_LOCAL, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rii_check(s0_parse_any_id(s, sp, &src, err));
    rii_check(s0_parse_require_token(s, sp, ".", 1, err));
    rii_check(s0_parse_int(s, sp, &index, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_gettuple_new(s, dest, src, index, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_GETCLASS(struct swan *s, struct s0_parser *sp,
                  struct cork_error *err)
{
    s0_id  dest;
    s0_tagged_id  src;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_LOCAL, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rii_check(s0_parse_any_id(s, sp, &src, err));
    rii_check(s0_parse_string(s, sp, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_getclass_new(s, dest, src, sp->scratch.buf, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_MACRO(struct swan *s, struct s0_parser *sp,
               struct cork_error *err)
{
    int  rc;
    s0_id  dest;
    struct s0_instruction  *instr;
    s0_instruction_array  *saved_body;

    rii_check(s0_parse_id(s, sp, S0_ID_TAG_LOCAL, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rii_check(s0_parse_string(s, sp, err));
    rip_check(instr = s0i_macro_new(s, dest, sp->scratch.buf, err));

    /* First an optional upvalue */
    rc = s0_parse_try_token(s, sp, "upvalue", 7, err);
    if (rc == -1) {
        goto error;
    } else if (rc == 0) {
        ei_check(s0_parse_require_token(s, sp, "(", 1, err));
        ei_check(s0_parse_any_id(s, sp, &instr->_.macro.upvalue, err));
        ei_check(s0_parse_require_token(s, sp, ")", 1, err));
    }

    /* Then required input and output types */
    ei_check(s0_parse_any_id(s, sp, &instr->_.macro.input, err));
    ei_check(s0_parse_require_token(s, sp, "->", 2, err));
    ei_check(s0_parse_any_id(s, sp, &instr->_.macro.output, err));

    /* Then a required list of instructions */
    saved_body = sp->body;
    sp->body = &instr->_.macro.body;
    ei_check(s0_parse_require_token(s, sp, "{", 1, err));
    ei_check(s0_parse_skip_leading(s, sp, err));
    while (sp->slice->size > 0) {
        /* A closing brace ends the macro body. */
        rc = s0_parse_try_token(s, sp, "}", 1, err);
        if (rc == -1) {
            goto error;
        } else if (rc == 0) {
            break;
        }

        /* No brace, so there must be an instruction next. */
        ei_check(s0_parse_instruction(s, sp, err));
        ei_check(s0_parse_skip_leading(s, sp, err));
    }

    sp->body = saved_body;
    ei_check(s0_parse_require_token(s, sp, ";", 1, err));
    ei_check(cork_array_append(sp->body, instr, err));
    return 0;

error:
    cork_gc_decref(swan_gc(s), instr);
    return -1;
}

static int
s0_parse_CALL(struct swan *s, struct s0_parser *sp,
              struct cork_error *err)
{
    s0_id  dest;
    s0_tagged_id  callee;
    s0_tagged_id  input;
    struct s0_instruction  *instr;
    rii_check(s0_parse_id(s, sp, S0_ID_TAG_LOCAL, &dest, err));
    rii_check(s0_parse_require_token(s, sp, "=", 1, err));
    rii_check(s0_parse_any_id(s, sp, &callee, err));
    rii_check(s0_parse_require_token(s, sp, "(", 1, err));
    rii_check(s0_parse_any_id(s, sp, &input, err));
    rii_check(s0_parse_require_token(s, sp, ")", 1, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_call_new(s, dest, callee, input, err));
    rii_check(cork_array_append(sp->body, instr, err));
    return 0;
}

static int
s0_parse_RETURN(struct swan *s, struct s0_parser *sp,
                struct cork_error *err)
{
    s0_tagged_id  result;
    struct s0_instruction  *instr;
    rii_check(s0_parse_any_id(s, sp, &result, err));
    rii_check(s0_parse_require_token(s, sp, ";", 1, err));
    rip_check(instr = s0i_return_new(s, result, err));
    rii_check(cork_array_append(sp->body, instr, err));
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
#define PARSE_OPCODE(name) \
        case S0_##name: \
            return s0_parse_##name(s, sp, err);
        S0_OPCODES(PARSE_OPCODE)
#undef PARSE_OPCODE

        default:
            cork_unknown_error_set(err);
            return -1;
    }
}


struct s0_basic_block *
s0_parse(struct swan *s, struct cork_slice *src, struct cork_error *err)
{
    struct s0_basic_block  *result;
    rpp_check(result = s0_basic_block_new
              (s, "<top-level>", NULL, NULL, NULL, err));

    struct s0_parser  sp = { src, {0,0}, CORK_BUFFER_INIT(), &result->body };

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
