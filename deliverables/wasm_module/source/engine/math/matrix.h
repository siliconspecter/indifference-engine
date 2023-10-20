#ifndef MATRIX_H

#define MATRIX_H

#include "../primitives/f32.h"

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

#endif
