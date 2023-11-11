#ifndef SCRIPTS_H

#define SCRIPTS_H

#include "../../engine/exports/buffers/error.h"
#include "../../engine/exports/buffers/persist.h"
#include "../../engine/primitives/s32.h"

/**
 * The ID of a script.
 */
typedef s32 script;

/**
 * The identifier of the first script ran when the game starts.  This must
 * always be 0.
 */
#define SCRIPT_START 0

/**
 * Called by the engine when a script should be ran.
 * @remark This can happen only during a tick event handler, or at the very
 *         start of the first event handler.
 * @remark The scene will always be empty.  This is where it should be seeded
 *         with new entities and components.  @ref persist_state will be called
 *         on your behalf.
 * @remark Call @ref throw with @ref ERROR_UNKNOWN_SCRIPT should the given
 *         script ID not be recognized.
 * @param script The ID of the script to run.
 */
void script_body(const script script);

#endif
