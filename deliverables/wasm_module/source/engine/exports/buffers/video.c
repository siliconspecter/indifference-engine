#include "video.h"
#include "../../primitives/s32.h"
#include "../../primitives/f32.h"
#include "../../primitives/quantity.h"
#include "../../../game/project_settings/video_settings.h"
#include "../export.h"

quantity video_rows ALIGN(s32) = MAXIMUM_VIDEO_ROWS;

quantity video_columns ALIGN(s32) = MAXIMUM_VIDEO_COLUMNS;

f32 video_reds[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS] ALIGN(f32);
f32 video_greens[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS] ALIGN(f32);
f32 video_blues[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS] ALIGN(f32);
f32 video_opacities[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS] ALIGN(f32);
f32 video_depths[MAXIMUM_VIDEO_ROWS * MAXIMUM_VIDEO_COLUMNS] ALIGN(f32);

f32 tick_progress ALIGN(f32) = 1;
f32 inverse_tick_progress;
