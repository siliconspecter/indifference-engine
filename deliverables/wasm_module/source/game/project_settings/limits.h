/** @file */

#ifndef LIMITS_H

#define LIMITS_H

/**
 * The maximum number of entities which may exist at any given time.
 * @remark The structure a component_handle may need to be adjusted if this is
 *         set to extreme values.
 */
#define MAXIMUM_ENTITIES 16

/**
 * The maximum number of components which may exist at any given time.
 * @remark The structure a component_handle may need to be adjusted if this is
 *         set to extreme values.
 */
#define MAXIMUM_COMPONENTS 16

/**
 * The maximum number of timer components which may exist at any given time.
 * @remark The structure a component_handle may need to be adjusted if this is
 *         set to extreme values.
 */
#define MAXIMUM_TIMER_COMPONENTS 8

/**
 * The maximum number of sound components which may exist at any given time.
 * @remark The structure a component_handle may need to be adjusted if this is
 *         set to extreme values.
 */
#define MAXIMUM_SOUND_COMPONENTS 8

/**
 * The maximum number of camera components which may exist at any given time.
 * @remark The structure a component_handle may need to be adjusted if this is
 *         set to extreme values.
 */
#define MAXIMUM_CAMERA_COMPONENTS 4

/**
 * The maximum number of opaque mesh components which may exist at any given
 * @remark The structure a component_handle may need to be adjusted if this is
 *         set to extreme values.
 * time.
 */
#define MAXIMUM_OPAQUE_MESH_COMPONENTS 8

#endif
