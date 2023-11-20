#ifndef TRIANGLE_H

#define TRIANGLE_H

#include "../primitives/s32.h"
#include "../primitives/f32.h"

/**
 * Draws an opaque line in the current camera component's viewport.
 * @remark Only use when rendering video for a camera component.
 * @param texture_rows The height of the texture in pixel rows.
 * @param texture_rows_minus_one The height of the texture in pixel rows, minus
 *                               one.
 * @param texture_columns The width of the texture in pixel columns.
 * @param texture_columns_minus_one The width of the texture in pixel columns,
 *                                  minus one.
 * @param texture_reds The intensity of the red channel of each pixel within the
 *                     texture, row-major.  0 is dark and 1 is bright.  Values
 *                     may exceed this range.
 * @param texture_greens The intensity of the green channel of each pixel within
 *                       the texture, row-major.  0 is dark and 1 is bright.
 *                       Values may exceed this range.
 * @param texture_blues The intensity of the blue channel of each pixel within
 *                      the texture, row-major.  0 is dark and 1 is bright.
 *                      Values may exceed this range.
 * @param texture_opacities The opacity of each pixel within the texture,
 *                          row-major.  0 is transparent and 1 is opaque.
 *                          Behavior is undefined outside of this range.
 * @param a_camera_row The number of rows between the top of the current camera
 *                     component's viewport and the triangle's first vertex.
 * @param a_camera_column The number of columns between the left of the current
 *                        camera component's viewport and the triangle's first
 *                        vertex.
 * @param a_depth The depth of the triangle's first vertex, where -1 is near and
 *                1 is far, non-linear.
 * @param a_texture_row The V coordinate of the triangle's first vertex, where 0
 *                      is the top and 1 is the bottom.
 * @param a_texture_column The U coordinate of the triangle's first vertex,
 *                         where 0 is the left and 1 is the right.
 * @param a_opacity The opacity of the triangle's first vertex, where 0 is
 *                  transparent and 1 is opaque.  Behavior is undefined outside
 *                  of this range.
 * @param a_red The intensity of the red channel of the triangle's first vertex,
 *              in candela per square meter.
 * @param a_green The intensity of the green channel of the triangle's first
 *                vertex, in candela per square meter.
 * @param a_blue The intensity of the blue channel of the triangle's first
 *               vertex, in candela per square meter.
 * @param b_camera_row The number of rows between the top of the current camera
 *                     component's viewport and the triangle's second vertex.
 * @param b_camera_column The number of columns between the left of the current
 *                        camera component's viewport and the triangle's second
 *                        vertex.
 * @param b_depth The depth of the triangle's second vertex, where -1 is near
 *                and 1 is far, non-linear.
 * @param b_texture_row The V coordinate of the triangle's second vertex, where
 *                      0 is the top and 1 is the bottom.
 * @param b_texture_column The U coordinate of the triangle's second vertex,
 *                         where 0 is the left and 1 is the right.
 * @param b_opacity The opacity of the triangle's second vertex, where 0 is
 *                  transparent and 1 is opaque.  Behavior is undefined outside
 *                  of this range.
 * @param b_red The intensity of the red channel of the triangle's second
 *              vertex, in candela per square meter.
 * @param b_green The intensity of the green channel of the triangle's second
 *                vertex, in candela per square meter.
 * @param b_blue The intensity of the blue channel of the triangle's second
 *               vertex, in candela per square meter.
 * @param c_camera_row The number of rows between the top of the current camera
 *                     component's viewport and the triangle's third vertex.
 * @param c_camera_column The number of columns between the left of the current
 *                        camera component's viewport and the triangle's third
 *                        vertex.
 * @param c_depth The depth of the triangle's third vertex, where -1 is near and
 *                1 is far, non-linear.
 * @param c_texture_row The V coordinate of the triangle's third vertex, where 0
 *                      is the top and 1 is the bottom.
 * @param c_texture_column The U coordinate of the triangle's third vertex,
 *                         where 0 is the left and 1 is the right.
 * @param c_opacity The opacity of the triangle's third vertex, where 0 is
 *                  transparent and 1 is opaque.  Behavior is undefined outside
 *                  of this range.
 * @param c_red The intensity of the red channel of the triangle's third vertex,
 *              in candela per square meter.
 * @param c_green The intensity of the green channel of the triangle's third
 *                vertex, in candela per square meter.
 * @param c_blue The intensity of the blue channel of the triangle's third
 *               vertex, in candela per square meter.
 */
void render_opaque_triangle(
    const s32 texture_rows,
    const s32 texture_rows_minus_one,
    const s32 texture_columns,
    const s32 texture_columns_minus_one,
    const f32 *const texture_reds,
    const f32 *const texture_greens,
    const f32 *const texture_blues,
    const f32 *const texture_opacities,
    const f32 a_camera_row,
    const f32 a_camera_column,
    const f32 a_depth,
    const f32 a_texture_row,
    const f32 a_texture_column,
    const f32 a_opacity,
    const f32 a_red,
    const f32 a_green,
    const f32 a_blue,
    const f32 b_camera_row,
    const f32 b_camera_column,
    const f32 b_depth,
    const f32 b_texture_row,
    const f32 b_texture_column,
    const f32 b_opacity,
    const f32 b_red,
    const f32 b_green,
    const f32 b_blue,
    const f32 c_camera_row,
    const f32 c_camera_column,
    const f32 c_depth,
    const f32 c_texture_row,
    const f32 c_texture_column,
    const f32 c_opacity,
    const f32 c_red,
    const f32 c_green,
    const f32 c_blue);

#endif
