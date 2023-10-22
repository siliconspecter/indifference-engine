#ifndef PERSIST_H

#define PERSIST_H

#include "../primitives/s32.h"

/**
 * Indicates that no persistence is to occur until further notice.
 */
#define PERSIST_NOTHING 0

/**
 * Indicates that the hosting runtime is to persist all game state buffers
 * following the current event handler.
 */
#define PERSIST_GAME_STATE 1

/**
 * The persistence state readable by the hosting platform at the end of the
 * current event handler.  Automatically set to @ref PERSIST_NOTHING at the
 * start of each event handler.
 */
extern s32 persist;

#endif
