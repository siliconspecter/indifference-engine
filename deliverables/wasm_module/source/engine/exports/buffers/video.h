/** @file */

#ifndef VIDEO_H

#define VIDEO_H

#include "../../primitives/s32.h"
#include "../../primitives/f32.h"
#include "../../video/color.h"
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
 * The video output buffer, running from left to right, then top to bottom.
 */
extern color video_colors[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS];

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
