/** @file */

#ifndef SOUND_H

#define SOUND_H

#include "../primitives/f32.h"
#include "../primitives/quantity.h"
#include "../primitives/index.h"

/**
 * An immutable 32-bit float monoaural sound.
 */
typedef struct
{
  /**
   * The sound's sample rate, in samples per second.
   */
  const f32 samples_per_second;

  /**
   * The length of the sound, in samples.
   */
  const quantity length;

  /**
   * When the sound plays to the end, this specifies the index of the sample to
   * return to in order to loop.  The sound will NOT loop if set to
   * @ref INDEX_NONE.
   */
  const index loop_from;

  /**
   * The samples of the sound, running from earlier to later, where each sample
   * ranges from -1 to 1.
   */
  const f32 *const samples;
} sound;

#endif
