/** @file */

#ifndef AUDIO_H

#define AUDIO_H

#include "../../primitives/f32.h"
#include "../../math/vector.h"
#include "../../../game/project_settings/audio_settings.h"

/**
 * The position of the listener on the X, Y and Z axes in world space, in
 * meters.
 */
extern vector listener_location;

/**
 * A unit vector pointing in the direction of the listener on the X, Y and Z axes in world space.
 */
extern vector listener_normal;

/**
 * The audio output buffer, running from earlier to later, where each sample
 * ranges from -1 to 1.
 */
extern f32 audio_samples[SAMPLES_PER_TICK];

#endif
