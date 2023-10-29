#include "../../primitives/s32.h"
#include "../../primitives/f32.h"
#include "../../../game/project_settings/input_settings.h"
#include "controllers.h"
#include "../export.h"

controller_state controller_states[MAXIMUM_CONTROLLERS] ALIGN(s32);

f32 controller_y_axes[MAXIMUM_CONTROLLERS] ALIGN(f32);

f32 controller_x_axes[MAXIMUM_CONTROLLERS] ALIGN(f32);
