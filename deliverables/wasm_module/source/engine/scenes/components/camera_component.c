#include "../entity.h"
#include "component.h"
#include "../../primitives/f32.h"
#include "../../primitives/quantity.h"
#include "../../primitives/index.h"
#include "../../../game/project_settings/limits.h"
#include "camera_component.h"
#include "../../miscellaneous.h"
#include "../../exports/buffers/error.h"
#include "../../exports/buffers/video.h"
#include "../../math/relational.h"
#include "../../math/matrix.h"

f32 previous_camera_component_sensor_sizes[MAXIMUM_CAMERA_COMPONENTS];
f32 next_camera_component_sensor_sizes[MAXIMUM_CAMERA_COMPONENTS];
f32 previous_camera_component_near_clip_distances[MAXIMUM_CAMERA_COMPONENTS];
f32 next_camera_component_near_clip_distances[MAXIMUM_CAMERA_COMPONENTS];
f32 previous_camera_component_far_clip_distances[MAXIMUM_CAMERA_COMPONENTS];
f32 next_camera_component_far_clip_distances[MAXIMUM_CAMERA_COMPONENTS];
f32 previous_camera_component_focal_lengths[MAXIMUM_CAMERA_COMPONENTS];
f32 next_camera_component_focal_lengths[MAXIMUM_CAMERA_COMPONENTS];
f32 previous_camera_component_gains[MAXIMUM_CAMERA_COMPONENTS];
f32 next_camera_component_gains[MAXIMUM_CAMERA_COMPONENTS];
f32 previous_camera_component_tops[MAXIMUM_CAMERA_COMPONENTS];
f32 next_camera_component_tops[MAXIMUM_CAMERA_COMPONENTS];
f32 previous_camera_component_bottoms[MAXIMUM_CAMERA_COMPONENTS];
f32 next_camera_component_bottoms[MAXIMUM_CAMERA_COMPONENTS];
f32 previous_camera_component_lefts[MAXIMUM_CAMERA_COMPONENTS];
f32 next_camera_component_lefts[MAXIMUM_CAMERA_COMPONENTS];
f32 previous_camera_component_rights[MAXIMUM_CAMERA_COMPONENTS];
f32 next_camera_component_rights[MAXIMUM_CAMERA_COMPONENTS];

static index first_occupied;
static index last_occupied;
static quantity total_occupied;
static const matrix *transforms[MAXIMUM_CAMERA_COMPONENTS];
static const matrix *inverse_transforms[MAXIMUM_CAMERA_COMPONENTS];

matrix camera_component_view_projection;
matrix camera_component_inverse_view_projection;
f32 *camera_component_reds;
f32 *camera_component_greens;
f32 *camera_component_blues;
f32 *camera_component_opacities;
f32 *camera_component_depths;
quantity camera_component_rows;
quantity camera_component_columns;
f32 camera_component_clip_to_video_row_coefficient;
f32 camera_component_clip_to_video_row_offset;
f32 camera_component_clip_to_video_column_coefficient;
f32 camera_component_clip_to_video_column_offset;
f32 camera_component_gain;

static index allocate(index entity)
{
  FIND_EMPTY_INDEX(transforms, NULL, MAXIMUM_CAMERA_COMPONENTS, first_occupied, last_occupied, total_occupied, ERROR_NO_CAMERA_COMPONENTS_TO_ALLOCATE, camera)

  transforms[camera] = &interpolated_entity_transforms[entity];
  inverse_transforms[camera] = &interpolated_inverse_entity_transforms[entity];
  previous_camera_component_sensor_sizes[camera] = 36;
  next_camera_component_sensor_sizes[camera] = 36;
  previous_camera_component_near_clip_distances[camera] = 0.1;
  next_camera_component_near_clip_distances[camera] = 0.1;
  previous_camera_component_far_clip_distances[camera] = 1000;
  next_camera_component_far_clip_distances[camera] = 1000;
  previous_camera_component_focal_lengths[camera] = 50;
  next_camera_component_focal_lengths[camera] = 50;
  previous_camera_component_lefts[camera] = -1;
  next_camera_component_lefts[camera] = -1;
  previous_camera_component_rights[camera] = 1;
  next_camera_component_rights[camera] = 1;
  previous_camera_component_bottoms[camera] = -1;
  next_camera_component_bottoms[camera] = -1;
  previous_camera_component_tops[camera] = 1;
  next_camera_component_tops[camera] = 1;

  return camera;
}

static void destroy(const component_handle component)
{
  const index camera = COMPONENT_HANDLE_META(component);
  transforms[camera] = NULL;
}

component_handle camera_component(
    const index entity)
{
  const index camera = allocate(entity);
  return component(entity, camera, destroy);
}

component_handle camera_sub_component(
    const component_handle component)
{
  const index camera = allocate(parent_entity_of(component));
  return sub_component(component, camera, destroy);
}

void render_camera_components(render_camera_component *const on_render)
{
  if (first_occupied != INDEX_NONE)
  {
    const f32 clip_to_video_row_offset = ((f32)video_rows) / 2.0f;
    const f32 clip_to_video_row_coefficient = -clip_to_video_row_offset;

    const f32 clip_to_video_column_offset = ((f32)video_columns) / 2.0f;
    const f32 clip_to_video_column_coefficient = clip_to_video_column_offset;

    for (index camera = first_occupied; camera <= last_occupied; camera++)
    {
      const matrix *const transform = transforms[camera];

      if (transform != NULL)
      {
        const f32 top = tick_progress * next_camera_component_tops[camera] + inverse_tick_progress * previous_camera_component_tops[camera];
        const quantity top_rows = top * clip_to_video_row_coefficient + clip_to_video_row_offset;
        const quantity top_rows_clamped = CLAMP(top_rows, 0, video_rows);

        const f32 bottom = tick_progress * next_camera_component_bottoms[camera] + inverse_tick_progress * previous_camera_component_bottoms[camera];
        const quantity bottom_rows = bottom * clip_to_video_row_coefficient + clip_to_video_row_offset;
        const quantity bottom_rows_clamped = CLAMP(bottom_rows, 0, video_rows);

        const f32 left = tick_progress * next_camera_component_lefts[camera] + inverse_tick_progress * previous_camera_component_lefts[camera];
        const quantity left_columns = left * clip_to_video_column_coefficient + clip_to_video_column_offset;
        const quantity left_columns_clamped = CLAMP(left_columns, 0, video_columns);

        const f32 right = tick_progress * next_camera_component_rights[camera] + inverse_tick_progress * previous_camera_component_rights[camera];
        const quantity right_columns = right * clip_to_video_column_coefficient + clip_to_video_column_offset;
        const quantity right_columns_clamped = CLAMP(right_columns, 0, video_columns);

        if (left_columns_clamped < right_columns_clamped && top_rows_clamped < bottom_rows_clamped)
        {
          const index offset = top_rows_clamped * video_columns + left_columns_clamped;
          camera_component_reds = &video_reds[offset];
          camera_component_greens = &video_greens[offset];
          camera_component_blues = &video_blues[offset];
          camera_component_opacities = &video_opacities[offset];
          camera_component_depths = &video_depths[offset];
          camera_component_rows = bottom_rows - top_rows;
          camera_component_columns = right_columns - left_columns;

          const f32 sensor_size = tick_progress * next_camera_component_sensor_sizes[camera] + inverse_tick_progress * previous_camera_component_sensor_sizes[camera];
          const f32 near_clip_distance = tick_progress * next_camera_component_near_clip_distances[camera] + inverse_tick_progress * previous_camera_component_near_clip_distances[camera];
          const f32 far_clip_distance = tick_progress * next_camera_component_far_clip_distances[camera] + inverse_tick_progress * previous_camera_component_far_clip_distances[camera];
          const f32 focal_length = tick_progress * next_camera_component_focal_lengths[camera] + inverse_tick_progress * previous_camera_component_focal_lengths[camera];
          camera_component_gain = tick_progress * next_camera_component_gains[camera] + inverse_tick_progress * previous_camera_component_gains[camera];

          camera_component_clip_to_video_row_coefficient = ((f32)top_rows - (f32)bottom_rows) * 0.5f;
          camera_component_clip_to_video_row_offset = -camera_component_clip_to_video_row_coefficient;
          camera_component_clip_to_video_column_coefficient = ((f32)right_columns - (f32)left_columns) * 0.5f;
          camera_component_clip_to_video_column_offset = camera_component_clip_to_video_column_coefficient;

          matrix projection, inverse_projection;

          perspective(
              right_columns - left_columns,
              bottom_rows - top_rows,
              focal_length,
              0,
              0,
              near_clip_distance,
              far_clip_distance,
              sensor_size,
              projection,
              inverse_projection);

          multiply_matrices(projection, *inverse_transforms[camera], camera_component_view_projection);

          // TODO: check ordering
          multiply_matrices(inverse_projection, *transform, camera_component_inverse_view_projection);

          on_render();
        }
      }
    }
  }
}
