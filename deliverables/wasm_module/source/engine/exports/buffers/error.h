/** @file */

#ifndef ERROR_H

#define ERROR_H

#include "../../primitives/s32.h"

/**
 * Indicates that no error has occurred.
 */
#define ERROR_NONE 0

/**
 * Indicates that no entities were left to allocate.
 */
#define ERROR_NO_ENTITIES_TO_ALLOCATE -1

/**
 * Indicates that the specified entities does not exist.
 */
#define ERROR_ENTITY_DOES_NOT_EXIST -2

/**
 * Indicates that no components were left to allocate.
 */
#define ERROR_NO_COMPONENTS_TO_ALLOCATE -3

/**
 * Indicates that the specified component does not exist.
 */
#define ERROR_COMPONENT_DOES_NOT_EXIST -4

/**
 * Indicates that no camera components were left to allocate.
 */
#define ERROR_NO_CAMERA_COMPONENTS_TO_ALLOCATE -4

/**
 * Indicates that the game was requested to run a script it did not support.
 */
#define ERROR_UNKNOWN_SCRIPT -5

/**
 * The error number readable by the hosting platform at the end of the current
 * event handler.  Positive values are generated by the game, while negative
 * values are generated by the engine.  Do not change this directly; use
 * @ref throw.  Automatically set to @ref ERROR_NONE at the start of each event
 * handler.
 */
extern s32 error;

/**
 * Call from anywhere to immediately halt execution with an error.  The hosting
 * runtime will NOT raise any further event handlers nor persist any buffers
 * until the game is fully restarted.
 * @param error The number of the error to raise.  Positive values are
 *              game-specific, while negative values are engine-specific.
 */
__attribute__((noreturn)) void throw(const s32 error);

#endif
