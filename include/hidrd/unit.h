/** @file
 * @brief HID report descriptor - unit value
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

#ifndef __HIDRD_UNIT_H__
#define __HIDRD_UNIT_H__

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Unit system */
typedef enum hidrd_unit_system {
    HIDRD_UNIT_SYSTEM_NONE              = 0x0,
    HIDRD_UNIT_SYSTEM_SI_LINEAR         = 0x1,
    HIDRD_UNIT_SYSTEM_SI_ROTATION       = 0x2,
    HIDRD_UNIT_SYSTEM_ENGLISH_LINEAR    = 0x3,
    HIDRD_UNIT_SYSTEM_ENGLISH_ROTATION  = 0x4,
    HIDRD_UNIT_SYSTEM_VENDOR            = 0xF
} hidrd_unit_system;

#define HIDRD_UNIT_SYSTEM_MIN   HIDRD_UNIT_SYSTEM_NONE
#define HIDRD_UNIT_SYSTEM_MAX   HIDRD_UNIT_SYSTEM_VENDOR

#define HIDRD_UNIT_SYSTEM_KNOWN_MIN HIDRD_UNIT_SYSTEM_SI_LINEAR
#define HIDRD_UNIT_SYSTEM_KNOWN_MAX HIDRD_UNIT_SYSTEM_ENGLISH_ROTATION

#define HIDRD_UNIT_SYSTEM_RESERVED_MIN  0x5
#define HIDRD_UNIT_SYSTEM_RESERVED_MAX  0xE

/**
 * Check if a unit system is valid.
 *
 * @param system    Unit system to check.
 *
 * @return True if the system is valid, false otherwise.
 */
static inline bool
hidrd_unit_system_valid(hidrd_unit_system system)
{
    return system <= HIDRD_UNIT_SYSTEM_MAX;
}


/**
 * Convert a unit system code to decimal string.
 *
 * @param system    Unit system to convert.
 *
 * @return Dynamically allocated unit system code decimal string, or NULL if
 *         failed to allocate memory.
 */
/* FIXME Make extern */
static inline char *
hidrd_unit_system_to_dec(hidrd_unit_system system)
{
    char   *dec;

    assert(hidrd_unit_system_valid(system));

    if (asprintf(&dec, "%u", system) < 0)
        return NULL;

    return dec;
}


/**
 * Convert a unit system decimal string to a unit system code.
 *
 * @param psystem   Location for resulting unit system code; will not be
 *                  modified in case of error; could be NULL.
 * @param dec       Unit system decimal string to convert.
 *
 * @return True if converted successfully (decimal string was valid), false
 *         otherwise.
 */
static inline bool
hidrd_unit_system_from_dec(hidrd_unit_system *psystem, const char *dec)
{
    hidrd_unit_system   system;

    assert(dec != NULL);

    if ((sscanf(dec, "%u", &system) != 1) ||
        !hidrd_unit_system_valid(system))
        return false;

    if (psystem != NULL)
        *psystem = system;

    return true;
}


#ifdef HIDRD_WITH_TOKENS

/**
 * Convert a unit system code to a token.
 *
 * @param system    Unit system.
 *
 * @return Constant unit system token string, or NULL if none.
 */
/* FIXME Make extern */
static inline const char *
hidrd_unit_system_to_token(hidrd_unit_system system)
{
    assert(hidrd_unit_system_valid(system));

    switch (system)
    {
#define MAP(_NAME, _name) \
    case HIDRD_UNIT_SYSTEM_##_NAME: \
        return #_name

        MAP(NONE, none);
        MAP(SI_LINEAR, si_linear);
        MAP(SI_ROTATION, si_rotation);
        MAP(ENGLISH_LINEAR, english_linear);
        MAP(ENGLISH_ROTATION, english_rotation);
        MAP(VENDOR, vendor);

#undef MAP
        default:
            return NULL;
    }
}

/**
 * Convert a unit system token to a code.
 *
 * @param psystem   Location for the resulting system code; will not be
 *                  changed in case of error; could be NULL.
 * @param token     Unit system token.
 *
 * @return True if the token was recognized, false otherwise.
 */
/* FIXME Make extern */
static inline bool
hidrd_unit_system_from_token(hidrd_unit_system *psystem, const char *token)
{
    hidrd_unit_system   system;

    assert(token != NULL);

#define MAP(_NAME, _name) \
    do {                                        \
        if (strcasecmp(token, #_name) == 0)     \
        {                                       \
            system = HIDRD_UNIT_SYSTEM_##_NAME; \
            goto found;                         \
        }                                       \
    } while (0)

    MAP(NONE, none);
    MAP(SI_LINEAR, si_linear);
    MAP(SI_ROTATION, si_rotation);
    MAP(ENGLISH_LINEAR, english_linear);
    MAP(ENGLISH_ROTATION, english_rotation);
    MAP(VENDOR, vendor);

#undef MAP

    return false;

found:

    if (psystem != NULL)
        *psystem = system;

    return true;
}


/**
 * Convert a unit system code to a token or (if there is no token) to a
 * decimal string.
 *
 * @param system    Unit system code to convert.
 *
 * @return Dynamically allocated unit system token or decimal string; NULL
 *         if failed to allocate memory.
 */
/* FIXME Make extern */
static inline char *
hidrd_unit_system_to_token_or_dec(hidrd_unit_system system)
{
    const char *token;

    assert(hidrd_unit_system_valid(system));

    token = hidrd_unit_system_to_token(system);

    return (token != NULL)
            ? strdup(token)
            : hidrd_unit_system_to_dec(system);
}


/**
 * Convert a unit system token or decimal string to a unit system code.
 *
 * @param psystem       Location for resulting unit system code; will not be
 *                      modified in case of error; could be NULL.
 * @param token_or_dec  Unit system token or decimal string to convert.
 *
 * @return True if either unit system token was recognized or decimal string
 *         was valid, false otherwise.
 */
/* FIXME Make extern */
static inline bool
hidrd_unit_system_from_token_or_dec(hidrd_unit_system  *psystem,
                                    const char         *token_or_dec)
{
    assert(token_or_dec != NULL);

    return hidrd_unit_system_from_token(psystem, token_or_dec) ||
           hidrd_unit_system_from_dec(psystem, token_or_dec);
}

#endif /* HIDRD_WITH_TOKENS */

/**
 * Check if a unit system is known.
 *
 * @param system    Unit system to check.
 *
 * @return True if the system is known, false otherwise.
 */
static inline bool
hidrd_unit_system_known(hidrd_unit_system system)
{
    return (system >= HIDRD_UNIT_SYSTEM_KNOWN_MIN) &&
           (system <= HIDRD_UNIT_SYSTEM_KNOWN_MAX);
}

/**
 * Check if a unit system is reserved.
 *
 * @param system    Unit system to check.
 *
 * @return True if the system is reserved, false otherwise.
 */
static inline bool
hidrd_unit_system_reserved(hidrd_unit_system system)
{
    return (system >= HIDRD_UNIT_SYSTEM_RESERVED_MIN) &&
           (system <= HIDRD_UNIT_SYSTEM_RESERVED_MAX);
}

/** Unit exponent */
typedef enum hidrd_unit_exp {
    HIDRD_UNIT_EXP_0        = 0x0,
    HIDRD_UNIT_EXP_1        = 0x1,
    HIDRD_UNIT_EXP_2        = 0x2,
    HIDRD_UNIT_EXP_3        = 0x3,
    HIDRD_UNIT_EXP_4        = 0x4,
    HIDRD_UNIT_EXP_5        = 0x5,
    HIDRD_UNIT_EXP_6        = 0x6,
    HIDRD_UNIT_EXP_7        = 0x7,
    HIDRD_UNIT_EXP_MINUS_8  = 0x8,
    HIDRD_UNIT_EXP_MINUS_7  = 0x9,
    HIDRD_UNIT_EXP_MINUS_6  = 0xA,
    HIDRD_UNIT_EXP_MINUS_5  = 0xB,
    HIDRD_UNIT_EXP_MINUS_4  = 0xC,
    HIDRD_UNIT_EXP_MINUS_3  = 0xD,
    HIDRD_UNIT_EXP_MINUS_2  = 0xE,
    HIDRD_UNIT_EXP_MINUS_1  = 0xF,
} hidrd_unit_exp;

#define HIDRD_UNIT_EXP_MIN  HIDRD_UNIT_EXP_0
#define HIDRD_UNIT_EXP_MAX  HIDRD_UNIT_EXP_MINUS_1

/**
 * Check if a unit exponent is valid.
 *
 * @param exp   Unit exponent to check.
 *
 * @return True if the unit exponent is valid, false otherwise.
 */
static inline bool
hidrd_unit_exp_valid(hidrd_unit_exp exp)
{
    return exp <= HIDRD_UNIT_EXP_MAX;
}


/**
 * Validate a unit exponent.
 *
 * @param exp   Unit exponent to validate.
 *
 * @return Validated exponent.
 */
static inline hidrd_unit_exp
hidrd_unit_exp_validate(hidrd_unit_exp exp)
{
    assert(hidrd_unit_exp_valid(exp));
    return exp;
}


#define HIDRD_UNIT_EXP_MIN_INT  -8
#define HIDRD_UNIT_EXP_MAX_INT  7

/**
 * Check if an integer number representation of a unit exponent is valid.
 *
 * @param i Unit exponent integer to check.
 *
 * @return True if the integer is valid, false otherwise.
 */
static inline bool
hidrd_unit_exp_valid_int(int i)
{
    return (i >= HIDRD_UNIT_EXP_MIN_INT) &&
           (i <= HIDRD_UNIT_EXP_MAX_INT);
}


/**
 * Validate an integer number representation of a unit exponent.
 *
 * @param i Unit exponent integer to validate.
 *
 * @return Validated integer.
 */
static inline int
hidrd_unit_exp_validate_int(int i)
{
    assert(hidrd_unit_exp_valid_int(i));
    return i;
}


/**
 * Convert a unit exponent code to an integer number.
 *
 * @param exp   Exponent to convert from.
 *
 * @return Integer number representation of the exponent.
 */
static inline int
hidrd_unit_exp_to_int(hidrd_unit_exp exp)
{
    assert(hidrd_unit_exp_valid(exp));
    return (exp & 0x8) ? ((int)exp - 0x10) : (int)exp;
}


/**
 * Convert an integer number representation of unit exponent to a unit
 * exponent code.
 *
 * @param i Integer number representation to convert from.
 *
 * @return Unit exponent code corresponding to the integer.
 */
static inline hidrd_unit_exp
hidrd_unit_exp_from_int(int i)
{
    assert(hidrd_unit_exp_valid_int(i));
    return (i < 0) ? (i + 0x10) : i;
}

/** Unit nibble (4-bit field) index */
typedef enum hidrd_unit_nibble_index {
    HIDRD_UNIT_NIBBLE_INDEX_SYSTEM              = 0x0,
    HIDRD_UNIT_NIBBLE_INDEX_LENGTH,
    HIDRD_UNIT_NIBBLE_INDEX_MASS,
    HIDRD_UNIT_NIBBLE_INDEX_TIME,
    HIDRD_UNIT_NIBBLE_INDEX_TEMPERATURE,
    HIDRD_UNIT_NIBBLE_INDEX_CURRENT,
    HIDRD_UNIT_NIBBLE_INDEX_LUMINOUS_INTENSITY,
    HIDRD_UNIT_NIBBLE_INDEX_RESERVED,
} hidrd_unit_nibble_index;


/**
 * Check if a unit nibble index is valid.
 *
 * @param i Nibble index to check.
 *
 * @return True if the index is valid, false otherwise.
 */
static inline bool
hidrd_unit_nibble_index_valid(hidrd_unit_nibble_index i)
{
    return (i <= 7);
}


/** Unit value type */
typedef uint32_t hidrd_unit;

/**
 * Check if a unit is valid (currently a stub - always true).
 *
 * @param unit  Unit to check.
 *
 * @return True if the unit is valid, false otherwise.
 */
static inline bool
hidrd_unit_valid(hidrd_unit unit)
{
    (void)unit;
    return true;
}


/**
 * Retrieve a unit nibble (4-bit field) value, specified by index.
 *
 * @param unit  Unit to retrieve nibble from.
 * @param i     Nibble index to retrieve from.
 *
 * @return The retrieved nibble value.
 */
static inline uint8_t
hidrd_unit_get_nibble(hidrd_unit unit, hidrd_unit_nibble_index i)
{
    assert(hidrd_unit_valid(unit));
    assert(hidrd_unit_nibble_index_valid(i));

    return (unit >> (i * 4)) & 0xF;
}


/**
 * Set a unit nibble (4-bit field) value, specified by index.
 *
 * @param unit  Unit to set nibble to.
 * @param i     Nibble index to set to.
 * @param value Nibble value to set.
 *
 * @return The unit with the nibble set.
 */ 
static inline hidrd_unit
hidrd_unit_set_nibble(hidrd_unit unit, uint8_t i, uint8_t value)
{
    assert(hidrd_unit_valid(unit));
    assert(hidrd_unit_nibble_index_valid(i));
    assert(value <= 0xF);

    return (unit & ~(hidrd_unit)(0xF << (i * 4))) | (value << (i * 4));
}


#define HIDRD_UNIT_NIBBLE_ACCESSOR_PAIR(_type, _name, _NAME) \
    static inline hidrd_unit_##_type                                    \
    hidrd_unit_get_##_name(hidrd_unit unit)                             \
    {                                                                   \
        assert(hidrd_unit_valid(unit));                                 \
        return hidrd_unit_get_nibble(unit,                              \
                                     HIDRD_UNIT_NIBBLE_INDEX_##_NAME);  \
    }                                                                   \
                                                                        \
    static inline hidrd_unit                                            \
    hidrd_unit_set_##_name(hidrd_unit unit, hidrd_unit_##_type value)   \
    {                                                                   \
        assert(hidrd_unit_valid(unit));                                 \
        assert(hidrd_unit_##_type##_valid(value));                      \
        return hidrd_unit_set_nibble(unit,                              \
                                     HIDRD_UNIT_NIBBLE_INDEX_##_NAME,   \
                                     value);                            \
    }


HIDRD_UNIT_NIBBLE_ACCESSOR_PAIR(system, system, SYSTEM)
HIDRD_UNIT_NIBBLE_ACCESSOR_PAIR(exp, length, LENGTH)
HIDRD_UNIT_NIBBLE_ACCESSOR_PAIR(exp, mass, MASS)
HIDRD_UNIT_NIBBLE_ACCESSOR_PAIR(exp, time, TIME)
HIDRD_UNIT_NIBBLE_ACCESSOR_PAIR(exp, temperature, TEMPERATURE)
HIDRD_UNIT_NIBBLE_ACCESSOR_PAIR(exp, current, CURRENT)
HIDRD_UNIT_NIBBLE_ACCESSOR_PAIR(exp, luminous_intensity, LUMINOUS_INTENSITY)

/**
 * Check if a unit value is known (in effect if it could be interpreted
 * using the functions and constants provided in this module).
 *
 * @param unit  Unit value to check.
 *
 * @return True if the unit value is known, false otherwise.
 */
static inline bool
hidrd_unit_known(hidrd_unit unit)
{
    /* The only problem we have is the reserved nibble */
    return hidrd_unit_get_nibble(unit,
                                 HIDRD_UNIT_NIBBLE_INDEX_RESERVED) == 0;
}

/**
 * Check if a unit value is void.
 *
 * @param unit  Unit value to check.
 *
 * @return True if the unit value is void, false otherwise.
 */
static inline bool
hidrd_unit_void(hidrd_unit unit)
{
    /*
     * If there is no system or all the exponents (including reserved) are
     * zeroes.
     */
    return hidrd_unit_get_system(unit) == HIDRD_UNIT_SYSTEM_NONE ||
           /* May you forgive us for using an obscure constant :) */
           ((unit & 0xFFFFFFF0) == 0);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HIDRD_UNIT_H__ */
