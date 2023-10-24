/** @file */

#ifndef S32_H

#define S32_H

#include <stdint.h>

/**
 * Represents a signed WASM i32.
 */
typedef int32_t s32;

/**
 * The minimum value of a signed WASM i32.
 */
#define S32_MIN INT32_MIN

/**
 * The maximum value of a signed WASM i32.
 */
#define S32_MAX INT32_MAX

#endif
