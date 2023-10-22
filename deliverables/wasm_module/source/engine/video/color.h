#ifndef COLOR_H

#define COLOR_H

#include "../primitives/byte.h"

/**
 * The number of channels in a color.
 */
#define COLOR_CHANNELS 4

/**
 * The index of the red channel in a color.  This is an intensity value, where 0
 * is as dark as possible and 255 is as bright as possible.
 */
#define COLOR_RED 0

/**
 * The index of the green channel in a color.  This is an intensity value, where
 * 0 is as dark as possible and 255 is as bright as possible.
 */
#define COLOR_GREEN 1

/**
 * The index of the blue channel in a color.  This is an intensity value, where
 * 0 is as dark as possible and 255 is as bright as possible.
 */
#define COLOR_BLUE 2

/**
 * The index of the opacity channel in a color.  0 is fully transparent and 255
 * is fully opaque.
 */
#define COLOR_OPACITY 3

/**
 * A 32-bits-per-pixel red-green-blue-opacity color.  Opacity is NOT
 * pre-multiplied.
 */
typedef byte color[COLOR_CHANNELS];

#endif
