#include "video.h"
#include "../../primitives/s32.h"
#include "../../primitives/f32.h"
#include "../../video/color.h"
#include "../../../game/project_settings/video_settings.h"

s32 video_rows;

s32 video_columns;

color video_buffer[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS];

f32 tick_progress;
