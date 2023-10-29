/** @file */

#ifndef COMPONENT_H

#define COMPONENT_H

#include "../primitives/index.h"
#include "../primitives/s32.h"
#include "../../limits.h"

/**
 * A handle to a component.
 * @remark This is composed of two components:
 *         - The index of the component itself in the global pool (primarily
 *           used to handle its destruction).  This can be extracted using
 *           @ref COMPONENT_INDEX.
 *         - An index which is specific to the component type itself.  This can
 *           be extracted using @ref COMPONENT_META.
 */
typedef s32 component_handle;

/**
 * The maximum value permissible for a component handle's
 * component-type-specific index.
 */
#define COMPONENT_META_MAX 1023

/**
 * Extracts the index of the component itself from a handle to it.
 * @param component_handle The component handle from which to extract the index
 *                         of the component itself.
 * @return The index of the component itself.
 */
#define COMPONENT_HANDLE_INDEX(component_handle) (((component_handle) & 2147482625) >> 10)

/**
 * Extracts the index specific to a type of component from a handle to it.
 * @param component_handle The component handle from which to extract the index
 *                         specific to the component type.
 * @return The index specific to the component type.
 */
#define COMPONENT_HANDLE_META(component_handle) ((component_handle) & 1023)

/**
 * A callback which is called when a component is destroyed.
 * @remark This can happen only during a script or the tick event handler.
 * @param component A handle to the component which was destroyed.
 */
typedef void(component_destroyed)(component_handle component);

/**
 * Creates a new component as a direct child of an entity.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @param entity The index of the entity to which to add a component.
 * @param meta An arbitrary index which can be used to look up
 *             component-type-specific data.  Cannot be greater than
 *             @see COMPONENT_META_MAX.
 * @param on_destroy Called when the component is destroyed.
 * @return A handle to the created component.
 */
component_handle component(
    const index entity,
    const index meta,
    const component_destroyed *const on_destroy);

/**
 * Creates a new component as a direct child of another component.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @param component A handle to the component to which to add a component.
 * @param meta An arbitrary index which can be used to look up
 *             component-type-specific data.  Cannot be greater than
 *             @see COMPONENT_META_MAX.
 * @param on_destroy Called when the component is destroyed.
 * @return A handle to the created component.
 */
component_handle sub_component(
    const component_handle component,
    const index meta,
    const component_destroyed *const on_destroy);

/**
 * Destroys a previously created component and all sub-components within it.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @remark The component's index and the indices of all components within it will
 *         be re-used; ensure that any copies of them are not used once this call
 *         starts.
 * @param component A handle to the component to destroy.
 */
void destroy_component(const component_handle component);

#endif
