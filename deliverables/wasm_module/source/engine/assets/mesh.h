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
   * The number of vertices in the mesh.
   */
  const quantity vertices;

  /**
   * The locations of the vertices in the mesh.
   */
  const vector *const locations;

  /**
   * The number of render passes in the mesh.
   */
  const quantity passes;

  /**
   * The texture of each render pass in the mesh.
   */
  texture_factory *const *const textures;

  /**
   * The number of triangles for each render pass in the mesh.  The triangles in
   * the following arrays are ordered to match, e.g. if render pass A has 3 and
   * render pass B has 2, the triangles are ordered AAABB.
   */
  const quantity *const triangles;

  /**
   * The location indices of the triangles in the mesh.  There are three for
   * each triangle.
   */
  const index *const indices;

  /**
   * The texture coordinates of the vertices of the triangles in the mesh.
   * There are six values for each triangle (two axes, row and column, for each
   * of the three vertices).
   */
  const f32 *const texture_coordinates;

  /**
   * The intensity of the red channel of each vertex of each triangle within the
   * mesh, in candela per square meter.
   */
  const f32 *const reds;

  /**
   * The intensity of the green channel of each vertex of each triangle within
   * the mesh, in candela per square meter.
   */
  const f32 *const greens;

  /**
   * The intensity of the blue channel of each vertex of each triangle within
   * the mesh, in candela per square meter.
   */
  const f32 *const blues;

  /**
   * The opacity of each vertex of each triangle within the mesh.
   * @remark 0 is transparent and 1 is opaque.  Behavior is undefined outside of
   *         this range.
   */
  const f32 *const opacities;
} mesh;

/**
 * Provides a mesh when executed.
 * @return The provided mesh.
 */
typedef const mesh *(mesh_factory)();

#ifndef DOXYGEN_IGNORE

/**
 * Draws an opaque mesh in the current camera component's viewport.
 * @remark Only use when rendering video for a camera component.
 * @param mesh The mesh to render.
 * @param model_view_projection The model-view-projection matrix to apply.
 */
void render_opaque_mesh(
    const mesh *const mesh,
    const matrix model_view_projection);

#endif

#endif
