/** @file */

#ifndef MATERIAL_TYPE_H

#define MATERIAL_TYPE_H

#include "../primitives/s32.h"

/**
 * The type of a material.
 */
typedef s32 material_type;

/**
 * Solid geometry.  Should be used whenever possible.
 */
#define MATERIAL_TYPE_OPAQUE 0

/**
 * Similar to MATERIAL_TYPE_OPAQUE, but where any opacity values below 50% are
 * fully transparent, with a hard edge.  Useful for grates, fences, foliage,
 * etc.  Prefer over MATERIAL_TYPE_BLEND where possible.
 */
#define MATERIAL_TYPE_CUTOUT 1

/**
 * Blends between the surface and those behind it (similar to the "Normal" layer
 * blend mode in Adobe Photoshop).  May draw in the wrong order if layered with
 * MATERIAL_TYPE_BLEND or MATERIAL_TYPE_ADDITIVE.  Useful for smoke, glass, etc.
 */
#define MATERIAL_TYPE_BLEND 2

/**
 * Brightens the background (similar to the "Linear Dodge (Add)" layer blend
 * mode in Adobe Photoshop).  May draw in the wrong order if layered together
 * with MATERIAL_TYPE_BLEND, but will combine correctly with
 * MATERIAL_TYPE_OPAQUE, MATERIAL_TYPE_CUTOUT and/or MATERIAL_TYPE_ADDITIVE
 * materials.  Useful for lasers, lens flares, flames, explosions, muzzle
 * flashes, etc.
 */
#define MATERIAL_TYPE_ADDITIVE 3

#endif
