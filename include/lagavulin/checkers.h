/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2011, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the LICENSE.txt file in this distribution for license
 * details.
 * ----------------------------------------------------------------------
 */

#ifndef LAGAVULIN_CHECKERS_H
#define LAGAVULIN_CHECKERS_H

/**
 * @file
 * @brief Implementation of the @ref checkers module
 */


/**
 * @addtogroup checkers Checker macros
 *
 * <tt>#%include \<lagavulin/checkers.h\></tt>
 *
 * This module defines several helper macros for checking the results of
 * functions, and passing on error conditions.
 *
 * @{
 */

#define e_check_(call, file, line) \
    do { \
        int  __rc = call; \
        if (__rc != 0) { \
            goto error; \
        } \
    } while (0)

#define e_check(call)  e_check_(call, __FILE__, __LINE__)

#define r_check_(call, file, line) \
    do { \
        int  __rc = call; \
        if (__rc != 0) { \
            return __rc; \
        } \
    } while (0)

#define r_check(call)  r_check_(call, __FILE__, __LINE__)

/* end of checkers group */
/**
 * @}
 */

#endif  /* LAGAVULIN_CHECKERS_H */
