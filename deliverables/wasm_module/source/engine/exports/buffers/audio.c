#include "../../primitives/f32.h"
#include "../../math/vector.h"
#include "../../../game/project_settings/audio_settings.h"
#include "../export.h"

vector listener_location ALIGN(f32);

vector listener_normal ALIGN(f32) = {0, 0, -1};

f32 audio_samples[SAMPLES_PER_TICK] ALIGN(f32);
