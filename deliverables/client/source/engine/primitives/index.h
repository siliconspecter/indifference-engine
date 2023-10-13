#ifndef INDEX_H

#define INDEX_H

#include "s32.h"

/**
 * Represents an index into an array.
 */
typedef s32 index;

/**
 * The minimum value of an array index.
 */
#define INDEX_MIN 0

/**
 * The maximum value of an array index.
 */
#define INDEX_MAX S32_MAX

/**
 * A value which can be used in place of an index to represent nothing.
 */
#define INDEX_NONE -1

#endif
