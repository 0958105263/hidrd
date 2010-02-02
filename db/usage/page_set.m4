dnl
dnl Usage page set table.
dnl
dnl Copyright (C) 2010 Nikolai Kondrashov
dnl
dnl This file is part of hidrd.
dnl
dnl Hidrd is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl Hidrd is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with hidrd; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
dnl
dnl
dnl PAGE_SET - describe a page set.
dnl Arguments:
dnl     * Set token (lowercase, underscores as spaces)
dnl     * Set description (non-capitalized)
dnl
PAGE_SET(`top_level', `top-level')dnl
PAGE_SET(`reserved',  `reserved')dnl
PAGE_SET(`monitor',   `monitor page')dnl
PAGE_SET(`power',     `power page')dnl
PAGE_SET(`pos',       `POS page')dnl
PAGE_SET(`vendor',    `vendor-defined')dnl
dnl
dnl
dnl PAGE_SET_RANGE_NUM - calculate number of set ranges
dnl Arguments
dnl     * Set token (lowercase, underscores as spaces)
dnl
define(`PAGE_SET_RANGE_NUM',
`pushdef(`$1_range_num', 0)dnl
pushdef(`PAGE_SET_RANGE',
        `ifelse($'`1, $1, `define(`$1_range_num', incr($1_range_num))')')dnl
include(`db/usage/page_set_range.m4')dnl
popdef(`PAGE_SET_RANGE')dnl
$1_range_num`'dnl
popdef(`$1_range_num')')dnl
dnl
