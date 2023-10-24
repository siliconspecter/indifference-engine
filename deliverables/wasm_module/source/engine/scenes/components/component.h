/** @file */

#ifndef COMPONENT_H

#define COMPONENT_H

#include "../primitives/index.h"
#include "../../limits.h"

/**
 * A callback which is called when a component is destroyed.
 * @param meta The arbitrary index of the component which was destroyed.
 */
typedef void(component_destroyed)(index meta);

/**
 * Creates a new component as a direct child of an entity.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @param entity The index of the entity to which to add a component.
 * @param meta An arbitrary index which can be used to look up
 *             component-type-specific data.
 * @param on_destroy Called when the component is destroyed.
 * @return The index of the created component.
 */
index component(
    const index entity,
    const index meta,
    const component_destroyed *const on_destroy);

/**
 * Creates a new component as a direct child of another component.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @param component The index of the component to which to add a component.
 * @param meta An arbitrary index which can be used to look up
 *             component-type-specific data.
 * @param on_destroy Called when the component is destroyed.
 * @return The index of the created component.
 */
index sub_component(
    const index component,
    const index meta,
    const component_destroyed *const on_destroy);

/**
 * Destroys a previously created component and all sub-components within it.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @remark The component's index and the indices of all components within it will
 *         be re-used; ensure that any copies of them are not used once this call
 *         starts.
 * @param component The index of the component to destroy.
 */
void destroy_component(const index component);

#endif
