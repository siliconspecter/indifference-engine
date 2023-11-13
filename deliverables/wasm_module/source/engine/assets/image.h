/** @file */

#ifndef IMAGE_H

#define IMAGE_H

#include "../primitives/s32.h"
#include "../primitives/f32.h"
#include "image.h"

/**
 * An immutable 32-bits per pixel red-green-blue-opacity image.  Opacity is NOT
 * pre-multiplied.
 */
typedef struct
{
  /**
   * The height of the image, in pixel rows.
   */
  const s32 rows;

  /**
   * The width of the image, in pixel columns.
   */
  const s32 columns;

  /**
   * The intensity of the red channel of each pixel within the image, row-major.
   * @remark 0 is dark and 1 is bright.  Values may exceed this range.
   */
  const f32 *const reds;

  /**
   * The intensity of the green channel of each pixel within the image,
   * row-major.
   * @remark 0 is dark and 1 is bright.  Values may exceed this range.
   */
  const f32 *const greens;

  /**
   * The intensity of the blue channel of each pixel within the image,
   * row-major.
   * @remark 0 is dark and 1 is bright.  Values may exceed this range.
   */
  const f32 *const blues;

  /**
   * The opacity of each pixel within the image, row-major.
   * @remark 0 is transparent and 1 is opaque.  Behavior is undefined outside of
   *         this range.
   */
  const f32 *const opacities;
} image;

/**
 * Provides a image when executed.
 * @return The provided image.
 */
typedef const image *(image_factory)();

#endif
