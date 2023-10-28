/** @file */

#ifndef F32_H

#define F32_H

#include "../miscellaneous.h"

/**
 * Represents a WASM f32.
 */
typedef float f32;

ASSERT(f32_size, sizeof(f32) == 4);

#endif
