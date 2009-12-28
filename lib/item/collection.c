/** @file
 * @brief HID report descriptor - collection item
 *
 * Copyright (C) 2009 Nikolai Kondrashov
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

#include <string.h>
#include <stdio.h>
#include "hidrd/item/collection.h"

#ifdef HIDRD_WITH_TOKENS
char *
hidrd_item_collection_type_to_token(hidrd_item_collection_type type)
{
    assert(hidrd_item_collection_type_valid(type));

    switch (type)
    {
#define MAP(_NAME, _name) \
    case HIDRD_ITEM_COLLECTION_TYPE_##_NAME:   \
        return strdup(#_name)

        MAP(PHYSICAL, physical);
        MAP(APPLICATION, application);
        MAP(LOGICAL, logical);
        MAP(REPORT, report);
        MAP(NAMED_ARRAY, named_array);
        MAP(USAGE_SWITCH, usage_switch);
        MAP(USAGE_MODIFIER, usage_modifier);

#undef MAP

    default:
        {
            char   *token;

            if (!hidrd_item_collection_type_valid(type))
                return NULL;

            if (asprintf(&token, "%u", type) < 0)
                return NULL;

            return token;
        }
    }
}
#endif /* HIDRD_WITH_TOKENS */
