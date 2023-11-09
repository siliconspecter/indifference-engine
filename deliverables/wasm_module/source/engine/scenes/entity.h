/** @file */

#ifndef ENTITY_H

#define ENTITY_H

#include "../primitives/index.h"
#include "../math/matrix.h"
#include "../limits.h"

/**
 * Creates a new entity with a default identity transform.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should there be no entities left to allocate.
 * @return The index of the created entity.
 */
index entity();

/**
 * The forward transforms of all entities at the start of the tick.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Any changes made to this array MUST be mirrored in
 *         @ref previous_inverse_entity_transforms.
 */
extern matrix previous_entity_transforms[MAXIMUM_ENTITIES];

/**
 * The forward transforms of all entities at the end of the tick.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Any changes made to this array MUST be mirrored in
 *         @ref next_inverse_entity_transforms.
 */
extern matrix next_entity_transforms[MAXIMUM_ENTITIES];

/**
 * The inverse transforms of all entities at the start of the tick.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Any changes made to this array MUST be mirrored in
 *         @ref previous_entity_transforms.
 */
extern matrix previous_inverse_entity_transforms[MAXIMUM_ENTITIES];

/**
 * The inverse transforms of all entities at the end of the tick.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Any changes made to this array MUST be mirrored in
 *         @ref next_entity_transforms.
 */
extern matrix next_inverse_entity_transforms[MAXIMUM_ENTITIES];

/**
 * Destroys a previously created entity and all components within it.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @remark The entity's index and the indices of all components within it will
 *         be re-used; ensure that any copies of them are not used once this
 *         call starts.
 * @param entity The index of the entity to destroy.
 */
void destroy_entity(const index entity);

#endif
