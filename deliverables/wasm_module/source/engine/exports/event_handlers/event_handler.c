#include "event_handler.h"
#include "../buffers/persist.h"
#include "../buffers/error.h"
#include "../../primitives/s32.h"
#include "../../scenes/scene.h"

static s32 first_initialization = 1;

void initialize_event_handler()
{
  persist = PERSIST_NOTHING;
  error = ERROR_NONE;

  if (first_initialization)
  {
    first_initialization = 0;
    enter(current_script);
  }
}
