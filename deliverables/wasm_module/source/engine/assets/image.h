/** @file */

#ifndef IMAGE_H

#define IMAGE_H

#include "../primitives/s32.h"
#include "../video/color.h"
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
   * The colors within the image, running from left to right then top to bottom.
   */
  const color *const colors;
} image;

/**
 * Provides a image when executed.
 * @return The provided image.
 */
typedef const image *const(image_factory)();

#endif
