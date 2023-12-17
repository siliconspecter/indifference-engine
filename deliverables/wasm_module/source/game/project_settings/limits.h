/** @file */

#ifndef LIMITS_H

#define LIMITS_H

/**
 * The maximum number of entities which may exist at any given time.
 */
#define MAXIMUM_ENTITIES 16

/**
 * The maximum number of components which may exist at any given time.
 */
#define MAXIMUM_COMPONENTS 16

/**
 * The maximum number of timer components which may exist at any given time.
 */
#define MAXIMUM_TIMER_COMPONENTS 8

/**
 * The maximum number of sound components which may exist at any given time.
 */
#define MAXIMUM_SOUND_COMPONENTS 8

/**
 * The maximum number of camera components which may exist at any given time.
 */
#define MAXIMUM_CAMERA_COMPONENTS 4

/**
 * The maximum number of mesh components which may exist at any given time.
 */
#define MAXIMUM_MESH_COMPONENTS 8

/**
 * The maximum number of vertices which may exist in a mesh.
 */
#define MAXIMUM_VERTICES_PER_MESH 4096

/**
 * The maximum number of mesh components with opaque and/or cutout geometry
 * which may exist at any given time.
 */
#define MAXIMUM_OPAQUE_CUTOUT_MESH_COMPONENTS 8

/**
 * The maximum number of mesh components with additive and/or blended geometry
 * which may exist at any given time.
 */
#define MAXIMUM_ADDITIVE_BLENDED_MESH_COMPONENTS 8

#endif
