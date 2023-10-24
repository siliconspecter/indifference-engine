/** @file */

#ifndef CULLED_BY_H

#define CULLED_BY_H

#include "../primitives/s32.h"

/**
 * The reason that something was culled.  Falsy when it was NOT culled.
 */
typedef s32 culled_by;

/**
 * No culling has occurred.
 */
#define CULLED_BY_NOTHING 0

/**
 * Culling occurred as W was zero.
 */
#define CULLED_BY_W_ZERO 1

/**
 * Culling occurred as Z was closer than the Z near plane.
 */
#define CULLED_BY_Z_NEAR 2

/**
 * Culling occurred as Z was further than the Z far plane.
 */
#define CULLED_BY_Z_FAR 3

#endif
