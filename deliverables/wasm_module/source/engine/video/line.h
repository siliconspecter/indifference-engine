#include "../primitives/index.h"
#include "../primitives/s32.h"
#include "../primitives/f32.h"
#include "color.h"
#include "../math/relational.h"

/**
 * Draws an opaque line in the current camera component's viewport.
 * @remark Only use when rendering video for a camera component.
 * @param start_color The color of the start of the line.  Opacity is ignored.
 * @param start_row The number of rows between the top of the current camera
 *                  component's viewport and the start of the line.
 * @param start_column The number of columns between the left of the current
 *                     camera component's viewport and the start of the line.
 * @param start_depth The depth of the start of the line, where -1 is near and
 *                    1 is far, non-linear.
 * @param end_color The color of the end of the line.  Opacity is ignored.
 * @param end_row The number of rows between the top of the current camera
 *                component's viewport and the end of the line.
 * @param end_column The number of columns between the left of the current
 *                   camera component's viewport and the end of the line.
 * @param end_depth The depth of the end of the line, where -1 is near and 1 is
 *                  far, non-linear.
 */
void render_opaque_line(
    const color start_color,
    const s32 start_row,
    const s32 start_column,
    const f32 start_depth,
    const color end_color,
    const s32 end_row,
    const s32 end_column,
    const f32 end_depth);

/**
 * Draws a transparent line in in the current camera component's
 * viewport.
 * @remark Only use when rendering video for a camera component.
 * @remark Does not write to the depth buffer.
 * @param start_color The color of the start of the line.
 * @param start_row The number of rows between the top of the current camera
 *                  component's viewport and the start of the line.
 * @param start_column The number of columns between the left of the current
 *                     camera component's viewport and the start of the line.
 * @param start_depth The depth of the start of the line, where -1 is near and
 *                    1 is far, non-linear.
 * @param end_color The color of the end of the line.
 * @param end_row The number of rows between the top of the current camera
 *                component's viewport and the end of the line.
 * @param end_column The number of columns between the left of the current
 *                   camera component's viewport and the end of the line.
 * @param end_depth The depth of the end of the line, where -1 is near and 1 is
 *                  far, non-linear.
 */
void render_transparent_line(
    const color start_color,
    const s32 start_row,
    const s32 start_column,
    const f32 start_depth,
    const color end_color,
    const s32 end_row,
    const s32 end_column,
    const f32 end_depth);
