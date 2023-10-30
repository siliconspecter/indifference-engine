/** @file */

#ifndef COLOR_H

#define COLOR_H

#include "../primitives/s32.h"
#include "../miscellaneous.h"

/**
 * Represents a 32-bit RGBA color, where each byte within represents a channel,
 * from least to most significant byte:
 * - Red (0 = dark, 255 = bright).
 * - Green (0 = dark, 255 = bright).
 * - Blue (0 = dark, 255 = bright).
 * - Opacity (0 = transparent, 255 = opaque).
 * Opacity is NOT pre-multiplied.
 */
typedef unsigned int color;

ASSERT(color_size, sizeof(color) == 4);

/**
 * Extracts the red component from a given color.
 * @param color The color to extract the red component from.
 * @return The red component extracted from the given color.
 */
#define EXTRACT_RED(color) ((color)&255)

/**
 * Extracts the green component from a given color.
 * @param color The color to extract the green component from.
 * @return The green component extracted from the given color.
 */
#define EXTRACT_GREEN(color) (((color) >> 8) & 255)

/**
 * Extracts the blue component from a given color.
 * @param color The color to extract the blue component from.
 * @return The blue component extracted from the given color.
 */
#define EXTRACT_BLUE(color) (((color) >> 16) & 255)

/**
 * Extracts the opacity component from a given color.
 * @param color The color to extract the opacity component from.
 * @return The opacity component extracted from the given color.
 */
#define EXTRACT_OPACITY(color) ((color) >> 24)

/**
 * Creates a color.  Values are NOT clamped and behavior is undefined when
 * channels are less than 0 or greater than 255.
 * @param red The intensity of the red channel, where 0 is dark and 255 is
 *            bright.
 * @param green The intensity of the green channel, where 0 is dark and 255 is
 *              bright.
 * @param blue The intensity of the blue channel, where 0 is dark and 255 is
 *             bright.
 * @param opacity The intensity of the blue channel, where 0 is transparent and
 *              255 is opaque.
 * @return The created color.
 */
#define UNCLAMPED_COLOR(red, green, blue, opacity) ((color)((red) | ((green) << 8) | ((blue) << 16) | ((opacity) << 24)))

/**
 * Creates a color, clamping all channels to the range 0 to 255.
 * @param red The intensity of the red channel, where 0 is dark and 255 is
 *            bright.
 * @param green The intensity of the green channel, where 0 is dark and 255 is
 *              bright.
 * @param blue The intensity of the blue channel, where 0 is dark and 255 is
 *             bright.
 * @param opacity The intensity of the blue channel, where 0 is transparent and
 *              255 is opaque.
 * @return The created color.
 */
#define CLAMPED_COLOR(red, green, blue, opacity) UNCLAMPED_COLOR(CLAMP((red), 0, 255), CLAMP((green), 0, 255), CLAMP((blue), 0, 255), CLAMP((opacity), 0, 255))

/**
 * A color which is fully transparent.
 */
#define TRANSPARENT UNCLAMPED_COLOR(0, 0, 0, 0)

/**
 * Creates an opaque color.  Values are NOT clamped and behavior is undefined
 * when channels are less than 0 or greater than 255.
 * @param red The intensity of the red channel, where 0 is dark and 255 is
 *            bright.
 * @param green The intensity of the green channel, where 0 is dark and 255 is
 *              bright.
 * @param blue The intensity of the blue channel, where 0 is dark and 255 is
 *             bright.
 * @return The created color.
 */
#define UNCLAMPED_OPAQUE_COLOR(red, green, blue) UNCLAMPED_COLOR(red, green, blue, 255)

/**
 * Creates an opaque color, clamping all channels to the range 0 to 255.
 * @param red The intensity of the red channel, where 0 is dark and 255 is
 *            bright.
 * @param green The intensity of the green channel, where 0 is dark and 255 is
 *              bright.
 * @param blue The intensity of the blue channel, where 0 is dark and 255 is
 *             bright.
 * @return The created color.
 */
#define CLAMPED_OPAQUE_COLOR(red, green, blue) CLAMPED_OPAQUE_COLOR(red, green, blue, 255)

/**
 * A color which is fully black and opaque.
 */
#define BLACK UNCLAMPED_OPAQUE_COLOR(0, 0, 0)

/**
 * A color which is fully white and opaque.
 */
#define WHITE UNCLAMPED_OPAQUE_COLOR(255, 255, 255)

/**
 * A color which is fully red and opaque.
 */
#define RED UNCLAMPED_OPAQUE_COLOR(255, 0, 0)

/**
 * A color which is fully yellow and opaque.
 */
#define YELLOW UNCLAMPED_OPAQUE_COLOR(255, 255, 0)

/**
 * A color which is fully green and opaque.
 */
#define GREEN UNCLAMPED_OPAQUE_COLOR(0, 255, 0)

/**
 * A color which is fully cyan and opaque.
 */
#define CYAN UNCLAMPED_OPAQUE_COLOR(0, 255, 255)

/**
 * A color which is fully blue and opaque.
 */
#define BLUE UNCLAMPED_OPAQUE_COLOR(0, 0, 255)

/**
 * A color which is fully magenta and opaque.
 */
#define MAGENTA UNCLAMPED_OPAQUE_COLOR(255, 0, 255)

/**
 * Modulates two colors together.
 * @param a The first color to modulate.
 * @param b The second color to modulate.
 * @return The modulated color
 */
#define MODULATE_COLORS(a, b) UNCLAMPED_COLOR(   \
    (EXTRACT_RED(a) * EXTRACT_RED(b)) / 255,     \
    (EXTRACT_GREEN(a) * EXTRACT_GREEN(b)) / 255, \
    (EXTRACT_BLUE(a) * EXTRACT_BLUE(b)) / 255,   \
    (EXTRACT_OPACITY(a) * EXTRACT_OPACITY(b)) / 255)

/**
 * Layers one color onto another.
 * @param source The color to layer on top.
 * @param destination The color being layered onto.
 * @return The layered color.
 */
#define LAYER_COLORS(source, destination) UNCLAMPED_COLOR(                                                                      \
    ((EXTRACT_RED(source) * EXTRACT_OPACITY(source)) + (EXTRACT_RED(destination) * (255 - EXTRACT_OPACITY(source)))) / 255,     \
    ((EXTRACT_GREEN(source) * EXTRACT_OPACITY(source)) + (EXTRACT_GREEN(destination) * (255 - EXTRACT_OPACITY(source)))) / 255, \
    ((EXTRACT_BLUE(source) * EXTRACT_OPACITY(source)) + (EXTRACT_BLUE(destination) * (255 - EXTRACT_OPACITY(source)))) / 255,   \
    255 - ((255 - EXTRACT_OPACITY(source)) * (255 - EXTRACT_OPACITY(destination))) / 255)

#endif
