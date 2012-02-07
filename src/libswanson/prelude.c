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

_eval_obj_(swan_prelude__string, swan_static_string__class);

struct swan_object *
swan_prelude_new(struct swan *s)
{
    struct swan_class  *prelude_class;
    struct swan_object  *prelude;

#define _add_method_(name) \
    swan_default_class_add_method \
        (s, prelude_class, #name, &swan_prelude__##name##__eval);

    prelude_class = swan_default_class_new(s, "prelude");
    _add_method_(string);

    prelude = swan_object_new_basic(s, prelude_class);
    return prelude;
}
