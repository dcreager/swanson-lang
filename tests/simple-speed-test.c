/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>

#include "swanson/state.h"
#include "lagavulin/block.h"
#include "lagavulin/engine.h"
#include "lagavulin/stack.h"

int
main(int argc, char **argv)
{
    struct cork_error  err = CORK_ERROR_INIT();
    struct swan  *s = lgv_new(&err);

    struct lgv_block  *binit = lgv_block_new_constant_int(s, 0);

    struct lgv_block  *bcond_0 = lgv_block_new_dup(s);
    struct lgv_block  *bcond_1 = lgv_block_new_constant_int(s, 500000000);
    struct lgv_block  *bcond_2 = lgv_block_new_lt_int(s);
    struct lgv_block  *bcond_s0 = lgv_block_new_seq(s, bcond_0, bcond_1);
    struct lgv_block  *bcond_s1 = lgv_block_new_seq(s, bcond_s0, bcond_2);
    struct lgv_block  *bcond = bcond_s1;

    struct lgv_block  *bbody_0 = lgv_block_new_constant_int(s, 1);
    struct lgv_block  *bbody_1 = lgv_block_new_add_int(s);
    struct lgv_block  *bbody_s0 = lgv_block_new_seq(s, bbody_0, bbody_1);
    struct lgv_block  *bbody = bbody_s0;

    struct lgv_block  *bwhile = lgv_block_new_while(s, bcond, bbody);
    struct lgv_block  *b0 = lgv_block_new_seq(s, binit, bwhile);

    struct lgv_block  *b_ret = lgv_block_new_return(s);
    lgv_block_set_next(s, b0, b_ret);
    struct lgv_state  state;
    lgv_state_init(s, &state);
    state.ret = lgv_block_new_halt(s);
    lgv_block_execute(s, lgv_block_get_head(s, b0), &state, state.stack.top);
    int  actual = lgv_stack_get(&state.stack, state.stack.top, -1, si);
    printf("Got: %d\n", actual);
    lgv_state_done(s, &state);

    cork_gc_decref(swan_gc(s), b0);
    lgv_free(s);
    cork_error_done(&err);

    return 0;
}
