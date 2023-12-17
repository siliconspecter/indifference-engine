#include "../primitives/s32.h"
#include "../primitives/f32.h"
#include "../primitives/index.h"
#include "../primitives/quantity.h"
#include "../math/relational.h"
#include "../math/float.h"
#include "../scenes/components/camera_component.h"

// TODO
#include "../exports/buffers/error.h"

static void sort_top_to_bottom(
    f32 *const vertices,
    const quantity f32s_per_vertex,
    const f32 **const top,
    const f32 **const middle,
    const f32 **const bottom)
{
  f32 *const a = vertices;
  f32 *const b = &a[f32s_per_vertex];
  f32 *const c = &b[f32s_per_vertex];

  const f32 a_row = *a;
  const f32 b_row = *b;
  const f32 c_row = *c;

  if (a_row <= b_row)
  {
    if (b_row <= c_row)
    {
      *top = a;
      *middle = b;
      *bottom = c;
    }
    else if (a_row <= c_row)
    {
      *top = a;
      *middle = c;
      *bottom = b;
    }
    else
    {
      *top = c;
      *middle = a;
      *bottom = b;
    }
  }
  else if (a_row <= c_row)
  {
    *top = b;
    *middle = a;
    *bottom = c;
  }
  else if (b_row <= c_row)
  {
    *top = b;
    *middle = c;
    *bottom = a;
  }
  else
  {
    *top = c;
    *middle = b;
    *bottom = a;
  }
}

static void render_opaque_row(
    const quantity texture_rows,
    const quantity texture_rows_minus_one,
    const quantity texture_columns,
    const quantity texture_columns_minus_one,
    const f32 *const texture_reds,
    const f32 *const texture_greens,
    const f32 *const texture_blues,
    const f32 row_accumulators[14],
    const s32 camera_row)
{
  f32 per_columns[6];
  f32 accumulators[6];

  const f32 first_camera_column = row_accumulators[0];
  const f32 second_camera_column = row_accumulators[7];

  subtract_f32s_f32s(&row_accumulators[8], &row_accumulators[1], per_columns, 6);
  multiply_f32s_f32(per_columns, 1.0f / (second_camera_column - first_camera_column), per_columns, 6);

  s32 clamped_left_camera_column, clamped_right_camera_column;

  if (first_camera_column < second_camera_column)
  {
    clamped_left_camera_column = floor(first_camera_column);
    clamped_right_camera_column = floor(second_camera_column);
    copy_f32s(&row_accumulators[1], accumulators, 6);
  }
  else
  {
    clamped_left_camera_column = floor(second_camera_column);
    clamped_right_camera_column = floor(first_camera_column);
    copy_f32s(&row_accumulators[8], accumulators, 6);
  }

  if (clamped_left_camera_column < 0)
  {
    multiply_add_f32s_f32_f32s(per_columns, -clamped_left_camera_column, accumulators, accumulators, 5);
    clamped_left_camera_column = 0;
  }

  clamped_right_camera_column = MIN(clamped_right_camera_column, camera_component_columns);

  const index left_index = camera_row * camera_component_columns + clamped_left_camera_column;
  const index right_index = left_index + clamped_right_camera_column - clamped_left_camera_column;

  for (index camera_index = left_index; camera_index < right_index; camera_index++)
  {
    const f32 source_depth = accumulators[0];

    if (source_depth < camera_component_depths[camera_index])
    {

      const s32 texture_row = texture_rows * accumulators[1];
      const s32 clamped_texture_row = CLAMP(texture_row, 0, texture_rows_minus_one);
      const s32 texture_column = texture_columns * accumulators[2];
      const s32 clamped_texture_column = CLAMP(texture_column, 0, texture_columns_minus_one);

      const index texture_index = clamped_texture_row * texture_columns + clamped_texture_column;

      camera_component_depths[camera_index] = source_depth;
      camera_component_reds[camera_index] = accumulators[3] * texture_reds[texture_index];
      camera_component_greens[camera_index] = accumulators[4] * texture_greens[texture_index];
      camera_component_blues[camera_index] = accumulators[5] * texture_blues[texture_index];
      camera_component_opacities[camera_index] = 1.0f;
    }

    add_f32s_f32s(accumulators, per_columns, accumulators, 6);
  }
}

void render_opaque_triangle(
    const quantity texture_rows,
    const quantity texture_rows_minus_one,
    const quantity texture_columns,
    const quantity texture_columns_minus_one,
    const f32 *const texture_reds,
    const f32 *const texture_greens,
    const f32 *const texture_blues,
    const f32 a_camera_row,
    const f32 a_camera_column,
    const f32 a_depth,
    const f32 a_texture_row,
    const f32 a_texture_column,
    const f32 a_red,
    const f32 a_green,
    const f32 a_blue,
    const f32 b_camera_row,
    const f32 b_camera_column,
    const f32 b_depth,
    const f32 b_texture_row,
    const f32 b_texture_column,
    const f32 b_red,
    const f32 b_green,
    const f32 b_blue,
    const f32 c_camera_row,
    const f32 c_camera_column,
    const f32 c_depth,
    const f32 c_texture_row,
    const f32 c_texture_column,
    const f32 c_red,
    const f32 c_green,
    const f32 c_blue)
{
  f32 vertices[] = {
      a_camera_row,     // 0
      a_camera_column,  // 1
      a_depth,          // 2
      a_texture_row,    // 3
      a_texture_column, // 4
      a_red,            // 5
      a_green,          // 6
      a_blue,           // 7
      b_camera_row,     // 8
      b_camera_column,  // 9
      b_depth,          // 10
      b_texture_row,    // 11
      b_texture_column, // 12
      b_red,            // 13
      b_green,          // 14
      b_blue,           // 15
      c_camera_row,     // 16
      c_camera_column,  // 17
      c_depth,          // 18
      c_texture_row,    // 19
      c_texture_column, // 20
      c_red,            // 21
      c_green,          // 22
      c_blue,           // 23
  };

  const f32 *top;
  const f32 *middle;
  const f32 *bottom;

  sort_top_to_bottom(vertices, 8, &top, &middle, &bottom);

  f32 deltas[18];
  subtract_f32s_f32s(bottom, top, deltas, 8);
  subtract_f32s_f32s(middle, top, &deltas[8], 8);

  f32 per_rows[17];
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], per_rows, 7);
  const f32 short_camera_row_delta = deltas[8];
  multiply_f32s_f32(&deltas[9], 1.0f / short_camera_row_delta, &per_rows[7], 7);

  f32 accumulators[17];

  const s32 top_camera_row = floor(top[0]);
  const s32 clamped_top_camera_row = MAX(0, top_camera_row);

  if (top_camera_row < 0)
  {
    const f32 skipped_rows = -top_camera_row;
    multiply_add_f32s_f32_f32s(per_rows, skipped_rows, &top[1], accumulators, 7);
    multiply_add_f32s_f32_f32s(&per_rows[7], skipped_rows, &top[1], &accumulators[7], 7);
  }
  else
  {
    copy_f32s(&top[1], accumulators, 7);
    copy_f32s(&top[1], &accumulators[7], 7);
  }

  const s32 middle_camera_row = floor(middle[0]);
  const s32 clamped_middle_camera_row = CLAMP(middle_camera_row, 0, camera_component_rows);

  for (s32 camera_row = clamped_top_camera_row; camera_row < clamped_middle_camera_row; camera_row++)
  {
    render_opaque_row(
        texture_rows,
        texture_rows_minus_one,
        texture_columns,
        texture_columns_minus_one,
        texture_reds,
        texture_greens,
        texture_blues,
        accumulators,
        camera_row);

    add_f32s_f32s(accumulators, per_rows, accumulators, 14);
  }

  subtract_f32s_f32s(bottom, middle, deltas, 8);
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], &per_rows[7], 7);

  if (middle_camera_row < 0)
  {
    const f32 skipped_rows = -middle_camera_row;
    multiply_add_f32s_f32_f32s(&per_rows[7], skipped_rows, &middle[1], &accumulators[7], 7);
  }
  else
  {
    copy_f32s(&middle[1], &accumulators[7], 7);
  }

  const s32 bottom_camera_row = *bottom;
  const s32 clamped_bottom_camera_row = MIN(camera_component_rows, bottom_camera_row);

  for (s32 camera_row = clamped_middle_camera_row; camera_row < clamped_bottom_camera_row; camera_row++)
  {
    render_opaque_row(
        texture_rows,
        texture_rows_minus_one,
        texture_columns,
        texture_columns_minus_one,
        texture_reds,
        texture_greens,
        texture_blues,
        accumulators,
        camera_row);

    add_f32s_f32s(accumulators, per_rows, accumulators, 14);
  }
}

static void render_cutout_row(
    const quantity texture_rows,
    const quantity texture_rows_minus_one,
    const quantity texture_columns,
    const quantity texture_columns_minus_one,
    const f32 *const texture_reds,
    const f32 *const texture_greens,
    const f32 *const texture_blues,
    const f32 *const texture_opacities,
    const f32 row_accumulators[16],
    const s32 camera_row)
{
  f32 per_columns[7];
  f32 accumulators[7];

  const f32 first_camera_column = row_accumulators[0];
  const f32 second_camera_column = row_accumulators[8];

  subtract_f32s_f32s(&row_accumulators[9], &row_accumulators[1], per_columns, 7);
  multiply_f32s_f32(per_columns, 1.0f / (second_camera_column - first_camera_column), per_columns, 7);

  s32 clamped_left_camera_column, clamped_right_camera_column;

  if (first_camera_column < second_camera_column)
  {
    clamped_left_camera_column = floor(first_camera_column);
    clamped_right_camera_column = floor(second_camera_column);
    copy_f32s(&row_accumulators[1], accumulators, 7);
  }
  else
  {
    clamped_left_camera_column = floor(second_camera_column);
    clamped_right_camera_column = floor(first_camera_column);
    copy_f32s(&row_accumulators[9], accumulators, 7);
  }

  if (clamped_left_camera_column < 0)
  {
    multiply_add_f32s_f32_f32s(per_columns, -clamped_left_camera_column, accumulators, accumulators, 6);
    clamped_left_camera_column = 0;
  }

  clamped_right_camera_column = MIN(clamped_right_camera_column, camera_component_columns);

  const index left_index = camera_row * camera_component_columns + clamped_left_camera_column;
  const index right_index = left_index + clamped_right_camera_column - clamped_left_camera_column;

  for (index camera_index = left_index; camera_index < right_index; camera_index++)
  {
    const f32 source_depth = accumulators[0];

    if (source_depth < camera_component_depths[camera_index])
    {
      const s32 texture_row = texture_rows * accumulators[1];
      const s32 clamped_texture_row = CLAMP(texture_row, 0, texture_rows_minus_one);
      const s32 texture_column = texture_columns * accumulators[2];
      const s32 clamped_texture_column = CLAMP(texture_column, 0, texture_columns_minus_one);

      const index texture_index = clamped_texture_row * texture_columns + clamped_texture_column;

      const f32 opacity = accumulators[3] * texture_opacities[texture_index];

      if (opacity >= 0.5f)
      {
        camera_component_depths[camera_index] = source_depth;
        camera_component_reds[camera_index] = accumulators[4] * texture_reds[texture_index];
        camera_component_greens[camera_index] = accumulators[5] * texture_greens[texture_index];
        camera_component_blues[camera_index] = accumulators[6] * texture_blues[texture_index];
        camera_component_opacities[camera_index] = 1.0f;
      }
    }

    add_f32s_f32s(accumulators, per_columns, accumulators, 7);
  }
}

void render_cutout_triangle(
    const quantity texture_rows,
    const quantity texture_rows_minus_one,
    const quantity texture_columns,
    const quantity texture_columns_minus_one,
    const f32 *const texture_reds,
    const f32 *const texture_greens,
    const f32 *const texture_blues,
    const f32 *const texture_opacities,
    const f32 a_camera_row,
    const f32 a_camera_column,
    const f32 a_depth,
    const f32 a_texture_row,
    const f32 a_texture_column,
    const f32 a_opacity,
    const f32 a_red,
    const f32 a_green,
    const f32 a_blue,
    const f32 b_camera_row,
    const f32 b_camera_column,
    const f32 b_depth,
    const f32 b_texture_row,
    const f32 b_texture_column,
    const f32 b_opacity,
    const f32 b_red,
    const f32 b_green,
    const f32 b_blue,
    const f32 c_camera_row,
    const f32 c_camera_column,
    const f32 c_depth,
    const f32 c_texture_row,
    const f32 c_texture_column,
    const f32 c_opacity,
    const f32 c_red,
    const f32 c_green,
    const f32 c_blue)
{
  f32 vertices[] = {
      a_camera_row,     // 0
      a_camera_column,  // 1
      a_depth,          // 2
      a_texture_row,    // 3
      a_texture_column, // 4
      a_opacity,        // 5
      a_red,            // 6
      a_green,          // 7
      a_blue,           // 8
      b_camera_row,     // 9
      b_camera_column,  // 10
      b_depth,          // 11
      b_texture_row,    // 12
      b_texture_column, // 13
      b_opacity,        // 14
      b_red,            // 15
      b_green,          // 16
      b_blue,           // 17
      c_camera_row,     // 18
      c_camera_column,  // 19
      c_depth,          // 20
      c_texture_row,    // 21
      c_texture_column, // 22
      c_opacity,        // 23
      c_red,            // 24
      c_green,          // 25
      c_blue,           // 26
  };

  const f32 *top;
  const f32 *middle;
  const f32 *bottom;

  sort_top_to_bottom(vertices, 9, &top, &middle, &bottom);

  f32 deltas[18];
  subtract_f32s_f32s(bottom, top, deltas, 9);
  subtract_f32s_f32s(middle, top, &deltas[9], 9);

  f32 per_rows[17];
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], per_rows, 8);
  const f32 short_camera_row_delta = deltas[9];
  multiply_f32s_f32(&deltas[10], 1.0f / short_camera_row_delta, &per_rows[8], 8);

  f32 accumulators[17];

  const s32 top_camera_row = floor(top[0]);
  const s32 clamped_top_camera_row = MAX(0, top_camera_row);

  if (top_camera_row < 0)
  {
    const f32 skipped_rows = -top_camera_row;
    multiply_add_f32s_f32_f32s(per_rows, skipped_rows, &top[1], accumulators, 8);
    multiply_add_f32s_f32_f32s(&per_rows[8], skipped_rows, &top[1], &accumulators[8], 8);
  }
  else
  {
    copy_f32s(&top[1], accumulators, 8);
    copy_f32s(&top[1], &accumulators[8], 8);
  }

  const s32 middle_camera_row = floor(middle[0]);
  const s32 clamped_middle_camera_row = CLAMP(middle_camera_row, 0, camera_component_rows);

  for (s32 camera_row = clamped_top_camera_row; camera_row < clamped_middle_camera_row; camera_row++)
  {
    render_cutout_row(
        texture_rows,
        texture_rows_minus_one,
        texture_columns,
        texture_columns_minus_one,
        texture_reds,
        texture_greens,
        texture_blues,
        texture_opacities,
        accumulators,
        camera_row);

    add_f32s_f32s(accumulators, per_rows, accumulators, 16);
  }

  subtract_f32s_f32s(bottom, middle, deltas, 9);
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], &per_rows[8], 8);

  if (middle_camera_row < 0)
  {
    const f32 skipped_rows = -middle_camera_row;
    multiply_add_f32s_f32_f32s(&per_rows[8], skipped_rows, &middle[1], &accumulators[8], 8);
  }
  else
  {
    copy_f32s(&middle[1], &accumulators[8], 8);
  }

  const s32 bottom_camera_row = *bottom;
  const s32 clamped_bottom_camera_row = MIN(camera_component_rows, bottom_camera_row);

  for (s32 camera_row = clamped_middle_camera_row; camera_row < clamped_bottom_camera_row; camera_row++)
  {
    render_cutout_row(
        texture_rows,
        texture_rows_minus_one,
        texture_columns,
        texture_columns_minus_one,
        texture_reds,
        texture_greens,
        texture_blues,
        texture_opacities,
        accumulators,
        camera_row);

    add_f32s_f32s(accumulators, per_rows, accumulators, 16);
  }
}

static void render_additive_row(
    const quantity texture_rows,
    const quantity texture_rows_minus_one,
    const quantity texture_columns,
    const quantity texture_columns_minus_one,
    const f32 *const texture_reds,
    const f32 *const texture_greens,
    const f32 *const texture_blues,
    const f32 row_accumulators[16],
    const s32 camera_row)
{
  f32 per_columns[6];
  f32 accumulators[6];

  const f32 first_camera_column = row_accumulators[0];
  const f32 second_camera_column = row_accumulators[7];

  subtract_f32s_f32s(&row_accumulators[8], &row_accumulators[1], per_columns, 6);
  multiply_f32s_f32(per_columns, 1.0f / (second_camera_column - first_camera_column), per_columns, 6);

  s32 clamped_left_camera_column, clamped_right_camera_column;

  if (first_camera_column < second_camera_column)
  {
    clamped_left_camera_column = floor(first_camera_column);
    clamped_right_camera_column = floor(second_camera_column);
    copy_f32s(&row_accumulators[1], accumulators, 6);
  }
  else
  {
    clamped_left_camera_column = floor(second_camera_column);
    clamped_right_camera_column = floor(first_camera_column);
    copy_f32s(&row_accumulators[8], accumulators, 6);
  }

  if (clamped_left_camera_column < 0)
  {
    multiply_add_f32s_f32_f32s(per_columns, -clamped_left_camera_column, accumulators, accumulators, 5);
    clamped_left_camera_column = 0;
  }

  clamped_right_camera_column = MIN(clamped_right_camera_column, camera_component_columns);

  const index left_index = camera_row * camera_component_columns + clamped_left_camera_column;
  const index right_index = left_index + clamped_right_camera_column - clamped_left_camera_column;

  for (index camera_index = left_index; camera_index < right_index; camera_index++)
  {
    const f32 source_depth = accumulators[0];

    if (source_depth < camera_component_depths[camera_index])
    {
      const s32 texture_row = texture_rows * accumulators[1];
      const s32 clamped_texture_row = CLAMP(texture_row, 0, texture_rows_minus_one);
      const s32 texture_column = texture_columns * accumulators[2];
      const s32 clamped_texture_column = CLAMP(texture_column, 0, texture_columns_minus_one);

      const index texture_index = clamped_texture_row * texture_columns + clamped_texture_column;

      camera_component_reds[camera_index] += accumulators[3] * texture_reds[texture_index];
      camera_component_greens[camera_index] += accumulators[4] * texture_greens[texture_index];
      camera_component_blues[camera_index] += accumulators[5] * texture_blues[texture_index];

      // TODO: Update opacities
      // camera_component_opacities[camera_index] = 1.0f;
    }

    add_f32s_f32s(accumulators, per_columns, accumulators, 6);
  }
}

void render_additive_triangle(
    const quantity texture_rows,
    const quantity texture_rows_minus_one,
    const quantity texture_columns,
    const quantity texture_columns_minus_one,
    const f32 *const texture_reds,
    const f32 *const texture_greens,
    const f32 *const texture_blues,
    const f32 a_camera_row,
    const f32 a_camera_column,
    const f32 a_depth,
    const f32 a_texture_row,
    const f32 a_texture_column,
    const f32 a_red,
    const f32 a_green,
    const f32 a_blue,
    const f32 b_camera_row,
    const f32 b_camera_column,
    const f32 b_depth,
    const f32 b_texture_row,
    const f32 b_texture_column,
    const f32 b_red,
    const f32 b_green,
    const f32 b_blue,
    const f32 c_camera_row,
    const f32 c_camera_column,
    const f32 c_depth,
    const f32 c_texture_row,
    const f32 c_texture_column,
    const f32 c_red,
    const f32 c_green,
    const f32 c_blue)
{
  f32 vertices[] = {
      a_camera_row,     // 0
      a_camera_column,  // 1
      a_depth,          // 2
      a_texture_row,    // 3
      a_texture_column, // 4
      a_red,            // 5
      a_green,          // 6
      a_blue,           // 7
      b_camera_row,     // 8
      b_camera_column,  // 9
      b_depth,          // 10
      b_texture_row,    // 11
      b_texture_column, // 12
      b_red,            // 13
      b_green,          // 14
      b_blue,           // 15
      c_camera_row,     // 16
      c_camera_column,  // 17
      c_depth,          // 18
      c_texture_row,    // 19
      c_texture_column, // 20
      c_red,            // 21
      c_green,          // 22
      c_blue,           // 23
  };

  const f32 *top;
  const f32 *middle;
  const f32 *bottom;

  sort_top_to_bottom(vertices, 8, &top, &middle, &bottom);

  f32 deltas[18];
  subtract_f32s_f32s(bottom, top, deltas, 8);
  subtract_f32s_f32s(middle, top, &deltas[8], 8);

  f32 per_rows[17];
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], per_rows, 7);
  const f32 short_camera_row_delta = deltas[8];
  multiply_f32s_f32(&deltas[9], 1.0f / short_camera_row_delta, &per_rows[7], 7);

  f32 accumulators[17];

  const s32 top_camera_row = floor(top[0]);
  const s32 clamped_top_camera_row = MAX(0, top_camera_row);

  if (top_camera_row < 0)
  {
    const f32 skipped_rows = -top_camera_row;
    multiply_add_f32s_f32_f32s(per_rows, skipped_rows, &top[1], accumulators, 7);
    multiply_add_f32s_f32_f32s(&per_rows[7], skipped_rows, &top[1], &accumulators[7], 7);
  }
  else
  {
    copy_f32s(&top[1], accumulators, 7);
    copy_f32s(&top[1], &accumulators[7], 7);
  }

  const s32 middle_camera_row = floor(middle[0]);
  const s32 clamped_middle_camera_row = CLAMP(middle_camera_row, 0, camera_component_rows);

  for (s32 camera_row = clamped_top_camera_row; camera_row < clamped_middle_camera_row; camera_row++)
  {
    render_additive_row(
        texture_rows,
        texture_rows_minus_one,
        texture_columns,
        texture_columns_minus_one,
        texture_reds,
        texture_greens,
        texture_blues,
        accumulators,
        camera_row);

    add_f32s_f32s(accumulators, per_rows, accumulators, 14);
  }

  subtract_f32s_f32s(bottom, middle, deltas, 8);
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], &per_rows[7], 7);

  if (middle_camera_row < 0)
  {
    const f32 skipped_rows = -middle_camera_row;
    multiply_add_f32s_f32_f32s(&per_rows[7], skipped_rows, &middle[1], &accumulators[7], 7);
  }
  else
  {
    copy_f32s(&middle[1], &accumulators[7], 7);
  }

  const s32 bottom_camera_row = *bottom;
  const s32 clamped_bottom_camera_row = MIN(camera_component_rows, bottom_camera_row);

  for (s32 camera_row = clamped_middle_camera_row; camera_row < clamped_bottom_camera_row; camera_row++)
  {
    render_additive_row(
        texture_rows,
        texture_rows_minus_one,
        texture_columns,
        texture_columns_minus_one,
        texture_reds,
        texture_greens,
        texture_blues,
        accumulators,
        camera_row);

    add_f32s_f32s(accumulators, per_rows, accumulators, 14);
  }
}

static void render_blended_row(
    const quantity texture_rows,
    const quantity texture_rows_minus_one,
    const quantity texture_columns,
    const quantity texture_columns_minus_one,
    const f32 *const texture_reds,
    const f32 *const texture_greens,
    const f32 *const texture_blues,
    const f32 *const texture_opacities,
    const f32 row_accumulators[16],
    const s32 camera_row)
{
  f32 per_columns[7];
  f32 accumulators[7];

  const f32 first_camera_column = row_accumulators[0];
  const f32 second_camera_column = row_accumulators[8];

  subtract_f32s_f32s(&row_accumulators[9], &row_accumulators[1], per_columns, 7);
  multiply_f32s_f32(per_columns, 1.0f / (second_camera_column - first_camera_column), per_columns, 7);

  s32 clamped_left_camera_column, clamped_right_camera_column;

  if (first_camera_column < second_camera_column)
  {
    clamped_left_camera_column = floor(first_camera_column);
    clamped_right_camera_column = floor(second_camera_column);
    copy_f32s(&row_accumulators[1], accumulators, 7);
  }
  else
  {
    clamped_left_camera_column = floor(second_camera_column);
    clamped_right_camera_column = floor(first_camera_column);
    copy_f32s(&row_accumulators[9], accumulators, 7);
  }

  if (clamped_left_camera_column < 0)
  {
    multiply_add_f32s_f32_f32s(per_columns, -clamped_left_camera_column, accumulators, accumulators, 6);
    clamped_left_camera_column = 0;
  }

  clamped_right_camera_column = MIN(clamped_right_camera_column, camera_component_columns);

  const index left_index = camera_row * camera_component_columns + clamped_left_camera_column;
  const index right_index = left_index + clamped_right_camera_column - clamped_left_camera_column;

  for (index camera_index = left_index; camera_index < right_index; camera_index++)
  {
    const f32 source_depth = accumulators[0];

    if (source_depth < camera_component_depths[camera_index])
    {
      const s32 texture_row = texture_rows * accumulators[1];
      const s32 clamped_texture_row = CLAMP(texture_row, 0, texture_rows_minus_one);
      const s32 texture_column = texture_columns * accumulators[2];
      const s32 clamped_texture_column = CLAMP(texture_column, 0, texture_columns_minus_one);

      const index texture_index = clamped_texture_row * texture_columns + clamped_texture_column;

      const f32 unclamped_opacity = accumulators[3] * texture_opacities[texture_index];
      const f32 clamped_opacity = CLAMP(unclamped_opacity, 0.0f, 1.0f);
      const f32 inverse_opacity = 1.0f - clamped_opacity;
      camera_component_reds[camera_index] = camera_component_reds[camera_index] * inverse_opacity + accumulators[4] * texture_reds[texture_index] * clamped_opacity;
      camera_component_greens[camera_index] = camera_component_reds[camera_index] * inverse_opacity + accumulators[5] * texture_greens[texture_index] * clamped_opacity;
      camera_component_blues[camera_index] = camera_component_reds[camera_index] * inverse_opacity + accumulators[6] * texture_blues[texture_index] * clamped_opacity;
      camera_component_opacities[camera_index] = 1.0f;
      // TODO: implement mixing of colors
    }

    add_f32s_f32s(accumulators, per_columns, accumulators, 7);
  }
}

void render_blended_triangle(
    const quantity texture_rows,
    const quantity texture_rows_minus_one,
    const quantity texture_columns,
    const quantity texture_columns_minus_one,
    const f32 *const texture_reds,
    const f32 *const texture_greens,
    const f32 *const texture_blues,
    const f32 *const texture_opacities,
    const f32 a_camera_row,
    const f32 a_camera_column,
    const f32 a_depth,
    const f32 a_texture_row,
    const f32 a_texture_column,
    const f32 a_opacity,
    const f32 a_red,
    const f32 a_green,
    const f32 a_blue,
    const f32 b_camera_row,
    const f32 b_camera_column,
    const f32 b_depth,
    const f32 b_texture_row,
    const f32 b_texture_column,
    const f32 b_opacity,
    const f32 b_red,
    const f32 b_green,
    const f32 b_blue,
    const f32 c_camera_row,
    const f32 c_camera_column,
    const f32 c_depth,
    const f32 c_texture_row,
    const f32 c_texture_column,
    const f32 c_opacity,
    const f32 c_red,
    const f32 c_green,
    const f32 c_blue)
{
  f32 vertices[] = {
      a_camera_row,     // 0
      a_camera_column,  // 1
      a_depth,          // 2
      a_texture_row,    // 3
      a_texture_column, // 4
      a_opacity,        // 5
      a_red,            // 6
      a_green,          // 7
      a_blue,           // 8
      b_camera_row,     // 9
      b_camera_column,  // 10
      b_depth,          // 11
      b_texture_row,    // 12
      b_texture_column, // 13
      b_opacity,        // 14
      b_red,            // 15
      b_green,          // 16
      b_blue,           // 17
      c_camera_row,     // 18
      c_camera_column,  // 19
      c_depth,          // 20
      c_texture_row,    // 21
      c_texture_column, // 22
      c_opacity,        // 23
      c_red,            // 24
      c_green,          // 25
      c_blue,           // 26
  };

  const f32 *top;
  const f32 *middle;
  const f32 *bottom;

  sort_top_to_bottom(vertices, 9, &top, &middle, &bottom);

  f32 deltas[18];
  subtract_f32s_f32s(bottom, top, deltas, 9);
  subtract_f32s_f32s(middle, top, &deltas[9], 9);

  f32 per_rows[17];
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], per_rows, 8);
  const f32 short_camera_row_delta = deltas[9];
  multiply_f32s_f32(&deltas[10], 1.0f / short_camera_row_delta, &per_rows[8], 8);

  f32 accumulators[17];

  const s32 top_camera_row = floor(top[0]);
  const s32 clamped_top_camera_row = MAX(0, top_camera_row);

  if (top_camera_row < 0)
  {
    const f32 skipped_rows = -top_camera_row;
    multiply_add_f32s_f32_f32s(per_rows, skipped_rows, &top[1], accumulators, 8);
    multiply_add_f32s_f32_f32s(&per_rows[8], skipped_rows, &top[1], &accumulators[8], 8);
  }
  else
  {
    copy_f32s(&top[1], accumulators, 8);
    copy_f32s(&top[1], &accumulators[8], 8);
  }

  const s32 middle_camera_row = floor(middle[0]);
  const s32 clamped_middle_camera_row = CLAMP(middle_camera_row, 0, camera_component_rows);

  for (s32 camera_row = clamped_top_camera_row; camera_row < clamped_middle_camera_row; camera_row++)
  {
    render_blended_row(
        texture_rows,
        texture_rows_minus_one,
        texture_columns,
        texture_columns_minus_one,
        texture_reds,
        texture_greens,
        texture_blues,
        texture_opacities,
        accumulators,
        camera_row);

    add_f32s_f32s(accumulators, per_rows, accumulators, 16);
  }

  subtract_f32s_f32s(bottom, middle, deltas, 9);
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], &per_rows[8], 8);

  if (middle_camera_row < 0)
  {
    const f32 skipped_rows = -middle_camera_row;
    multiply_add_f32s_f32_f32s(&per_rows[8], skipped_rows, &middle[1], &accumulators[8], 8);
  }
  else
  {
    copy_f32s(&middle[1], &accumulators[8], 8);
  }

  const s32 bottom_camera_row = *bottom;
  const s32 clamped_bottom_camera_row = MIN(camera_component_rows, bottom_camera_row);

  for (s32 camera_row = clamped_middle_camera_row; camera_row < clamped_bottom_camera_row; camera_row++)
  {
    render_blended_row(
        texture_rows,
        texture_rows_minus_one,
        texture_columns,
        texture_columns_minus_one,
        texture_reds,
        texture_greens,
        texture_blues,
        texture_opacities,
        accumulators,
        camera_row);

    add_f32s_f32s(accumulators, per_rows, accumulators, 16);
  }
}
