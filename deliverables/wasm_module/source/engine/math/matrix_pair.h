#ifndef MATRIX_PAIR_H

#define MATRIX_PAIR_H

#include "matrix.h"

/**
 * The number of matrices in a matrix pair.
 */
#define MATRIX_PAIR_MATRICES 2

/**
 * The index of the "forward" matrix in a matrix pair.
 */
#define MATRIX_PAIR_FORWARD 0

/**
 * The index of the "inverse" matrix in a matrix pair.
 */
#define MATRIX_PAIR_INVERSE 1

/**
 * A matrix pair.
 */
typedef matrix matrix_pair[MATRIX_PAIR_MATRICES];

/**
 * An identity matrix pair, which does not make any changes to coordinates it is
 * applied to.
 */
extern const matrix_pair identity_matrix_pair;

#endif
