/** @file */

#ifndef VIDEO_H

#define VIDEO_H

#include "../../primitives/s32.h"
#include "../../primitives/f32.h"
#include "../../../game/project_settings/video_settings.h"

/**
 * The height of the video buffer, in pixel rows.
 */
extern s32 video_rows;

/**
 * The width of the video buffer, in pixel columns.
 */
extern s32 video_columns;

/**
 * The intensity of the red channel of each pixel within the video output
 * buffer in candela per square meter, row-major.
 */
extern f32 video_reds[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS];

/**
 * The intensity of the green channel of each pixel within the video output
 * buffer in candela per square meter, row-major.
 */
extern f32 video_greens[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS];

/**
 * The intensity of the blue channel of each pixel within the video output
 * buffer in candela per square meter, row-major.
 */
extern f32 video_blues[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS];

/**
 * The opacity of each pixel within the video output buffer, row-major.
 * @remark 0 is transparent and 1 is opaque.  Behavior is undefined outside of
 *         this range.  It is unlikely that the hosting runtime will use this
 *         buffer.
 */
extern f32 video_opacities[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS];

/**
 * The depth buffer, row-major, where -1 is near and 1 is far, non-linear.
 * @remark It is unlikely that the hosting runtime will use this buffer.
 */
extern f32 video_depths[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS];

/**
 * The progress through the current tick, where 0 is the start and 1 is the end.
 */
extern f32 tick_progress;

/**
 * The inverse progress through the current tick, where 1 is the start and 0 is
 * the end.
 */
extern f32 inverse_tick_progress;

#endif
