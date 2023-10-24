/** @file */

#ifndef VECTOR_H

#define VECTOR_H

#include "../primitives/f32.h"

/**
 * The number of components in a 3D vector.
 */
#define VECTOR_COMPONENTS 3

/**
 * A 3D vector.
 */
typedef f32 vector[VECTOR_COMPONENTS];

/**
 * Calculates the component-wise sum of two vectors.
 * @param augend The vector to add to.
 * @param addend The vector to add by.
 * @param sum The vector to write the result to.  May be either or both of the
 *            operands.
 */
void add_vectors(
    const vector augend,
    const vector addend,
    vector sum);

/**
 * Calculates the component-wise difference of two vectors.
 * @param minuend The vector to subtract from.
 * @param subtrahend The vector to subtract by.
 * @param difference The vector to write the result to.  May be either or both
 *                   of the operands.
 */
void subtract_vectors(
    const vector minuend,
    const vector subtrahend,
    vector difference);

/**
 * Calculates the component-wise product of a vector and a scalar.
 * @param multiplier The vector to multiply.
 * @param multiplicand The scalar to multiply by.
 * @param product The vector to write the result to.  May be the multiplier.
 */
void multiply_vector_by_scalar(
    const vector multiplier,
    const f32 multiplicand,
    vector product);

/**
 * Calculates the dot product of two vectors.
 * @param multiplier The first vector to combine.
 * @param multiplicand The second vector to combine.
 * @return The dot product of the given vectors.
 */
f32 dot_product(
    const vector multiplier,
    const vector multiplicand);

/**
 * Calculates the square of the distance between two vectors.
 * @param origin The first vector to measure.
 * @param destination The second vector to measure.
 * @return The square of the distance between the given three-dimensional
 *         vectors.
 */
f32 distance_squared(
    const vector origin,
    const vector destination);

/**
 * Linearly interpolates between two vectors using a scalar.
 * @param origin The vector to linearly interpolate from.
 * @param progress The progress along the linear interpolation, where 0 is the
 *                 origin and 1 is the destination.  Extrapolates beyond that
 *                 range.
 * @param destination The vector to linearly interpolate towards.
 * @param interpolated The vector to write the result to.  May be any vector
 *                     operand.
 */
void linearly_interpolate_vectors_by_scalar(
    const vector origin,
    const f32 progress,
    const vector destination,
    vector interpolated);

#endif
