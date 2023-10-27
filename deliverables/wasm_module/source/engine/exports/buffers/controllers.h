/** @file */

#ifndef CONTROLLER_H

#define CONTROLLER_H

#include "../../primitives/s32.h"
#include "../../primitives/f32.h"
#include "../../../game/project_settings/input_settings.h"

/**
 * The state of a controller.  The 8 least significant bits specify the type of
 * controller, as well as whether it is connected.
 */
typedef s32 controller_state;

/**
 * A bit mask for extracting the labelling of a controller from its state.
 */
#define CONTROLLER_STATE_LABELLING_MASK 255

/**
 * This labelling indicates that no controller is connected.
 */
#define CONTROLLER_LABELLING_NONE 0

/**
 * This labelling indicates that a Xbox-labelled controller is connected.
 */
#define CONTROLLER_LABELLING_XBOX 1

/**
 * This labelling indicates that a PlayStation-labelled controller is connected.
 */
#define CONTROLLER_LABELLING_PLAY_STATION 2

/**
 * This labelling indicates that a Nintendo-Switch-labelled controller is connected.
 */
#define CONTROLLER_LABELLING_NINTENDO_SWITCH 3

/**
 * A bit mask for extracting the buttons of a controller from its state.
 */
#define CONTROLLER_STATE_BUTTON_MASK 65280

/**
 * The bit within a controller state for the north face button.
 */
#define CONTROLLER_BUTTON_NORTH_FACE 256

/**
 * The bit within a controller state for the east face button.
 */
#define CONTROLLER_BUTTON_EAST_FACE 512

/**
 * The bit within a controller state for the south face button.
 */
#define CONTROLLER_BUTTON_SOUTH_FACE 1024

/**
 * The bit within a controller state for the west face button.
 */
#define CONTROLLER_BUTTON_WEST_FACE 2048

/**
 * The bit within a controller state for the left shoulder button.
 */
#define CONTROLLER_BUTTON_LEFT_SHOULDER 4096

/**
 * The bit within a controller state for the left shoulder button.
 */
#define CONTROLLER_BUTTON_RIGHT_SHOULDER 8192

/**
 * The bit within a controller state for the pause button.
 */
#define CONTROLLER_BUTTON_PAUSE 16384

/**
 * The states of the controllers.
 */
extern controller_state controller_states[MAXIMUM_CONTROLLERS];

/**
 * The locations of the controllers' sticks, on their Y axes, if known
 * (@see controller_states).
 */
extern f32 controller_y_axes[MAXIMUM_CONTROLLERS];

/**
 * The locations of the controllers' sticks, on their X axes, if known
 * (@see controller_states).
 */
extern f32 controller_x_axes[MAXIMUM_CONTROLLERS];

#endif
