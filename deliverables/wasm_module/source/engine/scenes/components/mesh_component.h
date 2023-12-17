/** @file */

#ifndef MESH_COMPONENT_H

#define MESH_COMPONENT_H

#include "component.h"
#include "../../primitives/index.h"
#include "../../assets/mesh.h"

/**
 * Creates a new mesh component as a direct child of an entity.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should there be no mesh components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @param entity The index of the entity to which to add a mesh component.
 * @param mesh The initial mesh to display.
 * @return A handle to the created mesh component.
 */
component_handle mesh_component(
    const index entity,
    const mesh *const mesh);

/**
 * Creates a new mesh component as a direct child of another component.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should there be no mesh components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @param component A handle to the component to which to add an mesh component.
 * @param mesh The initial mesh to display.
 * @return A handle to the created mesh component.
 */
component_handle mesh_sub_component(
    const component_handle component,
    const mesh *const mesh);

#ifndef DOXYGEN_IGNORE

/**
 * Called by the video event handler once per camera to render all opaque and
 * cutout mesh components.
 */
void render_opaque_cutout_mesh_components();

/**
 * Called by the video event handler once per camera to render all additive and
 * blended mesh components.
 */
void render_additive_blended_mesh_components();

#endif

#endif
