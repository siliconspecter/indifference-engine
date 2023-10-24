/** @file */

#ifndef MATRIX_H

#define MATRIX_H

#include "../primitives/f32.h"
#include "culled_by.h"
#include "vector.h"

/**
 * The number of rows in a matrix.
 */
#define MATRIX_ROWS 4

/**
 * The number of columns in a matrix.
 */
#define MATRIX_COLUMNS 4

/**
 * A matrix.
 */
typedef f32 matrix[MATRIX_ROWS][MATRIX_COLUMNS];

/**
 * An identity matrix, which does not make any changes to coordinates it is
 * applied to.
 */
extern const matrix identity_matrix;

/**
 * Transforms a vector from model space into clip space using a
 * model-view-projection matrix.
 * @remark Coordinates are only culled when W is zero or Z is not within the -1
 *         to 1 range; X and Y may be any values.
 * @param model The vector in model space to transform.
 * @param model_view_projection The model-view-projection matrix to apply.
 * @param clip Overwritten with the corresponding vector in clip space when not
 *             culled, otherwise unmodified.  May be "model".
 * @return The culling which occurred, if any.
 */
culled_by model_to_clip(
    const vector model,
    const matrix model_view_projection,
    vector clip);

/**
 * Transforms a vector from clip space into world space using an inverse
 * model-view matrix.
 * @remark Coordinates are only culled when W is 0; X, Y and Z may be any
 *         values.
 * @param clip The vector in clip space to transform.
 * @param inverse_model_view The inverse model-view matrix to apply.
 * @param world Overwritten with the corresponding vector in world space when
 *              not culled, otherwise unmodified.  May be "clip".
 * @return The culling which occurred, if any.
 */
culled_by clip_to_world(
    const vector clip,
    const matrix inverse_model_view,
    vector world);

/**
 * Calculates the product of two matrices.
 * @param multiplier The first matrix to multiply.
 * @param multiplicand The second matrix to multiply.
 * @param product The matrix to which to write the result.  May be "multiplier",
 *                "multiplicand" or both.
 */
void multiply_matrices(
    const matrix multiplier,
    const matrix multiplicand,
    matrix product);

#endif
