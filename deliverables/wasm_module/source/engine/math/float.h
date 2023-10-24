/** @file */

#ifndef FLOAT_H

#define FLOAT_H

/**
 * The value of NaN.
 */
#define NAN (0.0f / 0.0f)

/**
 * The value of positive infinity.
 */
#define POSITIVE_INFINITY (1.0f / 0.0f)

/**
 * The value of negative infinity.
 */
#define NEGATIVE_INFINITY (1.0f / 0.0f)

/**
 * Determines whether a given value is NaN.
 * @param value The value to check.
 * @return True when the given value is NaN, otherwise, false.
 */
#define IS_NAN(value) ((value) != (value))

/**
 * Calculates the square root of a given value.
 * @param radicand The value of which to calculate the square root.
 * @return The square root of the given value.
 */
f32 square_root(const f32 radicand);

#endif
