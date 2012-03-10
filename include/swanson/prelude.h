/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011-2012, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef SWANSON_PRELUDE_H
#define SWANSON_PRELUDE_H

#include <libcork/core.h>

#include <swanson/metamodel.h>


struct swan_thing *
swan_prelude_new(struct swan *s);


/*-----------------------------------------------------------------------
 * Default breed and metabreed
 */

#define swan_static_metabreed___breed_id  0x706fd91e
#define swan_static_metabreed___breed_name  "swan_static_metabreed"
#define swan_gc_metabreed___breed_id  0x52740682
#define swan_gc_metabreed___breed_name  "swan_gc_metabreed"

#define swan_default_breed___breed_id  0xbabb8e19
#define swan_default_breed___breed_name  "swan_default_breed"
#define swan_default_method___breed_id  0x149cc9ac
#define swan_default_method___breed_name  "swan_default_method"

struct swan_default_breed {
    struct swan_breed  parent;
    struct cork_hash_table  methods;
};

#define swan_thing_to_swan_default_breed(t) \
    (cork_container_of((t), struct swan_default_breed, parent.parent))

struct swan_default_method {
    struct swan_method  parent;
    struct swan_default_breed  *b;
};

extern struct swan_breed  swan_static_metabreed;
extern struct swan_breed  swan_gc_metabreed;

struct swan_breed *
swan_default_breed_new(struct swan *s, const char *name);

int
swan_default_breed_add_method(struct swan *s, struct swan_breed *b,
                              const char *name, swan_method_evaluate evaluate);


/*-----------------------------------------------------------------------
 * Boolean
 */

#define swan_bool___breed_id  0x9043bf18
#define swan_bool___breed_name  "swan_bool"

struct swan_bool {
    struct swan_thing  parent;
    bool  value;
};

#define swan_thing_to_swan_bool(t) \
    (cork_container_of((t), struct swan_bool, parent))

struct swan_thing *
swan_bool_false(void);

struct swan_thing *
swan_bool_true(void);

#define swan_bool_new(val) \
    ((val)? swan_bool_true(): swan_bool_false())


/*-----------------------------------------------------------------------
 * swan_static_string
 */

#define swan_static_string___breed_id  0xb5eaf926
#define swan_static_string___breed_name  "swan_static_string"

struct swan_static_string {
    struct swan_thing  parent;
    const char  *contents;
};

#define swan_thing_to_swan_static_string(t) \
    (cork_container_of((t), struct swan_static_string, parent))

struct swan_static_string *
swan_static_string_new(struct swan *s, const char *contents);


#endif  /* SWANSON_PRELUDE_H */
