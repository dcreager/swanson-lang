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

#include "swanson/ast.h"
#include "swanson/ds.h"
#include "swanson/metamodel.h"
#include "swanson/state.h"

static const char  digit_values[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1,
    -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1
};

%%{
machine s0;

# mark_end actually marks the character *after* the thing we just
# matched, but that's alright, since it makes the length calculation
# easier.
action mark_start { xs = fpc; }
action mark_end   { xe = fpc; }

nl = '\n' @{curline += 1;};
any_count_line = any | nl;
comment = '#' [^\n]* nl;
ws = (comment | nl | (space-'\n'))*;

bare_identifier = ([a-zA-Z_][a-zA-Z_0-9]*) >mark_start
                  %{ cork_buffer_set(&scratch, xs, (fpc-xs)); };

ce_bs = '\\\\' %{ cork_buffer_append(&scratch, "\\", 1); };
ce_dq = '\\\"' %{ cork_buffer_append(&scratch, "\"", 1); };
ce_sq = '\\\'' %{ cork_buffer_append(&scratch, "'",  1); };
ce_a  = '\\a'  %{ cork_buffer_append(&scratch, "\a",  1); };
ce_b  = '\\b'  %{ cork_buffer_append(&scratch, "\b",  1); };
ce_f  = '\\f'  %{ cork_buffer_append(&scratch, "\f",  1); };
ce_n  = '\\n'  %{ cork_buffer_append(&scratch, "\n",  1); };
ce_r  = '\\r'  %{ cork_buffer_append(&scratch, "\r",  1); };
ce_t  = '\\t'  %{ cork_buffer_append(&scratch, "\t",  1); };
ce_v  = '\\v'  %{ cork_buffer_append(&scratch, "\v",  1); };

hex_digit = xdigit >{ hex_char |= digit_values[(int) fc]; };
ce_hex = '\\x'     @{ hex_char = 0; }
         hex_digit @{ hex_char <<= 4; }
         hex_digit %{ cork_buffer_append(&scratch, &hex_char, 1); };

c_escape = ce_bs | ce_dq | ce_sq |
           ce_a | ce_b | ce_f | ce_n | ce_r | ce_t | ce_v |
           ce_hex;

non_escaped = [^\\"]+ >mark_start
              %{ cork_buffer_append(&scratch, xs, (fpc-xs)); };

string = ('"' %{ cork_buffer_clear(&scratch); })
         (c_escape | non_escaped)*
         :>> '"';

identifier = ws (bare_identifier | string)
             %{ id = swan_static_string_new(s, scratch.buf); };

id_list = null %{ id_list = NULL; }
        | identifier
          %{
              id_list = swan_sllist_new(s, &id->parent, NULL);
              id_tail = id_list;
          }
          (ws "," identifier
          %{
              list_build = swan_sllist_new(s, &id->parent, NULL);
              id_tail->tail = list_build;
              id_tail = list_build;
          })*;

nonvoid_call = id_list %{ result_list = id_list; }
               ws "=" identifier %{ thing = id; }
               ws "." identifier %{ method = id; }
               ws "(" id_list ")" %{ param_list = id_list; }
               ws ";"
               %{ call = swan_ast_call_new
                      (s, result_list, thing, method, param_list); };

void_call = identifier %{ result_list = NULL; thing = id; }
            ws "." identifier %{ method = id; }
            ws "(" id_list ")" %{ param_list = id_list; }
            ws ";"
            %{ call = swan_ast_call_new
                   (s, result_list, thing, method, param_list); };

call = void_call | nonvoid_call;

call_list = (call
            %{
                list_build = swan_sllist_new(s, &call->parent, ast_list);
                if (ast_tail == NULL) {
                    ast_list = list_build;
                } else {
                    ast_tail->tail = list_build;
                }
                ast_tail = list_build;
            })+
            ws;

main := call_list >{ ast_tail = NULL; };
}%%

%% write data;

struct swan_sllist *
swan_ast_parse(struct swan *s, const char *buf, size_t size)
{
    const char  *p = buf;
    const char  *pe = buf + size;
    const char  *eof = pe;
    CORK_ATTR_UNUSED const char  *ts;
    CORK_ATTR_UNUSED const char  *te;
    CORK_ATTR_UNUSED int  cs;
    CORK_ATTR_UNUSED int  act;

    size_t  curline = 0;
    const char  *xs;
    struct cork_buffer  scratch = CORK_BUFFER_INIT();
    char  hex_char;

    struct swan_static_string  *thing;
    struct swan_static_string  *method;
    struct swan_static_string  *id;
    struct swan_sllist  *list_build;
    struct swan_sllist  *id_list;
    struct swan_sllist  *id_tail;
    struct swan_sllist  *param_list;
    struct swan_sllist  *result_list;
    struct swan_ast_call  *call = NULL;
    struct swan_sllist  *ast_list;
    struct swan_sllist  *ast_tail;

    %% write init;
    %% write exec;

    if (cs < %%{ write first_final; }%%) {
        printf("error at line %zu\n", curline);
        cork_error_set
            (SWAN_METAMODEL_ERROR, SWAN_METAMODEL_PARSE_ERROR,
             "Parse error");
        return 0;
    }

    cork_buffer_done(&scratch);
    return ast_list;
}
