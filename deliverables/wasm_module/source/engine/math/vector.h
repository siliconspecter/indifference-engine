#ifndef VECTOR_H

#define VECTOR_H

#include "../primitives/f32.h"

/**
 * The number of components in a three-dimensional vector.
 */
#define VECTOR_COMPONENTS 3

/**
 * A three-dimensional vector.
 */
typedef f32 vector[VECTOR_COMPONENTS];

/**
 * Calculates the component-wise sum of two three-dimensional vectors.
 * @param augend The three-dimensional vector to add to.
 * @param addend The three-dimensional vector to add by.
 * @param sum The three-dimensional vector to write the result to.  May be
 *               either or both of the operands.
 */
void add_vectors(
    const vector augend,
    const vector addend,
    vector sum);

/**
 * Calculates the component-wise difference of two three-dimensional vectors.
 * @param minuend The three-dimensional vector to subtract from.
 * @param subtrahend The three-dimensional vector to subtract by.
 * @param difference The three-dimensional vector to write the result to.  May
 *                   be either or both of the operands.
 */
void subtract_vectors(
    const vector minuend,
    const vector subtrahend,
    vector difference);

/**
 * Calculates the component-wise product of a three-dimensional vector and a
 * scalar.
 * @param multiplier The three-dimensional vector to multiply.
 * @param multiplicand The scalar to multiply by.
 * @param product The three-dimensional vector to write the result to.  May be
 *                the multiplier.
 */
void multiply_vector_by_scalar(
    const vector multiplier,
    const f32 multiplicand,
    vector product);

/**
 * Calculates the dot product of two three-dimensional vectors.
 * @param multiplier The first three-dimensional vector to combine.
 * @param multiplicand The second three-dimensional vector to combine.
 * @return The dot product of the given three-dimensional vectors.
 */
f32 dot_product(
    const vector multiplier,
    const vector multiplicand);

/**
 * Calculates the square of the distance between two three-dimensional vectors.
 * @param a The first three-dimensional vector to measure.
 * @param b The second three-dimensional vector to measure.
 * @return The square of the distance between the given three-dimensional
 *         vectors.
 */
f32 distance_squared(
    const vector a,
    const vector b);

/**
 * Linearly interpolates between two three-dimensional vectors using a scalar.
 * @param origin The three-dimensional vector to linearly interpolate from.
 * @param progress The progress along the linear interpolation, where 0 is the
 *                 origin and 1 is the destination.  Extrapolates beyond that
 *                 range.
 * @param destination The three-dimensional vector to linearly interpolate
 *                    towards.
 * @param interpolated The three-dimensional vector to write the result to.  May
 *                     be any three-dimensional vector operand.
 */
void linearly_interpolate_vectors_by_scalar(
    const vector origin,
    const f32 progress,
    const vector destination,
    vector interpolated);

#endif
