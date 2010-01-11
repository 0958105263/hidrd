/** @file
 * @brief HID report descriptor - option type
 *
 * Copyright (C) 2010 Nikolai Kondrashov
 *
 * This file is part of hidrd.
 *
 * Hidrd is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Hidrd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hidrd; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @author Nikolai Kondrashov <spbnick@gmail.com>
 *
 * @(#) $Id$
 */

#ifndef __HIDRD_STRM_OPT_TYPE_H__
#define __HIDRD_STRM_OPT_TYPE_H__

#include <stdbool.h>
#include "hidrd/strm/opt_value.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Option value type */
typedef enum hidrd_strm_opt_type {
    HIDRD_STRM_OPT_TYPE_STRING   = 's',  /**< String  */
    HIDRD_STRM_OPT_TYPE_BOOLEAN  = 'b',  /**< Boolean */
} hidrd_strm_opt_type;

/**
 * Check if an option type is valid.
 *
 * @param type  Option type to check.
 *
 * @return True if the type is valid, false otherwise.
 */
static inline bool
hidrd_strm_opt_type_valid(hidrd_strm_opt_type type)
{
    switch (type)
    {
        case HIDRD_STRM_OPT_TYPE_STRING:
        case HIDRD_STRM_OPT_TYPE_BOOLEAN:
            return true;
        default:
            return false;
    }
}

/**
 * Parse a string as a value of specified type.
 *
 * @param type  Value type to parse accordingly.
 * @param pval  Location for resulting value.
 * @param str   String to parse as a value of @e type type.
 *
 * @return True if parsed successfully, false otherwise.
 */
extern bool
hidrd_strm_opt_type_parse_value(hidrd_strm_opt_type     type,
                                hidrd_strm_opt_value   *pval,
                                const char             *str);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HIDRD_STRM_OPT_TYPE_H__ */


