#include "../../primitives/f32.h"
#include "../../math/vector.h"
#include "../../../game/project_settings/audio_settings.h"

vector listener_location;

vector listener_normal;

f32 audio_buffer[SAMPLES_PER_TICK];
