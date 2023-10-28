#include "video.h"
#include "../../primitives/s32.h"
#include "../../primitives/f32.h"
#include "../../video/color.h"
#include "../../../game/project_settings/video_settings.h"

s32 video_rows = MAXIMUM_VIDEO_ROWS;

s32 video_columns = MAXIMUM_VIDEO_COLUMNS;

color video_colors[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS];

f32 tick_progress = 1;
