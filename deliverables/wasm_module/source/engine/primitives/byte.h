/** @file */

#ifndef BYTE_H

#define BYTE_H

#include "../miscellaneous.h"

/**
 * Represents an unsigned byte.
 */
typedef unsigned char byte;

/**
 * The minimum value of an unsigned byte.
 */
#define BYTE_MIN 0

/**
 * The maximum value of an unsigned byte.
 */
#define BYTE_MAX 255

ASSERT(byte_size, sizeof(byte) == 1);

#endif
