#include "../export.h"
#include "event_handler.h"
#include "../buffers/video.h"
#include "../../scenes/entity.h"
#include "../../scenes/components/camera_component.h"
#include "../../primitives/f32.h"

static void render()
{
  copy_f32(1.0f, camera_component_depths, camera_component_rows * camera_component_columns);
}

EXPORT void video()
{
  initialize_event_handler();
  inverse_tick_progress = 1.0f - tick_progress;

  copy_f32(0.0f, camera_component_opacities, video_rows * video_columns);

  prepare_entities_for_video();
  render_camera_components(render);
}
