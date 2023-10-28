#include "../../primitives/f32.h"
#include "../../math/vector.h"
#include "../../../game/project_settings/audio_settings.h"

vector listener_location;

vector listener_normal = {0, 0, -1};

f32 audio_samples[SAMPLES_PER_TICK];
