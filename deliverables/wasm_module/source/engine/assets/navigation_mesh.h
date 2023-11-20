/** @file */

#include "../primitives/f32.h"
#include "../primitives/index.h"
#include "../primitives/quantity.h"

/**
 * An immutable mesh used for navigation.
 * @remark Each face within has a convex and planar surface, with an infinitely
 *         tall prism extending away from that surface.  The walls of said prism
 *         represent planes which can be crossed to enter neighboring faces.
 */
typedef struct
{
  /**
   * The number of faces within the navigation mesh.
   */
  const quantity count;

  /**
   * For each face, the number of vertices within.
   */
  const quantity *const vertex_counts;

  /**
   * For each face, for each edge within, the index of the face found by
   * crossing it, or INDEX_NONE if the edge represents a wall.
   */
  const index *const *const neighbor_faces;

  /**
   * For each face, for each edge within, the index of the edge found by
   * crossing it, or INDEX_NONE if the edge represents a wall.
   */
  const index *const *const neighbor_edges;

  /**
   * For each face, for each vertex within, a three-dimensional vector
   * representing the location.
   */
  const f32 *const *const vertex_locations;

  /**
   * For each face, a three-dimensional unit vector representing the surface
   * normal.
   */
  const f32 *const surface_normals;

  /**
   * For each face, for each edge within, a three-dimensional unit vector
   * representing the normal of the plane which must be crossed to exit over
   * that edge.
   */
  const f32 *const *const edge_exit_normals;

  /**
   * For each face, for each vertex within, a three-dimensional unit vector
   * pointing from the surface of the face along the line of intersection
   * between the vertex's two neighboring edges' exit planes.
   */
  const f32 *const *const edge_exit_intersection_normals;

  /**
   * For each face, for each edge within, a three-dimensional vector pointing
   * from the first to the second vertex, of magnitude 1 / (the length of the
   * edge).
   */
  const f32 *const *const edge_coefficients;

  /**
   * For each face, for each edge within, for each edge of the face, the
   * distance between those two edges' midpoints.
   */
  const f32 *const *const edge_midpoint_distances;

  /**
   * For each face, for each edge within, the distance between that midpoint of
   * that edge and the average of the face's vertices.
   */
  const f32 *const *const edge_midpoint_center_distances;
} navigation_mesh;

/**
 * Provides a navigation mesh when executed.
 * @return The provided navigation mesh.
 */
typedef const navigation_mesh *(navigation_mesh_factory)();
