/** @file */

#ifndef OPAQUE_MESH_COMPONENT_H

#define OPAQUE_MESH_COMPONENT_H

#include "component.h"
#include "../../primitives/f32.h"
#include "../../primitives/index.h"
#include "../../../game/project_settings/limits.h"
#include "../../assets/mesh.h"

/**
 * Creates a new opaque mesh component as a direct child of an entity.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should there be no opaque mesh components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @param entity The index of the entity to which to add an opaque mesh
 *               component.
 * @param mesh The initial mesh to display.
 * @return A handle to the created opaque mesh component.
 */
component_handle opaque_mesh_component(
    const index entity,
    const mesh *const mesh);

/**
 * Creates a new opaque mesh component as a direct child of another component.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should there be no opaque mesh components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @param component A handle to the component to which to add an opaque mesh
 *                  component.
 * @param mesh The initial mesh to display.
 * @return A handle to the created opaque mesh component.
 */
component_handle opaque_mesh_sub_component(
    const component_handle component,
    const mesh *const mesh);

/**
 * The meshes displayed by opaque mesh components.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to an opaque mesh component.
 */
extern const mesh *opaque_mesh_component_meshes[MAXIMUM_OPAQUE_MESH_COMPONENTS];

#endif
