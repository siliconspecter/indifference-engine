#include "../primitives/index.h"
#include "../primitives/s32.h"
#include "../primitives/f32.h"
#include "../math/relational.h"
#include "../math/float.h"
#include "../scenes/components/camera_component.h"
#include "line.h"
#include "../exports/buffers/video.h"

void render_opaque_line(
    const f32 start_red,
    const f32 start_green,
    const f32 start_blue,
    const f32 start_row,
    const f32 start_column,
    const f32 start_depth,
    const f32 end_red,
    const f32 end_green,
    const f32 end_blue,
    const f32 end_row,
    const f32 end_column,
    const f32 end_depth)
{
  const s32 rounded_start_row = floor(start_row);
  const s32 rounded_start_column = floor(start_column);

  const s32 rounded_end_row = floor(end_row);
  const s32 rounded_end_column = floor(end_column);

  const s32 row_delta = rounded_end_row - rounded_start_row;
  const s32 absolute_row_delta = absolute_f32(row_delta);
  const s32 column_delta = rounded_end_column - rounded_start_column;
  const s32 absolute_column_delta = absolute_f32(column_delta);

  s32 start_primary_axis, end_primary_axis, indices_per_primary_axis, maximum_primary_axis, indices_per_secondary_axis, maximum_secondary_axis;
  f32 starts[5], ends[5];

  starts[1] = start_depth;
  starts[2] = start_red;
  starts[3] = start_green;
  starts[4] = start_blue;
  ends[1] = end_depth;
  ends[2] = end_red;
  ends[3] = end_green;
  ends[4] = end_blue;

  if (absolute_row_delta > absolute_column_delta)
  {
    start_primary_axis = start_row;
    end_primary_axis = end_row;
    indices_per_primary_axis = camera_component_columns;
    maximum_primary_axis = camera_component_rows;
    indices_per_secondary_axis = 1;
    maximum_secondary_axis = camera_component_columns;
    starts[0] = start_column;
    ends[0] = end_column;
  }
  else
  {
    start_primary_axis = start_column;
    end_primary_axis = end_column;
    indices_per_primary_axis = 1;
    maximum_primary_axis = camera_component_columns;
    indices_per_secondary_axis = camera_component_columns;
    maximum_secondary_axis = camera_component_rows;
    starts[0] = start_row;
    ends[0] = end_row;
  }

  s32 first_primary_axis, last_primary_axis;
  f32 per_pixels[5], *accumulators, progress_per_pixel;

  subtract_f32s_f32s(ends, starts, per_pixels, 5);

  if (end_primary_axis > start_primary_axis)
  {
    first_primary_axis = start_primary_axis;
    last_primary_axis = end_primary_axis;
    accumulators = starts;
    progress_per_pixel = 1.0f / ((f32)(end_primary_axis + 1 - start_primary_axis));
  }
  else
  {
    first_primary_axis = end_primary_axis;
    last_primary_axis = start_primary_axis;
    accumulators = ends;
    progress_per_pixel = 1.0f / ((f32)(end_primary_axis - 1 - start_primary_axis));
  }

  multiply_f32s_f32(per_pixels, progress_per_pixel, per_pixels, 5);

  if (first_primary_axis < 0)
  {
    multiply_add_f32s_f32_f32s(per_pixels, -first_primary_axis, accumulators, accumulators, 4);
    first_primary_axis = 0;
  }

  last_primary_axis = MIN(last_primary_axis, maximum_primary_axis);

  for (s32 primary_axis = first_primary_axis; primary_axis <= last_primary_axis; primary_axis++)
  {
    const s32 secondary_axis = accumulators[0];

    if (secondary_axis >= 0 && secondary_axis < maximum_secondary_axis)
    {
      const index index = primary_axis * indices_per_primary_axis + secondary_axis * indices_per_secondary_axis;

      const f32 depth = accumulators[1];

      if (depth < camera_component_depths[index])
      {
        camera_component_depths[index] = depth;
        camera_component_reds[index] = accumulators[2];
        camera_component_greens[index] = accumulators[3];
        camera_component_blues[index] = accumulators[4];
        camera_component_opacities[index] = 1;
      }
    }

    add_f32s_f32s(accumulators, per_pixels, accumulators, 5);
  }
}

void render_blended_line(
    const f32 start_red,
    const f32 start_green,
    const f32 start_blue,
    const f32 start_opacity,
    const f32 start_row,
    const f32 start_column,
    const f32 start_depth,
    const f32 end_red,
    const f32 end_green,
    const f32 end_blue,
    const f32 end_opacity,
    const f32 end_row,
    const f32 end_column,
    const f32 end_depth)
{
  const s32 rounded_start_row = start_row;
  const s32 rounded_start_column = start_column;

  const s32 rounded_end_row = end_row;
  const s32 rounded_end_column = end_column;

  const s32 row_delta = rounded_end_row - rounded_start_row;
  const s32 absolute_row_delta = absolute_f32(row_delta);
  const s32 column_delta = rounded_end_column - rounded_start_column;
  const s32 absolute_column_delta = absolute_f32(column_delta);

  s32 start_primary_axis, end_primary_axis, indices_per_primary_axis, maximum_primary_axis, indices_per_secondary_axis, maximum_secondary_axis;
  f32 starts[6], ends[6];

  starts[1] = start_depth;
  starts[2] = start_opacity;
  starts[3] = start_red;
  starts[4] = start_green;
  starts[5] = start_blue;
  ends[1] = end_depth;
  ends[2] = end_opacity;
  ends[3] = end_red;
  ends[4] = end_green;
  ends[5] = end_blue;

  if (absolute_row_delta > absolute_column_delta)
  {
    start_primary_axis = start_row;
    end_primary_axis = end_row;
    indices_per_primary_axis = camera_component_columns;
    maximum_primary_axis = camera_component_rows;
    indices_per_secondary_axis = 1;
    maximum_secondary_axis = camera_component_columns;
    starts[0] = start_column;
    ends[0] = end_column;
  }
  else
  {
    start_primary_axis = start_column;
    end_primary_axis = end_column;
    indices_per_primary_axis = 1;
    maximum_primary_axis = camera_component_columns;
    indices_per_secondary_axis = camera_component_columns;
    maximum_secondary_axis = camera_component_rows;
    starts[0] = start_row;
    ends[0] = end_row;
  }

  s32 first_primary_axis, last_primary_axis;
  f32 per_pixels[6], *accumulators, progress_per_pixel;

  subtract_f32s_f32s(ends, starts, per_pixels, 6);

  if (end_primary_axis > start_primary_axis)
  {
    first_primary_axis = start_primary_axis;
    last_primary_axis = end_primary_axis;
    accumulators = starts;
    progress_per_pixel = 1.0f / ((f32)(end_primary_axis + 1 - start_primary_axis));
  }
  else
  {
    first_primary_axis = end_primary_axis;
    last_primary_axis = start_primary_axis;
    accumulators = ends;
    progress_per_pixel = 1.0f / ((f32)(end_primary_axis - 1 - start_primary_axis));
  }

  multiply_f32s_f32(per_pixels, progress_per_pixel, per_pixels, 6);

  if (first_primary_axis < 0)
  {
    multiply_add_f32s_f32_f32s(per_pixels, -first_primary_axis, accumulators, accumulators, 4);
    first_primary_axis = 0;
  }

  last_primary_axis = MIN(last_primary_axis, maximum_primary_axis);

  for (s32 primary_axis = first_primary_axis; primary_axis <= last_primary_axis; primary_axis++)
  {
    const s32 secondary_axis = accumulators[0];

    if (secondary_axis >= 0 && secondary_axis < maximum_secondary_axis)
    {
      const index index = primary_axis * indices_per_primary_axis + secondary_axis * indices_per_secondary_axis;

      if (accumulators[1] < camera_component_depths[index])
      {
        const f32 source_opacity = accumulators[2];
        const f32 destination_opacity = camera_component_opacities[index] * (1.0f - source_opacity);

        camera_component_opacities[index] = source_opacity + destination_opacity;
        camera_component_reds[index] = linearly_interpolate_f32_f32_f32(camera_component_reds[index], accumulators[3], source_opacity, destination_opacity);
        camera_component_greens[index] = linearly_interpolate_f32_f32_f32(camera_component_greens[index], accumulators[4], source_opacity, destination_opacity);
        camera_component_blues[index] = linearly_interpolate_f32_f32_f32(camera_component_blues[index], accumulators[5], source_opacity, destination_opacity);
      }
    }

    add_f32s_f32s(accumulators, per_pixels, accumulators, 6);
  }
}
