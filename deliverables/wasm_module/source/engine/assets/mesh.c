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

void render_opaque_mesh(
    const mesh *const mesh,
    const matrix model_view_projection)
{
  const quantity vertices = mesh->vertices;

  if (vertices > MAXIMUM_VERTICES_PER_MESH)
  {
    throw(ERROR_MESH_HAS_TOO_MANY_VERTICES);
  }
  else
  {
    const vector *const model = mesh->locations;
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

    const quantity passes = mesh->passes;
    texture_factory *const *const textures = mesh->textures;
    const quantity *const triangles = mesh->triangles;
    const index *const indices = mesh->indices;
    const f32 *const texture_coordinates = mesh->texture_coordinates;
    const f32 *const reds = mesh->reds;
    const f32 *const greens = mesh->greens;
    const f32 *const blues = mesh->blues;
    const f32 *const opacities = mesh->opacities;

    // TODO: split texture coordinate into U and V

    index vertex = 0;

    for (index pass = 0; pass < passes; pass++)
    {
      const index end_vertex = vertex + triangles[pass] * 3;

      const texture *const texture = textures[pass]();
      const quantity texture_rows = texture->rows;
      const quantity texture_rows_minus_one = texture_rows - 1;
      const quantity texture_columns = texture->columns;
      const quantity texture_columns_minus_one = texture_columns - 1;
      const f32 *const texture_reds = texture->reds;
      const f32 *const texture_greens = texture->greens;
      const f32 *const texture_blues = texture->blues;
      const f32 *const texture_opacities = texture->opacities;

      while (vertex < end_vertex)
      {
        const index a_index = vertex++;
        const index vertex_index_a = indices[a_index] * 3; // TODO: pre-multiply?
        const f32 a_camera_column = clip[vertex_index_a];

        if (IS_NAN(a_camera_column))
        {
          vertex += 2;
          continue;
        }

        const index b_index = vertex++;
        const index vertex_index_b = indices[b_index] * 3;
        const f32 b_camera_column = clip[vertex_index_b];

        if (IS_NAN(b_camera_column))
        {
          vertex++;
          continue;
        }

        const index c_index = vertex++;
        const index vertex_index_c = indices[c_index] * 3;
        const f32 c_camera_column = clip[vertex_index_c];

        if (IS_NAN(c_camera_column))
        {
          continue;
        }

        const f32 a_camera_row = clip[vertex_index_a + 1];
        const f32 a_depth = clip[vertex_index_a + 2];
        const f32 a_texture_row = texture_coordinates[a_index * 2];
        const f32 a_texture_column = texture_coordinates[a_index * 2 + 1];
        const f32 a_red = reds[a_index];
        const f32 a_green = greens[a_index];
        const f32 a_blue = blues[a_index];
        const f32 a_opacity = opacities[a_index];

        const f32 b_camera_row = clip[vertex_index_b + 1];
        const f32 b_depth = clip[vertex_index_b + 2];
        const f32 b_texture_row = texture_coordinates[b_index * 2];
        const f32 b_texture_column = texture_coordinates[b_index * 2 + 1];
        const f32 b_red = reds[b_index];
        const f32 b_green = greens[b_index];
        const f32 b_blue = blues[b_index];
        const f32 b_opacity = opacities[b_index];

        const f32 c_camera_row = clip[vertex_index_c + 1];
        const f32 c_depth = clip[vertex_index_c + 2];
        const f32 c_texture_row = texture_coordinates[c_index * 2];
        const f32 c_texture_column = texture_coordinates[c_index * 2 + 1];
        const f32 c_red = reds[c_index];
        const f32 c_green = greens[c_index];
        const f32 c_blue = blues[c_index];
        const f32 c_opacity = opacities[c_index];

        render_opaque_triangle(
            texture_rows,
            texture_rows_minus_one,
            texture_columns,
            texture_columns_minus_one,
            texture_reds,
            texture_greens,
            texture_blues,
            texture_opacities,
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
