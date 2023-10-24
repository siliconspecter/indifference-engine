#ifndef ENTITY_H

#define ENTITY_H

#include "../primitives/index.h"
#include "../math/matrix_pair.h"
#include "../limits.h"

/**
 * Creates a new entity with a default identity transform.
 * @remark Will throw a trap should there be no entities left to allocate.
 * @return The index of the created entity.
 */
index entity();

/**
 * The transforms of all entities.
 * @remark Modify only during scripts, selectable callbacks, tick callbacks,
 *         timer expiry callbacks or animation callbacks (doing so in other
 *         situations may produce unexpected results).
 */
extern matrix_pair entity_transforms[MAXIMUM_ENTITIES];

/**
 * Destroys a previously created entity and all components within it.
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @remark The entity's index and the indices of all components within it will
 *         be re-used; ensure that any copies of them are not used once this
 *         call starts.
 * @param entity The index of the entity to destroy.
 */
void destroy_entity(const index entity);

#endif
