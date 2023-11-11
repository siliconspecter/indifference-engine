#include "../export.h"
#include "event_handler.h"
#include "../buffers/video.h"
#include "../../scenes/entity.h"
#include "../../scenes/components/camera_component.h"

#include "../../video/color.h"
#include "../../video/line.h"
#include "../buffers/error.h"
#include "../../math/matrix.h"

EXPORT void video()
{
  initialize_event_handler();
  inverse_tick_progress = 1.0f - tick_progress;
  prepare_entities_for_video();
}
