#include "../../primitives/index.h"
#include "../../primitives/quantity.h"
#include "../../primitives/s32.h"
#include "component.h"
#include "tick_component.h"
#include "../../../game/project_settings/limits.h"
#include "../../exports/buffers/error.h"
#include "../../miscellaneous.h"

static index first_occupied;
static index last_occupied;
static quantity total_occupied;
static tick_component_ticked *on_ticks[MAXIMUM_TICK_COMPONENTS];
static index metas[MAXIMUM_TICK_COMPONENTS];
static s32 delays[MAXIMUM_TICK_COMPONENTS];

#define STATE_OUTSIDE_TICK_OR_AFTER_EXECUTION 0
#define STATE_BEFORE_EXECUTION 1
#define STATE_DURING_EXECUTION 2

static s32 state = STATE_OUTSIDE_TICK_OR_AFTER_EXECUTION;
static index iterator;

static void destroy(const component_handle component)
{
  const index tick = COMPONENT_HANDLE_META(component);
  on_ticks[tick] = NULL;
}

static index allocate(const index meta, tick_component_ticked *const on_tick)
{
  FIND_EMPTY_INDEX(on_ticks, NULL, MAXIMUM_TICK_COMPONENTS, first_occupied, last_occupied, total_occupied, ERROR_NO_TICK_COMPONENTS_TO_ALLOCATE, tick)
  on_ticks[tick] = on_tick;
  metas[tick] = meta;

  switch (state)
  {
  case STATE_OUTSIDE_TICK_OR_AFTER_EXECUTION:
    // The next time we iterate, we should execute this tick component.
    delays[tick] = 0;
    break;

  case STATE_BEFORE_EXECUTION:
    // We're about to iterate, so wait a tick until this execute the component
    // we've just created.
    delays[tick] = 1;
    break;

  case STATE_DURING_EXECUTION:
    if (iterator >= meta)
    {
      // We're currently iterating; if we've already past this index, there's
      // effectively a delay until the next tick so don't delay further.
      delays[tick] = 0;
    }
    else
    {
      // Otherwise, avoid executing during this iteration with a delay.
      delays[tick] = 1;
    }
    break;
  }

  return tick;
}

index tick_component(
    const index entity,
    const index meta,
    tick_component_ticked *const on_tick)
{
  const index tick = allocate(meta, on_tick);
  return component(entity, tick, destroy);
}

component_handle tick_sub_component(
    const component_handle component,
    const index meta,
    tick_component_ticked *const on_tick)
{
  const index tick = allocate(meta, on_tick);
  return sub_component(component, tick, destroy);
}

void before_executing_tick_components()
{
  state = STATE_BEFORE_EXECUTION;
}

void execute_tick_components()
{
  if (first_occupied != INDEX_NONE)
  {
    state = STATE_DURING_EXECUTION;

    for (index tick = first_occupied; tick <= last_occupied; tick++)
    {
      tick_component_ticked *const on_tick = on_ticks[tick];

      if (on_tick != NULL)
      {
        const s32 delay = delays[tick];

        if (delay > 0)
        {
          delays[tick] = delay - 1;
        }
        else
        {
          on_tick(metas[tick]);
        }
      }
    }
  }

  state = STATE_OUTSIDE_TICK_OR_AFTER_EXECUTION;
}
