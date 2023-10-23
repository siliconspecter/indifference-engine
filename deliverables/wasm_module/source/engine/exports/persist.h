#ifndef PERSIST_H

#define PERSIST_H

#include "../primitives/s32.h"

/**
 * Indicates that no persistence is to occur until further notice.
 */
#define PERSIST_NOTHING 0

/**
 * A bit flag indicating that the hosting runtime is to persist all game and
 * engine state buffers following the current event handler.
 */
#define PERSIST_STATE 1

/**
 * The persistence state readable by the hosting platform at the end of the
 * current event handler.  Do not change this directly; use @ref persist_state.
 * Automatically set to @ref PERSIST_NOTHING at the start of each event handler.
 */
extern s32 persist;

/**
 * Enqueues a notification to the hosting runtime, that it is to persist all
 * game engine state buffers following the current event handler.
 */
void persist_state();

#endif
