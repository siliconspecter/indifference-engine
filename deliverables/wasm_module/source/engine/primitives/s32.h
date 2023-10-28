/** @file */

#ifndef S32_H

#define S32_H

#include "../miscellaneous.h"

/**
 * Represents a signed WASM i32.
 */
typedef signed int s32;

/**
 * The minimum value of a signed WASM i32.
 */
#define S32_MIN -2147483648

/**
 * The maximum value of a signed WASM i32.
 */
#define S32_MAX 2147483647

ASSERT(s32_size, sizeof(s32) == 4);

#endif
