#ifndef COLOR_H

#define COLOR_H

#include "../primitives/byte.h"

/**
 * The number of channels in a color.
 */
#define COLOR_CHANNELS 4

/**
 * The index of the red channel in a color.
 */
#define COLOR_RED 0

/**
 * The index of the green channel in a color.
 */
#define COLOR_GREEN 1

/**
 * The index of the blue channel in a color.
 */
#define COLOR_BLUE 2

/**
 * The index of the opacity channel in a color.
 */
#define COLOR_OPACITY 3

/**
 * A matrix.
 */
typedef byte color[COLOR_CHANNELS];

#endif
