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

#include <swanson/metamodel.h>
#include <swanson/s0.h>


struct swan_old_prelude {
    struct s0_type  *prelude;

    /* bool */
    struct s0_type  *t_bool;
    struct s0_value  *v_false;
    struct s0_value  *v_true;
};


struct swan_object *
swan_prelude_new(struct swan *s);


/*-----------------------------------------------------------------------
 * Default class and metaclass
 */

#define swan_static_metaclass___class_id  0x706fd91e
#define swan_static_metaclass___class_name  "swan_static_metaclass"
#define swan_gc_metaclass___class_id  0x52740682
#define swan_gc_metaclass___class_name  "swan_gc_metaclass"

#define swan_default_class___class_id  0xbabb8e19
#define swan_default_class___class_name  "swan_default_class"
#define swan_default_method___class_id  0x149cc9ac
#define swan_default_method___class_name  "swan_default_method"

struct swan_default_class {
    struct swan_class  parent;
    struct cork_hash_table  methods;
};

#define swan_object_to_swan_default_class(obj) \
    (cork_container_of((obj), struct swan_default_class, parent.parent))

struct swan_default_method {
    struct swan_method  parent;
    struct swan_default_class  *c;
};

extern struct swan_class  swan_static_metaclass;
extern struct swan_class  swan_gc_metaclass;

struct swan_class *
swan_default_class_new(struct swan *s, const char *name);

int
swan_default_class_add_method(struct swan *s, struct swan_class *c,
                              const char *name, swan_method_evaluate evaluate);


/*-----------------------------------------------------------------------
 * Boolean
 */

#define swan_bool___class_id  0x9043bf18
#define swan_bool___class_name  "swan_bool"

struct swan_bool {
    struct swan_object  parent;
    bool  value;
};

#define swan_object_to_swan_bool(obj) \
    (cork_container_of((obj), struct swan_bool, parent))

struct swan_object *
swan_bool_false(void);

struct swan_object *
swan_bool_true(void);

#define swan_bool_new(val) \
    ((val)? swan_bool_true(): swan_bool_false())


/*-----------------------------------------------------------------------
 * swan_static_string
 */

#define swan_static_string___class_id  0xb5eaf926
#define swan_static_string___class_name  "swan_static_string"

struct swan_static_string {
    struct swan_object  parent;
    const char  *contents;
};

#define swan_object_to_swan_static_string(obj) \
    (cork_container_of((obj), struct swan_static_string, parent))

struct swan_static_string *
swan_static_string_new(struct swan *s, const char *contents);


#endif  /* SWANSON_PRELUDE_H */
