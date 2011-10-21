/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#include "swanson/state.h"
#include "swanson/swanson0.h"

int
swan_expression_init(struct swan *s, struct swan_expression *expr,
                     struct cork_error *err)
{
    expr->parent.cls = SWAN_EXPRESSION_CLASS;
    return 0;
}
