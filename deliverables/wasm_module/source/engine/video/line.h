#ifndef LINE_H

#define LINE_H

#include "../primitives/index.h"
#include "../primitives/f32.h"
#include "../math/relational.h"

/**
 * Draws an opaque line in the current camera component's viewport.
 * @remark Only use when rendering video for a camera component.
 * @param start_red The intensity of the red channel at the start of the line,
 *                  in candela per square meter.
 * @param start_green The intensity of the green channel at the start of the
 *                    line, in candela per square meter.
 * @param start_blue The intensity of the blue channel at the start of the line,
 *                   in candela per square meter.
 * @param start_row The number of rows between the top of the current camera
 *                  component's viewport and the start of the line.
 * @param start_column The number of columns between the left of the current
 *                     camera component's viewport and the start of the line.
 * @param start_depth The depth of the start of the line, where -1 is near and
 *                    1 is far, non-linear.
 * @param end_red The intensity of the red channel at the end of the line, in
 *                candela per square meter.
 * @param end_green The intensity of the green channel at the end of the line,
 *                  in candela per square meter.
 * @param end_blue The intensity of the blue channel at the start of the line,
 *                 in candela per square meter.
 * @param end_row The number of rows between the top of the current camera
 *                component's viewport and the end of the line.
 * @param end_column The number of columns between the left of the current
 *                   camera component's viewport and the end of the line.
 * @param end_depth The depth of the end of the line, where -1 is near and 1 is
 *                  far, non-linear.
 */
void render_opaque_line(
    const f32 start_red,
    const f32 start_green,
    const f32 start_blue,
    const f32 start_row,
    const f32 start_column,
    const f32 start_depth,
    const f32 end_red,
    const f32 end_green,
    const f32 end_blue,
    const f32 end_row,
    const f32 end_column,
    const f32 end_depth);

/**
 * Draws a transparent line in in the current camera component's viewport.
 * @remark Only use when rendering video for a camera component.
 * @remark Does not write to the depth buffer.
 * @param start_red The intensity of the red channel at the start of the line,
 *                  in candela per square meter.
 * @param start_green The intensity of the green channel at the start of the
 *                    line, in candela per square meter.
 * @param start_blue The intensity of the blue channel at the start of the line,
 *                   in candela per square meter.
 * @param start_opacity The opacity of the end of the line, where 0 is
 *                      transparent and 1 is opaque.
 * @param start_row The number of rows between the top of the current camera
 *                  component's viewport and the start of the line.
 * @param start_column The number of columns between the left of the current
 *                     camera component's viewport and the start of the line.
 * @param start_depth The depth of the start of the line, where -1 is near and
 *                    1 is far, non-linear.
 * @param end_red The intensity of the red channel at the end of the line, in
 *                candela per square meter.
 * @param end_green The intensity of the green channel at the end of the line,
 *                  in candela per square meter.
 * @param end_blue The intensity of the blue channel at the start of the line,
 *                 in candela per square meter.
 * @param end_opacity The opacity of the start of the line, where 0 is
 *                    transparent and 1 is opaque.
 * @param end_row The number of rows between the top of the current camera
 *                component's viewport and the end of the line.
 * @param end_column The number of columns between the left of the current
 *                   camera component's viewport and the end of the line.
 * @param end_depth The depth of the end of the line, where -1 is near and 1 is
 *                  far, non-linear.
 */
void render_transparent_line(
    const f32 start_red,
    const f32 start_green,
    const f32 start_blue,
    const f32 start_opacity,
    const f32 start_row,
    const f32 start_column,
    const f32 start_depth,
    const f32 end_red,
    const f32 end_green,
    const f32 end_blue,
    const f32 end_opacity,
    const f32 end_row,
    const f32 end_column,
    const f32 end_depth);

#endif
