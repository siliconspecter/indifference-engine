/** @file */

#ifndef FLOAT_COLOR_H

#define FLOAT_COLOR_H

#include "../primitives/f32.h"
#include "color.h"

/**
 * The number of channels in a 128-bit RGBA color.
 */
#define FLOAT_COLOR_CHANNELS 4

/**
 * Represents a 128-bit RGBA color, where each float within represents a
 * channel:
 * - Red (0 = dark, 1 = bright).
 * - Green (0 = dark, 1 = bright).
 * - Blue (0 = dark, 1 = bright).
 * - Opacity (0 = transparent, 1 = opaque).
 * Opacity is NOT pre-multiplied.
 */
typedef f32 float_color[FLOAT_COLOR_CHANNELS];

/**
 * Converts a 32-bit RGBA color to a 128-bit RGBA color.
 * @param color The color to convert.
 * @param float_color Overwritten with the equivalent float color.
 */
void convert_to_float_color(const color color, float_color float_color);

/**
 * Converts a 128-bit RGBA color to a 32-bit RGBA color.
 * @remark Out-of-range channels will be clamped to 0 ... 255.
 * @param float_color The float color to convert.
 * @return The equivalent color.
 */
color convert_to_color(const float_color float_color);

/**
 * Calculates the component-wise sum of two float colors.
 * @param augend The float color to add to.
 * @param addend The float color to add by.
 * @param sum The float color to write the result to.  May be either or both of
 *            the operands.
 */
void add_float_colors(
    const float_color augend,
    const float_color addend,
    float_color sum);

/**
 * Calculates the component-wise difference of two float colors.
 * @param minuend The float color to subtract from.
 * @param subtrahend The float color to subtract by.
 * @param difference The float color to write the result to.  May be either or
 *                   both of the operands.
 */
void subtract_float_colors(
    const float_color minuend,
    const float_color subtrahend,
    float_color difference);

/**
 * Calculates the component-wise product of a float color and a scalar.
 * @param multiplier The float color to multiply.
 * @param multiplicand The scalar to multiply by.
 * @param product The float color to write the result to.  May be the
 *                multiplier.
 */
void multiply_float_color_by_scalar(
    const float_color multiplier,
    const f32 multiplicand,
    float_color product);

/**
 * Modulates two float colors together.
 * @param multiplier The float color to modulate.
 * @param multiplicand The float color to modulate by.
 * @param product The float color to write the result to.  May be either or both
 *                of the operands.
 */
void modulate_float_colors(
    const float_color multiplier,
    const float_color multiplicand,
    float_color product);

#endif
