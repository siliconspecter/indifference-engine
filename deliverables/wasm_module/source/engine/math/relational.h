/** @file */

#ifndef RELATIONAL_H

#define RELATIONAL_H

/**
 * Determines the lesser of two given values.
 * @param a The first value to compare.
 * @param b The second value to compare.
 * @return The lesser of the two given values.
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * Determines the greater of two given values.
 * @param a The first value to compare.
 * @param b The second value to compare.
 * @return The greater of the two given values.
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * Clamps a value to a range.
 * @param value The value to clamp.
 * @param min The lower bound to which to clamp.
 * @param max The upper bound to which to clamp.
 * @return The clamped value.
 */
#define CLAMP(value, min, max) MIN((max), MAX((min), (value)))

#endif
