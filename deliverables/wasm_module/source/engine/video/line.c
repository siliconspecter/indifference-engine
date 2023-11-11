#include "../primitives/index.h"
#include "../primitives/s32.h"
#include "../primitives/f32.h"
#include "color.h"
#include "../math/relational.h"
#include "../scenes/components/camera_component.h"
#include "line.h"
#include "../exports/buffers/video.h"

void render_opaque_line(
    const color start_color,
    const s32 start_row,
    const s32 start_column,
    const f32 start_depth,
    const color end_color,
    const s32 end_row,
    const s32 end_column,
    const f32 end_depth)
{
  s32 top_row, bottom_row;

  if (start_row > end_row)
  {
    top_row = end_row;
    bottom_row = start_row;
  }
  else
  {
    top_row = start_row;
    bottom_row = end_row;
  }

  const s32 absolute_row_delta = bottom_row - top_row;

  s32 left_column, right_column;

  if (start_column > end_column)
  {
    left_column = end_column;
    right_column = start_column;
  }
  else
  {
    left_column = start_column;
    right_column = end_column;
  }

  const s32 absolute_column_delta = right_column - left_column;

  if (!absolute_column_delta && !absolute_row_delta)
  {
    const index destination_index = start_row * camera_component_columns + start_column;
    if (start_depth < camera_component_depths[destination_index])
    {
      camera_component_colors[destination_index] = LAYER_COLORS(start_color, camera_component_colors[destination_index]);
    }
  }
  else if (absolute_column_delta > absolute_row_delta)
  {
    const f32 column_delta_reciprocal = 1.0f / absolute_column_delta;

    f32 left_depth, right_depth, left_row, right_row;
    color left_color, right_color;

    if (start_column > end_column)
    {
      left_depth = end_depth;
      right_depth = start_depth;
      left_row = end_row;
      right_row = start_row;
      left_color = end_color;
      right_color = start_color;
    }
    else
    {
      left_depth = start_depth;
      right_depth = end_depth;
      left_row = start_row;
      right_row = end_row;
      left_color = start_color;
      right_color = end_color;
    }

    f32 depth_accumulator = left_depth;
    const f32 depth_delta = right_depth - left_depth;
    const f32 depth_per_column = depth_delta * column_delta_reciprocal;

    f32 color_accumulator_red = EXTRACT_RED(left_color);
    f32 color_accumulator_green = EXTRACT_GREEN(left_color);
    f32 color_accumulator_blue = EXTRACT_BLUE(left_color);
    f32 color_accumulator_alpha = EXTRACT_OPACITY(left_color);

    const f32 color_delta_red = EXTRACT_RED(right_color) - color_accumulator_red;
    const f32 color_delta_green = EXTRACT_GREEN(right_color) - color_accumulator_green;
    const f32 color_delta_blue = EXTRACT_BLUE(right_color) - color_accumulator_blue;
    const f32 color_delta_alpha = EXTRACT_OPACITY(right_color) - color_accumulator_alpha;

    const f32 color_per_column_red = color_delta_red * column_delta_reciprocal;
    const f32 color_per_column_green = color_delta_green * column_delta_reciprocal;
    const f32 color_per_column_blue = color_delta_blue * column_delta_reciprocal;
    const f32 color_per_column_alpha = color_delta_alpha * column_delta_reciprocal;

    f32 row_accumulator = left_row;
    const f32 row_delta = right_row - left_row;
    const f32 rows_per_column = row_delta * column_delta_reciprocal;

    const s32 min_column = MAX(0, left_column);
    const s32 max_column = MIN(right_column, camera_component_columns - 1);

    for (s32 column = min_column; column <= max_column; column++)
    {
      const s32 row = row_accumulator;

      if (row >= 0 && row < camera_component_rows)
      {
        const index destination_index = row * video_columns + column;

        if (depth_accumulator < camera_component_depths[destination_index])
        {
          camera_component_depths[destination_index] = depth_accumulator;
          camera_component_colors[destination_index] = CLAMPED_COLOR((s32)color_accumulator_red, (s32)color_accumulator_green, (s32)color_accumulator_blue, (s32)color_accumulator_alpha);
        }
      }

      depth_accumulator += depth_per_column;
      color_accumulator_red += color_per_column_red;
      color_accumulator_green += color_per_column_green;
      color_accumulator_blue += color_per_column_blue;
      color_accumulator_alpha += color_per_column_alpha;
      row_accumulator += rows_per_column;
    }
  }
  else
  {
    const f32 row_delta_reciprocal = 1.0f / absolute_row_delta;

    f32 top_depth, bottom_depth, top_column, bottom_column;
    color top_color, bottom_color;

    if (start_row > end_row)
    {
      top_depth = end_depth;
      bottom_depth = start_depth;
      top_column = end_column;
      bottom_column = start_column;
      top_color = end_color;
      bottom_color = start_color;
    }
    else
    {
      top_depth = start_depth;
      bottom_depth = end_depth;
      top_column = start_column;
      bottom_column = end_column;
      top_color = start_color;
      bottom_color = end_color;
    }

    f32 depth_accumulator = top_depth;
    const f32 depth_delta = bottom_depth - top_depth;
    const f32 depth_per_row = depth_delta * row_delta_reciprocal;

    f32 color_accumulator_red = EXTRACT_RED(top_color);
    f32 color_accumulator_green = EXTRACT_GREEN(top_color);
    f32 color_accumulator_blue = EXTRACT_BLUE(top_color);
    f32 color_accumulator_alpha = EXTRACT_OPACITY(top_color);

    const f32 color_delta_red = EXTRACT_RED(bottom_color) - color_accumulator_red;
    const f32 color_delta_green = EXTRACT_GREEN(bottom_color) - color_accumulator_green;
    const f32 color_delta_blue = EXTRACT_BLUE(bottom_color) - color_accumulator_blue;
    const f32 color_delta_alpha = EXTRACT_OPACITY(bottom_color) - color_accumulator_alpha;

    const f32 color_per_row_red = color_delta_red * row_delta_reciprocal;
    const f32 color_per_row_green = color_delta_green * row_delta_reciprocal;
    const f32 color_per_row_blue = color_delta_blue * row_delta_reciprocal;
    const f32 color_per_row_alpha = color_delta_alpha * row_delta_reciprocal;

    f32 column_accumulator = top_column;
    const f32 column_delta = bottom_column - top_column;
    const f32 columns_per_row = column_delta * row_delta_reciprocal;

    const s32 min_row = MAX(0, top_row);
    const s32 max_row = MIN(bottom_row, camera_component_rows - 1);

    for (s32 row = min_row; row <= max_row; row++)
    {
      const s32 column = column_accumulator;

      if (column >= 0 && column < camera_component_columns)
      {
        const index destination_index = row * video_columns + column;

        if (depth_accumulator < camera_component_depths[destination_index])
        {
          camera_component_depths[destination_index] = depth_accumulator;
          camera_component_colors[destination_index] = CLAMPED_COLOR((s32)color_accumulator_red, (s32)color_accumulator_green, (s32)color_accumulator_blue, (s32)color_accumulator_alpha);
        }
      }

      depth_accumulator += depth_per_row;
      color_accumulator_red += color_per_row_red;
      color_accumulator_green += color_per_row_green;
      color_accumulator_blue += color_per_row_blue;
      color_accumulator_alpha += color_per_row_alpha;
      column_accumulator += columns_per_row;
    }
  }
}

void render_transparent_line(
    const color start_color,
    const s32 start_row,
    const s32 start_column,
    const f32 start_depth,
    const color end_color,
    const s32 end_row,
    const s32 end_column,
    const f32 end_depth)
{
  s32 top_row, bottom_row;

  if (start_row > end_row)
  {
    top_row = end_row;
    bottom_row = start_row;
  }
  else
  {
    top_row = start_row;
    bottom_row = end_row;
  }

  const s32 absolute_row_delta = bottom_row - top_row;

  s32 left_column, right_column;

  if (start_column > end_column)
  {
    left_column = end_column;
    right_column = start_column;
  }
  else
  {
    left_column = start_column;
    right_column = end_column;
  }

  const s32 absolute_column_delta = right_column - left_column;

  if (!absolute_column_delta && !absolute_row_delta)
  {
    const index destination_index = start_row * camera_component_columns + start_column;
    if (start_depth < camera_component_depths[destination_index])
    {
      camera_component_colors[destination_index] = LAYER_COLORS(start_color, camera_component_colors[destination_index]);
    }
  }
  else if (absolute_column_delta > absolute_row_delta)
  {
    const f32 column_delta_reciprocal = 1.0f / absolute_column_delta;

    f32 left_depth, right_depth, left_row, right_row;
    color left_color, right_color;

    if (start_column > end_column)
    {
      left_depth = end_depth;
      right_depth = start_depth;
      left_row = end_row;
      right_row = start_row;
      left_color = end_color;
      right_color = start_color;
    }
    else
    {
      left_depth = start_depth;
      right_depth = end_depth;
      left_row = start_row;
      right_row = end_row;
      left_color = start_color;
      right_color = end_color;
    }

    f32 depth_accumulator = left_depth;
    const f32 depth_delta = right_depth - left_depth;
    const f32 depth_per_column = depth_delta * column_delta_reciprocal;

    f32 color_accumulator_red = EXTRACT_RED(left_color);
    f32 color_accumulator_green = EXTRACT_GREEN(left_color);
    f32 color_accumulator_blue = EXTRACT_BLUE(left_color);
    f32 color_accumulator_alpha = EXTRACT_OPACITY(left_color);

    const f32 color_delta_red = EXTRACT_RED(right_color) - color_accumulator_red;
    const f32 color_delta_green = EXTRACT_GREEN(right_color) - color_accumulator_green;
    const f32 color_delta_blue = EXTRACT_BLUE(right_color) - color_accumulator_blue;
    const f32 color_delta_alpha = EXTRACT_OPACITY(right_color) - color_accumulator_alpha;

    const f32 color_per_column_red = color_delta_red * column_delta_reciprocal;
    const f32 color_per_column_green = color_delta_green * column_delta_reciprocal;
    const f32 color_per_column_blue = color_delta_blue * column_delta_reciprocal;
    const f32 color_per_column_alpha = color_delta_alpha * column_delta_reciprocal;

    f32 row_accumulator = left_row;
    const f32 row_delta = right_row - left_row;
    const f32 rows_per_column = row_delta * column_delta_reciprocal;

    const s32 min_column = MAX(0, left_column);
    const s32 max_column = MIN(right_column, camera_component_columns - 1);

    for (s32 column = min_column; column <= max_column; column++)
    {
      const s32 row = row_accumulator;

      if (row >= 0 && row < camera_component_rows)
      {
        const index destination_index = row * video_columns + column;

        if (depth_accumulator < camera_component_depths[destination_index])
        {
          const color source_color = CLAMPED_COLOR((s32)color_accumulator_red, (s32)color_accumulator_green, (s32)color_accumulator_blue, (s32)color_accumulator_alpha);
          const color destination_color = camera_component_colors[destination_index];

          camera_component_colors[destination_index] = LAYER_COLORS(source_color, destination_color);
        }
      }

      depth_accumulator += depth_per_column;
      color_accumulator_red += color_per_column_red;
      color_accumulator_green += color_per_column_green;
      color_accumulator_blue += color_per_column_blue;
      color_accumulator_alpha += color_per_column_alpha;
      row_accumulator += rows_per_column;
    }
  }
  else
  {
    const f32 row_delta_reciprocal = 1.0f / absolute_row_delta;

    f32 top_depth, bottom_depth, top_column, bottom_column;
    color top_color, bottom_color;

    if (start_row > end_row)
    {
      top_depth = end_depth;
      bottom_depth = start_depth;
      top_column = end_column;
      bottom_column = start_column;
      top_color = end_color;
      bottom_color = start_color;
    }
    else
    {
      top_depth = start_depth;
      bottom_depth = end_depth;
      top_column = start_column;
      bottom_column = end_column;
      top_color = start_color;
      bottom_color = end_color;
    }

    f32 depth_accumulator = top_depth;
    const f32 depth_delta = bottom_depth - top_depth;
    const f32 depth_per_row = depth_delta * row_delta_reciprocal;

    f32 color_accumulator_red = EXTRACT_RED(top_color);
    f32 color_accumulator_green = EXTRACT_GREEN(top_color);
    f32 color_accumulator_blue = EXTRACT_BLUE(top_color);
    f32 color_accumulator_alpha = EXTRACT_OPACITY(top_color);

    const f32 color_delta_red = EXTRACT_RED(bottom_color) - color_accumulator_red;
    const f32 color_delta_green = EXTRACT_GREEN(bottom_color) - color_accumulator_green;
    const f32 color_delta_blue = EXTRACT_BLUE(bottom_color) - color_accumulator_blue;
    const f32 color_delta_alpha = EXTRACT_OPACITY(bottom_color) - color_accumulator_alpha;

    const f32 color_per_row_red = color_delta_red * row_delta_reciprocal;
    const f32 color_per_row_green = color_delta_green * row_delta_reciprocal;
    const f32 color_per_row_blue = color_delta_blue * row_delta_reciprocal;
    const f32 color_per_row_alpha = color_delta_alpha * row_delta_reciprocal;

    f32 column_accumulator = top_column;
    const f32 column_delta = bottom_column - top_column;
    const f32 columns_per_row = column_delta * row_delta_reciprocal;

    const s32 min_row = MAX(0, top_row);
    const s32 max_row = MIN(bottom_row, camera_component_rows - 1);

    for (s32 row = min_row; row <= max_row; row++)
    {
      const s32 column = column_accumulator;

      if (column >= 0 && column < camera_component_columns)
      {
        const index destination_index = row * video_columns + column;

        if (depth_accumulator < camera_component_depths[destination_index])
        {
          const color source_color = CLAMPED_COLOR((s32)color_accumulator_red, (s32)color_accumulator_green, (s32)color_accumulator_blue, (s32)color_accumulator_alpha);
          const color destination_color = camera_component_colors[destination_index];

          camera_component_colors[destination_index] = LAYER_COLORS(source_color, destination_color);
        }
      }

      depth_accumulator += depth_per_row;
      color_accumulator_red += color_per_row_red;
      color_accumulator_green += color_per_row_green;
      color_accumulator_blue += color_per_row_blue;
      color_accumulator_alpha += color_per_row_alpha;
      column_accumulator += columns_per_row;
    }
  }
}
