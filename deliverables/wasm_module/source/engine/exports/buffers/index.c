#include "../export.h"
#include "../../primitives/s32.h"
#include "../../primitives/index.h"
#include "../../../game/project_settings/timing_settings.h"
#include "../../../game/project_settings/video_settings.h"
#include "../../../game/project_settings/audio_settings.h"
#include "audio.h"
#include "video.h"
#include "persist.h"
#include "error.h"
#include "pointer.h"
#include "controllers.h"

const s32 ticks_per_second = TICKS_PER_SECOND;
const s32 samples_per_tick = SAMPLES_PER_TICK;
const s32 maximum_video_rows = MAXIMUM_VIDEO_ROWS;
const s32 safe_area_rows = SAFE_AREA_ROWS;
const s32 safe_area_columns = SAFE_AREA_COLUMNS;

#define BUFFER_LIST(item)                                                                                     \
  item(1073741824, ticks_per_second)                                                                          \
      item(1073741825, samples_per_tick)                                                                      \
          item(1073741826, audio_buffer)                                                                      \
              item(1073741827, maximum_video_rows)                                                            \
                  item(1073741828, safe_area_rows)                                                            \
                      item(1073741839, safe_area_columns)                                                     \
                          item(1073741830, video_buffer)                                                      \
                              item(1073741831, pointer_state)                                                 \
                                  item(1073741832, pointer_row)                                               \
                                      item(1073741833, pointer_column)                                        \
                                          item(1073741834, persist)                                           \
                                              item(1073741835, tick_progress)                                 \
                                                  item(1073741836, video_rows)                                \
                                                      item(1073741837, video_columns)                         \
                                                          item(1073741838, listener_location)                 \
                                                              item(1073741839, listener_normal)               \
                                                                  item(1073741840, controller_states)         \
                                                                      item(1073741841, controller_y_axes)     \
                                                                          item(1073741842, controller_x_axes) \
                                                                              item(1073741843, error)

// TODO: macro for NULL
#define BUFFER_NULL(identifier, data) (void *)0,

void *buffer_pointer_values[] = {BUFFER_LIST(BUFFER_NULL)};
s32 buffer_size_values[sizeof(buffer_pointer_values) / sizeof(buffer_pointer_values[0])];
s32 buffer_identifier_values[sizeof(buffer_pointer_values) / sizeof(buffer_pointer_values[0])];

EXPORT s32 buffer_count()
{
  return sizeof(buffer_pointer_values) / sizeof(buffer_pointer_values[0]);
}

EXPORT void **buffer_pointers()
{
  index buffer = 0;
#define BUFFER_POINTER(identifier, data) buffer_pointer_values[buffer++] = (void *)&data;
  BUFFER_LIST(BUFFER_POINTER)
  return buffer_pointer_values;
}

EXPORT s32 *buffer_sizes()
{
  index buffer = 0;
#define BUFFER_SIZE(identifier, data) buffer_size_values[buffer++] = sizeof(data);
  BUFFER_LIST(BUFFER_SIZE)
  return buffer_size_values;
}

EXPORT s32 *buffer_identifiers()
{
  index buffer = 0;
#define BUFFER_IDENTIFIER(identifier, data) buffer_identifier_values[buffer++] = identifier;
  BUFFER_LIST(BUFFER_IDENTIFIER)
  return buffer_identifier_values;
}
