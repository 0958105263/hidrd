/** @file
 * @brief HID report descriptor - specification example sink
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

#ifndef __HIDRD_FMT_SPEC_SNK_H__
#define __HIDRD_FMT_SPEC_SNK_H__

#include "hidrd/strm/snk/inst.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Specification example sink type */
const hidrd_snk_type    hidrd_spec_snk;

/** Specification example sink item state table */
typedef struct hidrd_spec_snk_state hidrd_spec_snk_state;
struct hidrd_spec_snk_state {
    hidrd_spec_snk_state   *prev;       /**< Previous state */
    hidrd_usage_page        usage_page; /**< Usage page in effect */
};

/** Specification example sink instance */
typedef struct hidrd_spec_snk_inst {
    hidrd_snk               snk;    /**< Parent structure */
    size_t                  indent; /**< Number of indent columns */

    size_t                  depth;  /**< Current nesting depth */
    hidrd_spec_snk_state   *state;  /**< Item state table stack */

    void                   *buf;    /**< Buffer pointer */
    size_t                  size;   /**< Buffer size in bytes */
    size_t                  pos;    /**< Buffer position in bytes */
} hidrd_spec_snk_inst;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HIDRD_FMT_SPEC_SNK_H__ */
