/** @file */

#ifndef TEXTURE_H

#define TEXTURE_H

#include "../primitives/quantity.h"
#include "../primitives/f32.h"
#include "texture.h"

/**
 * An immutable 128-bits per pixel red-green-blue-opacity texture.  Opacity is
 * NOT pre-multiplied.
 */
typedef struct
{
  /**
   * The height of the texture, in pixel rows.
   */
  const quantity rows;

  /**
   * The width of the texture, in pixel columns.
   */
  const quantity columns;

  /**
   * The intensity of the red channel of each pixel within the texture,
   * row-major.
   * @remark 0 is dark and 1 is bright.  Values may exceed this range.
   */
  const f32 *const reds;

  /**
   * The intensity of the green channel of each pixel within the texture,
   * row-major.
   * @remark 0 is dark and 1 is bright.  Values may exceed this range.
   */
  const f32 *const greens;

  /**
   * The intensity of the blue channel of each pixel within the texture,
   * row-major.
   * @remark 0 is dark and 1 is bright.  Values may exceed this range.
   */
  const f32 *const blues;

  /**
   * The opacity of each pixel within the texture, row-major.
   * @remark 0 is transparent and 1 is opaque.  Behavior is undefined outside of
   *         this range.
   */
  const f32 *const opacities;
} texture;

/**
 * Provides a texture when executed.
 * @return The provided texture.
 */
typedef const texture *(texture_factory)();

#endif
