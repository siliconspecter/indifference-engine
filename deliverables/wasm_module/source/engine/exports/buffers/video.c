#include "video.h"
#include "../../primitives/s32.h"
#include "../../primitives/f32.h"
#include "../../video/color.h"
#include "../../../game/project_settings/video_settings.h"
#include "../export.h"

s32 video_rows ALIGN(s32) = MAXIMUM_VIDEO_ROWS;

s32 video_columns ALIGN(s32) = MAXIMUM_VIDEO_COLUMNS;

color video_colors[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS] ALIGN(s32);

f32 tick_progress ALIGN(f32) = 1;
f32 inverse_tick_progress;
