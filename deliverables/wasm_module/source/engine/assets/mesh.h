/** @file */

#ifndef MESH_H

#define MESH_H

#include "../primitives/f32.h"
#include "../primitives/index.h"
#include "../primitives/quantity.h"
#include "../math/vector.h"
#include "../math/matrix.h"
#include "texture.h"

/**
 * An immutable triangulated mesh with texture coordinates and vertex colors.
 */
typedef struct
{
  /**
   * The number of vertices for opaque and cutout geometry in the mesh.
   */
  const quantity opaque_cutout_vertices;

  /**
   * The locations of the vertices of opaque and cutout geometry in the mesh.
   */
  const vector *const opaque_cutout_locations;
  // TODO: Flatten

  /**
   * The number of opaque render passes in the mesh.
   */
  const quantity opaque_passes;

  /**
   * The texture of each opaque render pass in the mesh.
   */
  texture_factory *const *const opaque_textures;

  /**
   * The number of triangles for each opaque render pass in the mesh.  The
   * triangles in the following arrays are ordered to match, e.g. if render pass
   * A has 3 and render pass B has 2, the triangles are ordered AAABB.
   */
  const quantity *const opaque_triangles;

  /**
   * The location indices of the opaque triangles in the mesh.  There are three
   * for each triangle.
   */
  const index *const opaque_indices;

  /**
   * The number of rows between the top of the texture and the point to sample
   * of each vertex of each opaque triangle within the mesh.
   */
  const f32 *const opaque_rows;

  /**
   * The number of columns between the top of the texture and the point to
   * sample of each vertex of each opaque triangle within the mesh.
   */
  const f32 *const opaque_columns;

  /**
   * The intensity of the red channel of each vertex of each opaque triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const opaque_reds;

  /**
   * The intensity of the green channel of each vertex of each opaque triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const opaque_greens;

  /**
   * The intensity of the blue channel of each vertex of each opaque triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const opaque_blues;

  /**
   * The number of cutout render passes in the mesh.
   */
  const quantity cutout_passes;

  /**
   * The texture of each cutout render pass in the mesh.
   */
  texture_factory *const *const cutout_textures;

  /**
   * The number of triangles for each cutout render pass in the mesh.  The
   * triangles in the following arrays are ordered to match, e.g. if render pass
   * A has 3 and render pass B has 2, the triangles are ordered AAABB.
   */
  const quantity *const cutout_triangles;

  /**
   * The location indices of the cutout triangles in the mesh.  There are three
   * for each triangle.
   */
  const index *const cutout_indices;

  /**
   * The number of rows between the top of the texture and the point to sample
   * of each vertex of each cutout triangle within the mesh.
   */
  const f32 *const cutout_rows;

  /**
   * The number of columns between the top of the texture and the point to
   * sample of each vertex of each cutout triangle within the mesh.
   */
  const f32 *const cutout_columns;

  /**
   * The opacity of each vertex of each cutout triangle within the mesh.
   * @remark 0 is transparent and 1 is opaque.  Behavior is undefined outside of
   *         this range.
   */
  const f32 *const cutout_opacities;

  /**
   * The intensity of the red channel of each vertex of each cutout triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const cutout_reds;

  /**
   * The intensity of the green channel of each vertex of each cutout triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const cutout_greens;

  /**
   * The intensity of the blue channel of each vertex of each cutout triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const cutout_blues;

  /**
   * The number of vertices for additive and blended geometry in the mesh.
   */
  const quantity additive_blended_vertices;

  /**
   * The locations of the vertices of additive and blended geometry in the mesh.
   */
  const vector *const additive_blended_locations;

  /**
   * The number of additive render passes in the mesh.
   */
  const quantity additive_passes;

  /**
   * The texture of each additive render pass in the mesh.
   */
  texture_factory *const *const additive_textures;

  /**
   * The number of triangles for each additive render pass in the mesh.  The
   * triangles in the following arrays are ordered to match, e.g. if render pass
   * A has 3 and render pass B has 2, the triangles are ordered AAABB.
   */
  const quantity *const additive_triangles;

  /**
   * The location indices of the additive triangles in the mesh.  There are
   * three for each triangle.
   */
  const index *const additive_indices;

  /**
   * The number of rows between the top of the texture and the point to sample
   * of each vertex of each additive triangle within the mesh.
   */
  const f32 *const additive_rows;

  /**
   * The number of columns between the top of the texture and the point to
   * sample of each vertex of each additive triangle within the mesh.
   */
  const f32 *const additive_columns;

  /**
   * The intensity of the red channel of each vertex of each additive triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const additive_reds;

  /**
   * The intensity of the green channel of each vertex of each additive triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const additive_greens;

  /**
   * The intensity of the blue channel of each vertex of each additive triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const additive_blues;

  /**
   * The number of blended render passes in the mesh.
   */
  const quantity blended_passes;

  /**
   * The texture of each blended render pass in the mesh.
   */
  texture_factory *const *const blended_textures;

  /**
   * The number of triangles for each blended render pass in the mesh.  The
   * triangles in the following arrays are ordered to match, e.g. if render pass
   * A has 3 and render pass B has 2, the triangles are ordered AAABB.
   */
  const quantity *const blended_triangles;

  /**
   * The location indices of the blended triangles in the mesh.  There are three
   * for each triangle.
   */
  const index *const blended_indices;

  /**
   * The number of rows between the top of the texture and the point to sample
   * of each vertex of each blended triangle within the mesh.
   */
  const f32 *const blended_rows;

  /**
   * The number of columns between the top of the texture and the point to
   * sample of each vertex of each blended triangle within the mesh.
   */
  const f32 *const blended_columns;

  /**
   * The opacity of each vertex of each blended triangle within the mesh.
   * @remark 0 is transparent and 1 is opaque.  Behavior is undefined outside of
   *         this range.
   */
  const f32 *const blended_opacities;

  /**
   * The intensity of the red channel of each vertex of each blended triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const blended_reds;

  /**
   * The intensity of the green channel of each vertex of each blended triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const blended_greens;

  /**
   * The intensity of the blue channel of each vertex of each blended triangle
   * within the mesh, in candela per square meter.
   */
  const f32 *const blended_blues;
} mesh;

/**
 * Provides a mesh when executed.
 * @return The provided mesh.
 */
typedef const mesh *(mesh_factory)();

#ifndef DOXYGEN_IGNORE

/**
 * Draws the opaque and cutout passes of a mesh in the current camera
 * component's viewport.
 * @remark Only use when rendering video for a camera component.
 * @param mesh The mesh to render.
 * @param model_view_projection The model-view-projection matrix to apply.
 */
void render_opaque_cutout_mesh(
    const mesh *const mesh,
    const matrix model_view_projection);

/**
 * Draws the additive and blended passes of a mesh in the current camera
 * component's viewport.
 * @remark Only use when rendering video for a camera component.
 * @param mesh The mesh to render.
 * @param model_view_projection The model-view-projection matrix to apply.
 */
void render_additive_blended_mesh(
    const mesh *const mesh,
    const matrix model_view_projection);

#endif

#endif
