/** @file */

#ifndef MESH_H

#define MESH_H

#include "../primitives/f32.h"
#include "../primitives/s32.h"
#include "../math/vector.h"
#include "../video/color.h"
#include "image.h"

/**
 * An immutable triangulated mesh with texture coordinates and vertex colors.
 */
typedef struct
{
  /**
   * The number of vertices in the mesh.
   */
  const s32 vertices;

  /**
   * The locations of the vertices in the mesh.
   */
  const vector *const locations;

  /**
   * The number of textures in the mesh.
   */
  const s32 textures;

  /**
   * The image for each texture in the mesh.
   */
  const image_factory *const images;

  /**
   * The number of triangles for each texture in the mesh.  The triangles in the
   * following arrays are ordered to match, e.g. if texture A has 3 and texture
   * B has 2, the triangles are ordered AAABB.
   */
  const s32 *const triangles;

  /**
   * The location indices of the triangles in the mesh.  There are three for
   * each triangle.
   */
  const s32 *const indices;

  /**
   * The texture coordinates of the vertices of the triangles in the mesh.
   * There are six values for each triangle (two axes, row and column, for each
   * of the three vertices).
   */
  const f32 *const texture_coordinates;

  /**
   * The colors of the vertices of the triangles in the mesh.  There are three
   * for each triangle.
   */
  const color *const colors;
} mesh;

/**
 * Provides a mesh when executed.
 * @return The provided mesh.
 */
typedef const mesh *const(mesh_factory)();

#endif
