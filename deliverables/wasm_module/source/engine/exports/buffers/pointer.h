/** @file */

#ifndef POINTER_H

#define POINTER_H

#include "../../primitives/s32.h"

/**
 * No pointing device is known to be available.
 */
#define POINTER_STATE_NONE 0

/**
 * A mouse is connected and its location is known.  It is not currently
 * clicking.
 */
#define POINTER_STATE_MOUSE 1

/**
 * A mouse is connected and its location is known.  It is currently clicking its
 * primary button.
 */
#define POINTER_STATE_MOUSE_CLICKING_PRIMARY 2

/**
 * The state of the pointing device, if any.
 */
extern s32 pointer_state;

/**
 * The number of rows between the top of the video buffer and the pointer, if
 * known (@ref pointer_state).
 */
extern s32 pointer_row;

/**
 * The number of columns between the left of the video buffer and the pointer,
 * if known (@ref pointer_state).
 */
extern s32 pointer_column;

#endif
