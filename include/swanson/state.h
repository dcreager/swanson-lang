/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_STATE_H
#define SWANSON_STATE_H


#include <libcork/core.h>


struct swan_engine;
struct swan_old_prelude;

struct swan {
    struct swan_engine  *engine;
    struct swan_old_prelude  *prelude;
};

int
swan_init(struct swan *state);

void
swan_done(struct swan *state);


/* Hash of "swanson.general" */
#define SWAN_GENERAL_ERROR  0xae419dc9

enum swan_general_error {
    SWAN_GENERAL_BAD_TYPE
};


#endif  /* SWANSON_STATE_H */
