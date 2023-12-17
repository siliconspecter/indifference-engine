#include "mesh.h"
#include "texture.h"
#include "../math/matrix.h"
#include "../video/triangle.h"
#include "../primitives/quantity.h"
#include "../primitives/f32.h"
#include "../math/float.h"
#include "../../game/project_settings/limits.h"
#include "../scenes/components/camera_component.h"
#include "../exports/buffers/error.h"

static f32 clip[MAXIMUM_VERTICES_PER_MESH * 3];

void render_opaque_cutout_mesh(
    const mesh *const mesh,
    const matrix model_view_projection)
{
  const quantity vertices = mesh->opaque_cutout_vertices;

  if (vertices > MAXIMUM_VERTICES_PER_MESH)
  {
    throw(ERROR_MESH_HAS_TOO_MANY_VERTICES);
  }
  else
  {
    const vector *const model = mesh->opaque_cutout_locations;
    index clip_index = 0;

    for (index vertex = 0; vertex < vertices; vertex++)
    {
      vector temp;

      if (model_to_clip(model[vertex], model_view_projection, temp))
      {
        clip[clip_index += 3] = NAN;
      }
      else
      {
        clip[clip_index++] = temp[0] * camera_component_clip_to_video_column_coefficient + camera_component_clip_to_video_column_offset;
        clip[clip_index++] = temp[1] * camera_component_clip_to_video_row_coefficient + camera_component_clip_to_video_row_offset;
        clip[clip_index++] = temp[2];
      }
    }

    const quantity opaque_passes = mesh->opaque_passes;
    texture_factory *const *const opaque_textures = mesh->opaque_textures;
    const quantity *const opaque_triangles = mesh->opaque_triangles;
    const index *const opaque_indices = mesh->opaque_indices;
    const f32 *const opaque_rows = mesh->opaque_rows;
    const f32 *const opaque_columns = mesh->opaque_columns;
    const f32 *const opaque_reds = mesh->opaque_reds;
    const f32 *const opaque_greens = mesh->opaque_greens;
    const f32 *const opaque_blues = mesh->opaque_blues;

    index opaque_vertex = 0;

    for (index pass = 0; pass < opaque_passes; pass++)
    {
      const index end_vertex = opaque_vertex + opaque_triangles[pass] * 3; // todo: pre-multiply?

      const texture *const texture = opaque_textures[pass]();
      const quantity texture_rows = texture->rows;
      const quantity texture_rows_minus_one = texture_rows - 1;
      const quantity texture_columns = texture->columns;
      const quantity texture_columns_minus_one = texture_columns - 1;
      const f32 *const texture_reds = texture->reds;
      const f32 *const texture_greens = texture->greens;
      const f32 *const texture_blues = texture->blues;

      while (opaque_vertex < end_vertex)
      {
        const index a_index = opaque_vertex++;
        const index vertex_index_a = opaque_indices[a_index] * 3; // TODO: pre-multiply?
        const f32 a_camera_column = clip[vertex_index_a];

        if (IS_NAN(a_camera_column))
        {
          opaque_vertex += 2;
          continue;
        }

        const index b_index = opaque_vertex++;
        const index vertex_index_b = opaque_indices[b_index] * 3;
        const f32 b_camera_column = clip[vertex_index_b];

        if (IS_NAN(b_camera_column))
        {
          opaque_vertex++;
          continue;
        }

        const index c_index = opaque_vertex++;
        const index vertex_index_c = opaque_indices[c_index] * 3;
        const f32 c_camera_column = clip[vertex_index_c];

        if (IS_NAN(c_camera_column))
        {
          continue;
        }

        const f32 a_camera_row = clip[vertex_index_a + 1];
        const f32 a_depth = clip[vertex_index_a + 2];
        const f32 a_texture_row = opaque_rows[a_index];
        const f32 a_texture_column = opaque_columns[a_index];
        const f32 a_red = opaque_reds[a_index];
        const f32 a_green = opaque_greens[a_index];
        const f32 a_blue = opaque_blues[a_index];

        const f32 b_camera_row = clip[vertex_index_b + 1];
        const f32 b_depth = clip[vertex_index_b + 2];
        const f32 b_texture_row = opaque_rows[b_index];
        const f32 b_texture_column = opaque_columns[b_index];
        const f32 b_red = opaque_reds[b_index];
        const f32 b_green = opaque_greens[b_index];
        const f32 b_blue = opaque_blues[b_index];

        const f32 c_camera_row = clip[vertex_index_c + 1];
        const f32 c_depth = clip[vertex_index_c + 2];
        const f32 c_texture_row = opaque_rows[c_index];
        const f32 c_texture_column = opaque_columns[c_index];
        const f32 c_red = opaque_reds[c_index];
        const f32 c_green = opaque_greens[c_index];
        const f32 c_blue = opaque_blues[c_index];

        render_opaque_triangle(
            texture_rows,
            texture_rows_minus_one,
            texture_columns,
            texture_columns_minus_one,
            texture_reds,
            texture_greens,
            texture_blues,
            a_camera_row,
            a_camera_column,
            a_depth,
            a_texture_row,
            a_texture_column,
            a_red,
            a_green,
            a_blue,
            b_camera_row,
            b_camera_column,
            b_depth,
            b_texture_row,
            b_texture_column,
            b_red,
            b_green,
            b_blue,
            c_camera_row,
            c_camera_column,
            c_depth,
            c_texture_row,
            c_texture_column,
            c_red,
            c_green,
            c_blue);
      }
    }

    const quantity cutout_passes = mesh->cutout_passes;
    texture_factory *const *const cutout_textures = mesh->cutout_textures;
    const quantity *const cutout_triangles = mesh->cutout_triangles;
    const index *const cutout_indices = mesh->cutout_indices;
    const f32 *const cutout_rows = mesh->cutout_rows;
    const f32 *const cutout_columns = mesh->cutout_columns;
    const f32 *const cutout_opacities = mesh->cutout_opacities;
    const f32 *const cutout_reds = mesh->cutout_reds;
    const f32 *const cutout_greens = mesh->cutout_greens;
    const f32 *const cutout_blues = mesh->cutout_blues;

    index cutout_vertex = 0;

    for (index pass = 0; pass < cutout_passes; pass++)
    {
      const index end_vertex = cutout_vertex + cutout_triangles[pass] * 3;

      const texture *const texture = cutout_textures[pass]();
      const quantity texture_rows = texture->rows;
      const quantity texture_rows_minus_one = texture_rows - 1;
      const quantity texture_columns = texture->columns;
      const quantity texture_columns_minus_one = texture_columns - 1;
      const f32 *const texture_opacities = texture->opacities;
      const f32 *const texture_reds = texture->reds;
      const f32 *const texture_greens = texture->greens;
      const f32 *const texture_blues = texture->blues;

      while (cutout_vertex < end_vertex)
      {
        const index a_index = cutout_vertex++;
        const index vertex_index_a = cutout_indices[a_index] * 3; // TODO: pre-multiply?
        const f32 a_camera_column = clip[vertex_index_a];

        if (IS_NAN(a_camera_column))
        {
          cutout_vertex += 2;
          continue;
        }

        const index b_index = cutout_vertex++;
        const index vertex_index_b = cutout_indices[b_index] * 3;
        const f32 b_camera_column = clip[vertex_index_b];

        if (IS_NAN(b_camera_column))
        {
          cutout_vertex++;
          continue;
        }

        const index c_index = cutout_vertex++;
        const index vertex_index_c = cutout_indices[c_index] * 3;
        const f32 c_camera_column = clip[vertex_index_c];

        if (IS_NAN(c_camera_column))
        {
          continue;
        }

        const f32 a_camera_row = clip[vertex_index_a + 1];
        const f32 a_depth = clip[vertex_index_a + 2];
        const f32 a_texture_row = cutout_rows[a_index];
        const f32 a_texture_column = cutout_columns[a_index];
        const f32 a_opacity = cutout_opacities[a_index];
        const f32 a_red = cutout_reds[a_index];
        const f32 a_green = cutout_greens[a_index];
        const f32 a_blue = cutout_blues[a_index];

        const f32 b_camera_row = clip[vertex_index_b + 1];
        const f32 b_depth = clip[vertex_index_b + 2];
        const f32 b_texture_row = cutout_rows[b_index];
        const f32 b_texture_column = cutout_columns[b_index];
        const f32 b_opacity = cutout_opacities[b_index];
        const f32 b_red = cutout_reds[b_index];
        const f32 b_green = cutout_greens[b_index];
        const f32 b_blue = cutout_blues[b_index];

        const f32 c_camera_row = clip[vertex_index_c + 1];
        const f32 c_depth = clip[vertex_index_c + 2];
        const f32 c_texture_row = cutout_rows[c_index];
        const f32 c_texture_column = cutout_columns[c_index];
        const f32 c_opacity = cutout_opacities[c_index];
        const f32 c_red = cutout_reds[c_index];
        const f32 c_green = cutout_greens[c_index];
        const f32 c_blue = cutout_blues[c_index];

        render_cutout_triangle(
            texture_rows,
            texture_rows_minus_one,
            texture_columns,
            texture_columns_minus_one,
            texture_opacities,
            texture_reds,
            texture_greens,
            texture_blues,
            a_camera_row,
            a_camera_column,
            a_depth,
            a_texture_row,
            a_texture_column,
            a_opacity,
            a_red,
            a_green,
            a_blue,
            b_camera_row,
            b_camera_column,
            b_depth,
            b_texture_row,
            b_texture_column,
            b_opacity,
            b_red,
            b_green,
            b_blue,
            c_camera_row,
            c_camera_column,
            c_depth,
            c_texture_row,
            c_texture_column,
            c_opacity,
            c_red,
            c_green,
            c_blue);
      }
    }
  }
}

void render_additive_blended_mesh(
    const mesh *const mesh,
    const matrix model_view_projection)
{
  const quantity vertices = mesh->additive_blended_vertices;

  if (vertices > MAXIMUM_VERTICES_PER_MESH)
  {
    throw(ERROR_MESH_HAS_TOO_MANY_VERTICES);
  }
  else
  {
    const vector *const model = mesh->additive_blended_locations;
    index clip_index = 0;

    for (index vertex = 0; vertex < vertices; vertex++)
    {
      vector temp;

      if (model_to_clip(model[vertex], model_view_projection, temp))
      {
        clip[clip_index += 3] = NAN;
      }
      else
      {
        clip[clip_index++] = temp[0] * camera_component_clip_to_video_column_coefficient + camera_component_clip_to_video_column_offset;
        clip[clip_index++] = temp[1] * camera_component_clip_to_video_row_coefficient + camera_component_clip_to_video_row_offset;
        clip[clip_index++] = temp[2];
      }
    }

    const quantity additive_passes = mesh->additive_passes;
    texture_factory *const *const additive_textures = mesh->additive_textures;
    const quantity *const additive_triangles = mesh->additive_triangles;
    const index *const additive_indices = mesh->additive_indices;
    const f32 *const additive_rows = mesh->additive_rows;
    const f32 *const additive_columns = mesh->additive_columns;
    const f32 *const additive_reds = mesh->additive_reds;
    const f32 *const additive_greens = mesh->additive_greens;
    const f32 *const additive_blues = mesh->additive_blues;

    index additive_vertex = 0;

    for (index pass = 0; pass < additive_passes; pass++)
    {
      const index end_vertex = additive_vertex + additive_triangles[pass] * 3;

      const texture *const texture = additive_textures[pass]();
      const quantity texture_rows = texture->rows;
      const quantity texture_rows_minus_one = texture_rows - 1;
      const quantity texture_columns = texture->columns;
      const quantity texture_columns_minus_one = texture_columns - 1;
      const f32 *const texture_reds = texture->reds;
      const f32 *const texture_greens = texture->greens;
      const f32 *const texture_blues = texture->blues;

      while (additive_vertex < end_vertex)
      {
        const index a_index = additive_vertex++;
        const index vertex_index_a = additive_indices[a_index] * 3; // TODO: pre-multiply?
        const f32 a_camera_column = clip[vertex_index_a];

        if (IS_NAN(a_camera_column))
        {
          additive_vertex += 2;
          continue;
        }

        const index b_index = additive_vertex++;
        const index vertex_index_b = additive_indices[b_index] * 3;
        const f32 b_camera_column = clip[vertex_index_b];

        if (IS_NAN(b_camera_column))
        {
          additive_vertex++;
          continue;
        }

        const index c_index = additive_vertex++;
        const index vertex_index_c = additive_indices[c_index] * 3;
        const f32 c_camera_column = clip[vertex_index_c];

        if (IS_NAN(c_camera_column))
        {
          continue;
        }

        const f32 a_camera_row = clip[vertex_index_a + 1];
        const f32 a_depth = clip[vertex_index_a + 2];
        const f32 a_texture_row = additive_rows[a_index];
        const f32 a_texture_column = additive_columns[a_index];
        const f32 a_red = additive_reds[a_index];
        const f32 a_green = additive_greens[a_index];
        const f32 a_blue = additive_blues[a_index];

        const f32 b_camera_row = clip[vertex_index_b + 1];
        const f32 b_depth = clip[vertex_index_b + 2];
        const f32 b_texture_row = additive_rows[b_index];
        const f32 b_texture_column = additive_columns[b_index];
        const f32 b_red = additive_reds[b_index];
        const f32 b_green = additive_greens[b_index];
        const f32 b_blue = additive_blues[b_index];

        const f32 c_camera_row = clip[vertex_index_c + 1];
        const f32 c_depth = clip[vertex_index_c + 2];
        const f32 c_texture_row = additive_rows[c_index];
        const f32 c_texture_column = additive_columns[c_index];
        const f32 c_red = additive_reds[c_index];
        const f32 c_green = additive_greens[c_index];
        const f32 c_blue = additive_blues[c_index];

        render_additive_triangle(
            texture_rows,
            texture_rows_minus_one,
            texture_columns,
            texture_columns_minus_one,
            texture_reds,
            texture_greens,
            texture_blues,
            a_camera_row,
            a_camera_column,
            a_depth,
            a_texture_row,
            a_texture_column,
            a_red,
            a_green,
            a_blue,
            b_camera_row,
            b_camera_column,
            b_depth,
            b_texture_row,
            b_texture_column,
            b_red,
            b_green,
            b_blue,
            c_camera_row,
            c_camera_column,
            c_depth,
            c_texture_row,
            c_texture_column,
            c_red,
            c_green,
            c_blue);
      }
    }

    const quantity blended_passes = mesh->blended_passes;
    texture_factory *const *const blended_textures = mesh->blended_textures;
    const quantity *const blended_triangles = mesh->blended_triangles;
    const index *const blended_indices = mesh->blended_indices;
    const f32 *const blended_rows = mesh->blended_rows;
    const f32 *const blended_columns = mesh->blended_columns;
    const f32 *const blended_opacities = mesh->blended_opacities;
    const f32 *const blended_reds = mesh->blended_reds;
    const f32 *const blended_greens = mesh->blended_greens;
    const f32 *const blended_blues = mesh->blended_blues;

    index blended_vertex = 0;

    for (index pass = 0; pass < blended_passes; pass++)
    {
      const index end_vertex = blended_vertex + blended_triangles[pass] * 3;

      const texture *const texture = blended_textures[pass]();
      const quantity texture_rows = texture->rows;
      const quantity texture_rows_minus_one = texture_rows - 1;
      const quantity texture_columns = texture->columns;
      const quantity texture_columns_minus_one = texture_columns - 1;
      const f32 *const texture_opacities = texture->opacities;
      const f32 *const texture_reds = texture->reds;
      const f32 *const texture_greens = texture->greens;
      const f32 *const texture_blues = texture->blues;

      while (blended_vertex < end_vertex)
      {
        const index a_index = blended_vertex++;
        const index vertex_index_a = blended_indices[a_index] * 3; // TODO: pre-multiply?
        const f32 a_camera_column = clip[vertex_index_a];

        if (IS_NAN(a_camera_column))
        {
          blended_vertex += 2;
          continue;
        }

        const index b_index = blended_vertex++;
        const index vertex_index_b = blended_indices[b_index] * 3;
        const f32 b_camera_column = clip[vertex_index_b];

        if (IS_NAN(b_camera_column))
        {
          blended_vertex++;
          continue;
        }

        const index c_index = blended_vertex++;
        const index vertex_index_c = blended_indices[c_index] * 3;
        const f32 c_camera_column = clip[vertex_index_c];

        if (IS_NAN(c_camera_column))
        {
          continue;
        }

        const f32 a_camera_row = clip[vertex_index_a + 1];
        const f32 a_depth = clip[vertex_index_a + 2];
        const f32 a_texture_row = blended_rows[a_index];
        const f32 a_texture_column = blended_columns[a_index];
        const f32 a_opacity = blended_opacities[a_index];
        const f32 a_red = blended_reds[a_index];
        const f32 a_green = blended_greens[a_index];
        const f32 a_blue = blended_blues[a_index];

        const f32 b_camera_row = clip[vertex_index_b + 1];
        const f32 b_depth = clip[vertex_index_b + 2];
        const f32 b_texture_row = blended_rows[b_index];
        const f32 b_texture_column = blended_columns[b_index];
        const f32 b_opacity = blended_opacities[b_index];
        const f32 b_red = blended_reds[b_index];
        const f32 b_green = blended_greens[b_index];
        const f32 b_blue = blended_blues[b_index];

        const f32 c_camera_row = clip[vertex_index_c + 1];
        const f32 c_depth = clip[vertex_index_c + 2];
        const f32 c_texture_row = blended_rows[c_index];
        const f32 c_texture_column = blended_columns[c_index];
        const f32 c_opacity = blended_opacities[c_index];
        const f32 c_red = blended_reds[c_index];
        const f32 c_green = blended_greens[c_index];
        const f32 c_blue = blended_blues[c_index];

        render_blended_triangle(
            texture_rows,
            texture_rows_minus_one,
            texture_columns,
            texture_columns_minus_one,
            texture_opacities,
            texture_reds,
            texture_greens,
            texture_blues,
            a_camera_row,
            a_camera_column,
            a_depth,
            a_texture_row,
            a_texture_column,
            a_opacity,
            a_red,
            a_green,
            a_blue,
            b_camera_row,
            b_camera_column,
            b_depth,
            b_texture_row,
            b_texture_column,
            b_opacity,
            b_red,
            b_green,
            b_blue,
            c_camera_row,
            c_camera_column,
            c_depth,
            c_texture_row,
            c_texture_column,
            c_opacity,
            c_red,
            c_green,
            c_blue);
      }
    }
  }
}
