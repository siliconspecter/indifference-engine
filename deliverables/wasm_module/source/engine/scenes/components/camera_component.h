/** @file */

#ifndef CAMERA_COMPONENT_H

#define CAMERA_COMPONENT_H

#include "component.h"
#include "../../primitives/f32.h"
#include "../../primitives/index.h"
#include "../../../game/project_settings/limits.h"

/**
 * Creates a new camera component as a direct child of an entity.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should there be no camera components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @remark This defaults to a sensor size of 36mm.
 * @remark This defaults to a near clip distance of 0.1.
 * @remark This defaults to a far clip distance of 1000.
 * @remark This defaults to a focal length of 50mm.
 * @remark This defaults to covering the full video buffer.
 * @remark This defaults to playing sounds at unity gain (1.0).
 * @remark This defaults to filling the video buffer.
 * @param entity The index of the entity to which to add a camera component.
 * @return A handle to the created camera component.
 */
component_handle camera_component(
    const index entity);

/**
 * Creates a new camera component as a direct child of another component.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will throw a trap should there be no camera components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @remark This defaults to a sensor size of 36mm.
 * @remark This defaults to a near clip distance of 0.1.
 * @remark This defaults to a far clip distance of 1000.
 * @remark This defaults to a focal length of 50mm.
 * @remark This defaults to covering the full video buffer.
 * @remark This defaults to playing sounds at unity gain (1.0).
 * @remark This defaults to filling the video buffer.
 * @param component A handle to the component to which to add a camera
 *                  component.
 * @return A handle to the created camera component.
 */
component_handle camera_sub_component(
    const component_handle component);

/**
 * The sensor sizes of camera components, in millimeters.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 camera_sensor_sizes[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The near clip distances of camera components.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 camera_near_clip_distances[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The far clip distances of camera components.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 camera_far_clip_distances[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The focal lengths of camera components, in millimeters.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 camera_focal_lengths[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The gain levels of sounds played by camera components, where 0.0 is silent
 * and 1.0 is unity gain.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 camera_component_gains[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' left edges, in clip space
 * (-1 = left edge of the video buffer, 1 = right edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 camera_component_viewport_lefts[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' right edges, in clip space
 * (-1 = left edge of the video buffer, 1 = right edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 camera_component_viewport_rights[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' bottom edges, in clip
 * space (-1 = bottom edge of the video buffer, 1 = top edge of the video
 * buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 camera_component_viewport_bottoms[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' top edges, in clip space
 * (-1 = bottom edge of the video buffer, 1 = top edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 camera_component_viewport_tops[MAXIMUM_CAMERA_COMPONENTS];

// TODO: a way to cancel interpolation

#endif
