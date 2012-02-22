/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_DS_H
#define SWANSON_DS_H


#include <swanson/metamodel.h>
#include <swanson/state.h>


/*-----------------------------------------------------------------------
 * Singly-linked list
 */

#define swan_sllist___breed_id  0x03dbbf0a
#define swan_sllist___breed_name  "swan_sllist"

struct swan_sllist {
    struct swan_thing  parent;
    struct swan_thing  *head;
    struct swan_sllist  *tail;
};

#define swan_thing_to_swan_sllist(t) \
    (cork_container_of((t), struct swan_sllist, parent))

/* Steals references to head and tail */
struct swan_sllist *
swan_sllist_new(struct swan *s, struct swan_thing *head,
                struct swan_sllist *tail);


#endif  /* SWANSON_DS_H */
