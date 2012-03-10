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

#include "swanson/helpers.h"
#include "swanson/metamodel.h"
#include "swanson/state.h"

#include "prelude/metamodel.c.in"
#include "prelude/bool.c.in"
#include "prelude/string.c.in"

/* booleans */
_eval_obj_(swan_prelude__false, swan_bool__false);
_eval_obj_(swan_prelude__true, swan_bool__true);

/* static strings */
_eval_obj_(swan_prelude__string, swan_static_string__breed);

struct swan_thing *
swan_prelude_new(struct swan *s)
{
    struct swan_breed  *prelude_breed;
    struct swan_thing  *prelude;

#define _add_method_(name) \
    swan_default_breed_add_method \
        (s, prelude_breed, #name, &swan_prelude__##name##__eval);

    prelude_breed = swan_default_breed_new(s, "prelude");
    /* booleans */
    _add_method_(false);
    _add_method_(true);
    /* static strings */
    _add_method_(string);

    prelude = swan_thing_new_basic(s, prelude_breed);
    return prelude;
}
