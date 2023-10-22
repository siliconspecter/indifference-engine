#ifndef TRIGONOMETRY_H

#define TRIGONOMETRY_H

#include "../primitives/f32.h"

/**
 * The value of PI.
 */
#define PI 3.14159265359f

/**
 * Calculates the sine of a given angle.
 * @param radians The angle to calculate the sine of, in radians.
 * @return The sine of the given angle.
 */
f32 sine(f32 radians);

/**
 * Calculates the cosine of a given angle.
 * @param radians The angle to calculate the cosine of, in radians.
 * @return The cosine of the given angle.
 */
f32 cosine(f32 radians);

#endif
