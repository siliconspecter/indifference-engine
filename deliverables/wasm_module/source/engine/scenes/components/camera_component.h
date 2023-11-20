/** @file */

#ifndef CAMERA_COMPONENT_H

#define CAMERA_COMPONENT_H

#include "component.h"
#include "../../primitives/quantity.h"
#include "../../primitives/f32.h"
#include "../../primitives/index.h"
#include "../../math/matrix.h"
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
 * The sensor sizes of camera components at the start of the tick, in
 * millimeters.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 previous_camera_component_sensor_sizes[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The sensor sizes of camera components at the end of the tick, in
 * millimeters.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 next_camera_component_sensor_sizes[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The near clip distances of camera components at the start of the tick.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 previous_camera_component_near_clip_distances[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The near clip distances of camera components at the end of the tick.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 next_camera_component_near_clip_distances[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The far clip distances of camera components at the start of the tick.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 previous_camera_component_far_clip_distances[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The far clip distances of camera components at the end of the tick.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 next_camera_component_far_clip_distances[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The focal lengths of camera components at the start of the tick, in
 * millimeters.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 previous_camera_component_focal_lengths[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The focal lengths of camera components at the end of the tick, in
 * millimeters.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 next_camera_component_focal_lengths[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The gain levels of sounds played by camera components at the start of the
 * tick, where 0.0 is silent and 1.0 is unity gain.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 previous_camera_component_gains[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The gain levels of sounds played by camera components at the end of the
 * tick, where 0.0 is silent and 1.0 is unity gain.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 next_camera_component_gains[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' top edges at the start of
 * the tick, in normalized video buffer space (-1 = bottom edge of the video
 * buffer, 1 = top edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 previous_camera_component_tops[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' top edges at the end of
 * the tick, in normalized video buffer space (-1 = bottom edge of the video
 * buffer, 1 = top edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 next_camera_component_tops[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' bottom edges at the start
 * of the tick, in normalized video buffer space (-1 = bottom edge of the video
 * buffer, 1 = top edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 previous_camera_component_bottoms[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' bottom edges at the end of
 * the tick, in normalized video buffer space (-1 = bottom edge of the video
 * buffer, 1 = top edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 next_camera_component_bottoms[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' left edges at the start of
 * the tick, in normalized video buffer space (-1 = left edge of the video
 * buffer, 1 = right edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 previous_camera_component_lefts[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' left edges at the end of
 * the tick, in normalized video buffer space (-1 = left edge of the video
 * buffer, 1 = right edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 next_camera_component_lefts[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' right edges at the start
 * of the tick, in normalized video buffer space (-1 = left edge of the video
 * buffer, 1 = right edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 previous_camera_component_rights[MAXIMUM_CAMERA_COMPONENTS];

/**
 * The locations of the camera components' viewports' right edges at the end of
 * the tick, in normalized video buffer space (-1 = left edge of the video
 * buffer, 1 = right edge of the video buffer).
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a camera component.
 */
extern f32 next_camera_component_rights[MAXIMUM_CAMERA_COMPONENTS];

#ifndef DOXYGEN_IGNORE

/**
 * A matrix which transforms from world space into the current camera
 * component's clip space.
 * @remark Content is undefined except when a specific camera component.  Do NOT
 *         re-assign.
 */
extern matrix camera_component_view_projection;

/**
 * A matrix which transforms from world space into the current camera
 * component's clip space.
 * @remark Content is undefined except when a specific camera component.  Do NOT
 *         re-assign.
 */
extern matrix camera_component_inverse_view_projection;

/**
 * The start of the red values in the video output buffer held by the current
 * camera component's viewport.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 *camera_component_reds;

/**
 * The start of the green values in the video output buffer held by the current
 * camera component's viewport.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 *camera_component_greens;

/**
 * The start of the blue values in the video output buffer held by the current
 * camera component's viewport.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 *camera_component_blues;

/**
 * The start of the opacity values in the video output buffer held by the
 * current camera component's viewport.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 *camera_component_opacities;

/**
 * The start of the depth data held by the current camera component's viewport.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 *camera_component_depths;

/**
 * The height of the current camera component's viewport, in pixel rows.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern quantity camera_component_rows;

/**
 * The width of the current camera component's viewport, in pixel columns.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern quantity camera_component_columns;

/**
 * The coefficient to multiply by when transforming from the current camera
 * component's clip space to video rows.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 camera_component_clip_to_video_row_coefficient;

/**
 * The offset to add when transforming from the current camera component's clip
 * space to video rows.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 camera_component_clip_to_video_row_offset;

/**
 * The coefficient to multiply by when transforming from the current camera
 * component's clip space to video columns.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 camera_component_clip_to_video_column_coefficient;

/**
 * The offset to add when transforming from the current camera component's clip
 * space to video columns.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 camera_component_clip_to_video_column_offset;

/**
 * The gain levels of sounds played by the current camera component, where 0.0
 * is silent and 1.0 is unity gain.
 * @remark Content is undefined except when rendering a specific camera
 *         component.  Do NOT re-assign.
 */
extern f32 camera_component_gain;

/**
 * A callback which is called for each rendered camera component during a
 * render.
 * @remark The fields representing the parameters of the current camera
 *         component are accessible until this returns.
 */
typedef void(render_camera_component)();

/**
 * Called by the video event handler to render all camera components.
 * @param on_render Called once per rendered camera component.
 */
void render_camera_components(render_camera_component *const on_render);

#endif

#endif
