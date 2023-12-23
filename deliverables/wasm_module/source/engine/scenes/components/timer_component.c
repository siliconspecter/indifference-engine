

#include "../../primitives/index.h"
#include "../../primitives/quantity.h"
#include "../../miscellaneous.h"
#include "timer_component.h"
#include "component.h"

index timer_component(
    const index entity,
    const index meta,
    timer_component_ticked *const on_tick,
    timer_component_expired *const on_expiry,
    const quantity ticks)
{
  // TODO
  (void)(entity);
  (void)(meta);
  (void)(on_tick);
  (void)(on_expiry);
  (void)(ticks);
  return INDEX_NONE;
}

component_handle timer_sub_component(
    const component_handle component,
    const index meta,
    timer_component_ticked *const on_tick,
    timer_component_expired *const on_expiry,
    const quantity ticks)
{
  // TODO
  (void)(component);
  (void)(meta);
  (void)(on_tick);
  (void)(on_expiry);
  (void)(ticks);
  return INDEX_NONE;
}

void before_executing_timer_components()
{
  // TODO
}

void execute_timer_components()
{
  // TODO
}
