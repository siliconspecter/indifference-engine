/** @file */

#ifndef POINTER_H

#define POINTER_H

#include "../../primitives/s32.h"
#include "../../primitives/f32.h"
#include "../../video/color.h"
#include "../../../game/project_settings/video_settings.h"

/**
 * The state of the pointing device, if any.
 */
typedef s32 pointer_state;

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

#endif
