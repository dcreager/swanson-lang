/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_PRELUDE_H
#define SWANSON_PRELUDE_H

#include <libcork/core.h>

#include <swanson/s0.h>


struct swan_prelude {
    struct s0_type  *prelude;

    /* bool */
    struct s0_type  *t_bool;
    struct s0_value  *v_false;
    struct s0_value  *v_true;
};


#endif  /* SWANSON_PRELUDE_H */
