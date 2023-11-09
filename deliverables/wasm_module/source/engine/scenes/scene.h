#ifndef SCENE_H

#define SCENE_H

#include "../../game/scripts/scripts.h"
#include "../exports/buffers/persist.h"

/**
 * Destroys all entities in the scene, calls @ref persist_state, then executes
 * the specified script.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should the specified script not be recognized.
 * @param script The script to execute.
 */
void enter(const script script);

/**
 * The script currently being executed.
 * @remark Use @ref enter to change this rather than editing it directly.  It
 *         will be recovered from persistence should the game re-start.
 */
extern script current_script;

#endif
