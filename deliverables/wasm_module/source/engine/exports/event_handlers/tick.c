#include "../export.h"
#include "event_handler.h"
#include "../../scenes/components/tick_component.h"
#include "../../scenes/components/timer_component.h"

EXPORT void tick()
{
  initialize_event_handler();

  before_executing_tick_components();
  before_executing_timer_components();

  execute_tick_components();
  execute_timer_components();
}
