/** @file */

#ifndef COMPONENT_H

#define COMPONENT_H

#include "../../primitives/index.h"
#include "../../primitives/s32.h"
#include "../../../game/project_settings/limits.h"
#include "../../miscellaneous.h"

/**
 * The number of bits used to store indices specific to component types within
 * component handles.
 */
#define COMPONENT_HANDLE_BITS_FOR_META 8

/**
 * The number of bits used to store parent entity or component indices within
 * component handles.
 */
#define COMPONENT_HANDLE_BITS_FOR_PARENT 8

/**
 * The number of bits used to store component indices within component handles.
 */
#define COMPONENT_HANDLE_BITS_FOR_COMPONENT 8

ASSERT(too_many_bits_for_component_handle, COMPONENT_HANDLE_BITS_FOR_META + COMPONENT_HANDLE_BITS_FOR_PARENT + COMPONENT_HANDLE_BITS_FOR_COMPONENT <= 30);
ASSERT(too_few_meta_bits_for_timer_components, MAXIMUM_TIMER_COMPONENTS <= (1 << COMPONENT_HANDLE_BITS_FOR_META));
ASSERT(too_few_meta_bits_for_sound_components, MAXIMUM_SOUND_COMPONENTS <= (1 << COMPONENT_HANDLE_BITS_FOR_META));
ASSERT(too_few_meta_bits_for_camera_components, MAXIMUM_CAMERA_COMPONENTS <= (1 << COMPONENT_HANDLE_BITS_FOR_META));
ASSERT(too_few_meta_bits_for_opaque_mesh_components, MAXIMUM_OPAQUE_MESH_COMPONENTS <= (1 << COMPONENT_HANDLE_BITS_FOR_META));
ASSERT(too_few_parent_bits_for_entities, MAXIMUM_ENTITIES <= (1 << COMPONENT_HANDLE_BITS_FOR_PARENT));
ASSERT(too_few_parent_bits_for_components, MAXIMUM_COMPONENTS <= (1 << COMPONENT_HANDLE_BITS_FOR_PARENT));
ASSERT(too_few_component_bits, MAXIMUM_COMPONENTS <= (1 << COMPONENT_HANDLE_BITS_FOR_COMPONENT));

/**
 * A handle to a component.
 * @remark This is composed of four fields of bits, from least to most
 *         significant:
 *         - An index which is specific to the component type itself.  This can
 *           be extracted using @ref COMPONENT_HANDLE_META.
 *         - A bit specifying whether the component is the child of an entity
 *           or another component.  This can be checked using
 *           @ref COMPONENT_HANDLE_IS_CHILD_OF_ENTITY or
 *           @ref COMPONENT_HANDLE_IS_CHILD_OF_COMPONENT.
 *         - The index of the component's parent entity or component in the
 *           global pool (primarily used to handle destruction).  This can be
 *           extracted using @ref COMPONENT_HANDLE_PARENT.
 *         - The index of the component itself in the global pool (primarily
 *           used to handle its destruction).  This can be extracted using
 *           @ref COMPONENT_HANDLE_COMPONENT.
 */
typedef s32 component_handle;

/**
 * Extracts the index specific to a type of component from a handle to it.
 * @param component_handle The component handle from which to extract the index
 *                         specific to the component type.
 * @return The index specific to the component type.
 */
#define COMPONENT_HANDLE_META(component_handle) ((component_handle) & ((1 << COMPONENT_HANDLE_BITS_FOR_META) - 1))

/**
 * Determines whether a component handle is for the direct child of an entity.
 * @param component_handle The component handle to check.
 * @return Truthy when the component handle is the direct child of an entity,
 *         otherwise, falsy.
 */
#define COMPONENT_HANDLE_IS_CHILD_OF_ENTITY(component_handle) ((component_handle) & (1 << COMPONENT_HANDLE_BITS_FOR_META))

/**
 * Determines whether a component handle is for the direct child of a component.
 * @param component_handle The component handle to check.
 * @return Truthy when the component handle is the direct child of a component,
 *         otherwise, falsy.
 */
#define COMPONENT_HANDLE_IS_CHILD_OF_COMPONENT(component_handle) (!((component_handle) & (1 << COMPONENT_HANDLE_BITS_FOR_META)))

/**
 * Extracts the parent index from a component handle.
 * @remark Use @ref COMPONENT_HANDLE_IS_CHILD_OF_ENTITY and
 *         @ref COMPONENT_HANDLE_IS_CHILD_OF_CHILD to determine what the index
 *         refers to.
 * @param component_handle The component handle from which to extract the parent
 *                         index.
 * @return The parent index of the component handle.
 */
#define COMPONENT_HANDLE_PARENT(component_handle) (((component_handle) >> (COMPONENT_HANDLE_BITS_FOR_META + 1)) & ((1 << COMPONENT_HANDLE_BITS_FOR_PARENT) - 1))

/**
 * Extracts the index of the component itself from a handle to it.
 * @param component_handle The component handle from which to extract the index
 *                         of the component itself.
 * @return The index of the component itself.
 */
#define COMPONENT_HANDLE_COMPONENT(component_handle) ((component_handle) >> (COMPONENT_HANDLE_BITS_FOR_META + 1 + COMPONENT_HANDLE_BITS_FOR_PARENT))

/**
 * A value which can be used in place of a component handle to represent
 * nothing.
 */
#define COMPONENT_HANDLE_NONE -1

/**
 * A callback which is called when a component is destroyed.
 * @remark This can happen only during a script or the tick event handler.
 * @remark Do not create new entities or components within a component
 *         destructor.
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
    component_destroyed *const on_destroy);

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
    component_destroyed *const on_destroy);

/**
 * Destroys a previously created component and all sub-components within it.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @remark The component's index and the indices of all components within it
 *         will be re-used; ensure that any copies of them are not used once
 *         this call starts.
 * @param component A handle to the component to destroy.
 */
void destroy_component(const component_handle component);

/**
 * Finds the index of the parent entity of a given component handle.  Will
 * recurse up the tree if its direct parent is a component.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @param component The component handle for which to find the parent entity.
 * @return The index of the parent entity of the given component handle,
 *         recursing if the direct parent is a component.
 */
index parent_entity_of(const component_handle component);

/**
 * Destroys all components of an entity and all sub-components within them.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @param entity The index of the entity for which to destroy all components.
 */
void destroy_all_components_of(const index entity);

#endif
