#include "../../game/scripts/scripts.h"
#include "../exports/buffers/persist.h"
#include "scene.h"
#include "entity.h"

void enter(const script script)
{
  destroy_all_entities();
  current_script = script;
  script_body(script);
  persist_state();
}

script current_script = SCRIPT_START;
