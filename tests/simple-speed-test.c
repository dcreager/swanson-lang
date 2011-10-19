#include <stdlib.h>
#include <stdio.h>

#include "lagavulin/block.h"
#include "lagavulin/stack.h"


int main(int argc, char **argv)
{
    struct cork_alloc  *alloc = cork_allocator_new_debug();
    struct cork_gc  gc;
    cork_gc_init(&gc, alloc);

    struct lgv_block  *binit = lgv_block_new_constant_int(&gc, 0);

    struct lgv_block  *bcond_0 = lgv_block_new_dup(&gc);
    struct lgv_block  *bcond_1 = lgv_block_new_constant_int(&gc, 500000000);
    struct lgv_block  *bcond_2 = lgv_block_new_lt_int(&gc);
    struct lgv_block  *bcond_s0 = lgv_block_new_seq(&gc, bcond_0, bcond_1);
    struct lgv_block  *bcond_s1 = lgv_block_new_seq(&gc, bcond_s0, bcond_2);
    struct lgv_block  *bcond = bcond_s1;

    struct lgv_block  *bbody_0 = lgv_block_new_constant_int(&gc, 1);
    struct lgv_block  *bbody_1 = lgv_block_new_add_int(&gc);
    struct lgv_block  *bbody_s0 = lgv_block_new_seq(&gc, bbody_0, bbody_1);
    struct lgv_block  *bbody = bbody_s0;

    struct lgv_block  *bwhile = lgv_block_new_while(&gc, bcond, bbody);
    struct lgv_block  *b0 = lgv_block_new_seq(&gc, binit, bwhile);

    struct lgv_block  *b_ret = lgv_block_new_return(&gc);
    lgv_block_set_next(&gc, b0, b_ret);
    struct lgv_state  state;
    lgv_state_init(&gc, &state);
    state.ret = lgv_block_new_halt(&gc);
    lgv_block_execute(&gc, lgv_block_get_head(b0), &state, state.stack.top);
    int  actual = lgv_stack_get(&state.stack, state.stack.top, -1, si);
    printf("Got: %d\n", actual);
    lgv_state_done(&gc, &state);

    cork_gc_decref(&gc, b0);
    cork_gc_done(&gc);
    cork_allocator_free(alloc);

    return 0;
}
