/** @file
 * @brief HID report descriptor - XML source - element handling
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

#include "src_element.h"


/**
 * Prototype for an element processing function.
 *
 * @param xml_src   XML source instance.
 * @param e         Element to handle (src->cur - the encountered element).
 * 
 * @return Element processing result code.
 */
typedef element_rc element_fn(hidrd_xml_src_inst   *xml_src,
                              xmlNodePtr            e);

#define ELEMENT(_name) \
    static element_rc                                           \
    element_##_name(hidrd_xml_src_inst *xml_src, xmlNodePtr e)

#define ELEMENT_EXIT(_name) \
    static element_rc                                                   \
    element_##_name##_exit(hidrd_xml_src_inst *xml_src, xmlNodePtr e)

ELEMENT(descriptor)
{
    (void)xml_src;
    (void)e;
    /* No item yet */
    return ELEMENT_RC_NONE;
}

ELEMENT_EXIT(descriptor)
{
    (void)xml_src;
    (void)e;
    /* No more items */
    return ELEMENT_RC_END;
}

ELEMENT(COLLECTION)
{
    xmlChar    *type_str    = xmlGetProp(e, BAD_CAST "type");

    hidrd_item_collection_type  type;

    if (type_str == NULL)
        return ELEMENT_RC_ERROR;

    if (!hidrd_item_collection_type_from_token_or_dec(&type,
                                                      (const char *)
                                                        type_str))
        return ELEMENT_RC_ERROR;

    hidrd_item_collection_init(xml_src->item, type);
    return ELEMENT_RC_ITEM;
}

ELEMENT_EXIT(COLLECTION)
{
    (void)e;
    hidrd_item_end_collection_init(xml_src->item);
    return ELEMENT_RC_ITEM;
}

/** Element handler */
typedef struct element_handler {
    const char *name;           /**< Element name */
    element_fn *handle;         /**< Element handling function */
    element_fn *handle_exit;    /**< Element exit handling function */
} element_handler;

/** Element handler list */
static const element_handler handler_list[] = {
#define IGNORE(_name)       {.name = #_name}
#define HANDLE(_name)       {.name = #_name, .func = element_##_name}
#define ENTER(_name)        {.name = #_name, \
                             .handle = element_##_name,             \
                             .handle_exit = element_##_name##_exit}
    IGNORE(basic),
    IGNORE(short),
    IGNORE(main),
    IGNORE(input),
    IGNORE(output),
    IGNORE(feature),
    IGNORE(collection),
    IGNORE(end_collection),
    ENTER(COLLECTION),
    IGNORE(global),
    IGNORE(usage_page),
    IGNORE(logical_minimum),
    IGNORE(logical_maximum),
    IGNORE(physical_minimum),
    IGNORE(physical_maximum),
    IGNORE(unit_exponent),
    IGNORE(unit),
    IGNORE(report_size),
    IGNORE(report_count),
    IGNORE(report_id),
    IGNORE(push),
    IGNORE(pop),
    IGNORE(PUSH),
    IGNORE(local),
    IGNORE(usage),
    IGNORE(usage_minimum),
    IGNORE(usage_maximum),
    IGNORE(designator_index),
    IGNORE(designator_minimum),
    IGNORE(designator_maximum),
    IGNORE(string_index),
    IGNORE(string_minimum),
    IGNORE(string_maximum),
    IGNORE(delimiter),
    IGNORE(SET),
    IGNORE(long),
    ENTER(descriptor),
#undef IGNORE
#undef HANDLE
};


element_rc
element(hidrd_xml_src_inst *xml_src, bool *penter)
{
    const char             *name;
    size_t                  i;
    const element_handler  *handler;

    assert(xml_src != NULL);
    assert(penter != NULL);
    /* We have to process something */
    assert(xml_src->cur != NULL);
    /* We process elements only */
    assert(xml_src->cur->type == XML_ELEMENT_NODE);
    /* Either no parent or an element parent */
    assert(xml_src->prnt == NULL ||
           xml_src->prnt->type == XML_ELEMENT_NODE);

    name = (const char *)xml_src->cur->name;

    for (i = 0; i < sizeof(handler_list) / sizeof(*handler_list); i++)
    {
        handler = handler_list + i;

        if (strcmp(handler->name, name) == 0)
        {
            if (handler->handle_exit != NULL)
                *penter = true;
            if (handler->handle == NULL)
                return ELEMENT_RC_NONE;
            return (*handler->handle)(xml_src, xml_src->cur);
        }
    }

    return ELEMENT_RC_ERROR;
}


element_rc
element_exit(hidrd_xml_src_inst *xml_src)
{
    const char             *name;
    size_t                  i;
    const element_handler  *handler;

    assert(xml_src != NULL);
    /* We have to process something */
    assert(xml_src->prnt != NULL);
    /* We process elements only */
    assert(xml_src->prnt->type == XML_ELEMENT_NODE);
    /* We should be at the end of the node list */
    assert(xml_src->cur == NULL);

    name = (const char *)xml_src->prnt->name;

    for (i = 0; i < sizeof(handler_list) / sizeof(*handler_list); i++)
    {
        handler = handler_list + i;
        if (strcmp(handler->name, name) == 0)
        {
            assert(handler->handle_exit != NULL);
            return (*handler->handle_exit)(xml_src, xml_src->prnt);
        }
    }

    return ELEMENT_RC_ERROR;
}


