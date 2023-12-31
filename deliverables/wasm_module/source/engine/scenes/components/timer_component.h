/** @file */

#ifndef TIMER_COMPONENT_H

#define TIMER_COMPONENT_H

#include "../../primitives/index.h"
#include "../../primitives/quantity.h"
#include "../../miscellaneous.h"
#include "component.h"

/**
 * A callback which is called once per tick while a timer component runs,
 * exclusive of the tick in which it was created and inclusive of the tick on
 * which it expires (but before it is destroyed).
 * @remark This can happen only during a script or the tick event handler.
 * @param meta The arbitrary index of the timer component which ticked.
 */
typedef void(timer_component_ticked)(index meta);

/**
 * A callback which is called when a timer component expires.  The timer
 * component has been destroyed by the time of calling.  NOT called should the
 * timer component be destroyed first.
 * @remark This can happen only during a script or the tick event handler.
 * @param meta The arbitrary index of the timer component which expired.
 */
typedef void(timer_component_expired)(index meta);

/**
 * Creates a new timer component as a direct child of an entity.
 * @remark Will throw a trap should there be no timer components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @param entity The index of the entity to which to add a timer component.
 * @param meta An arbitrary index which can be used to look up use-case-specific
 *            data.
 * @param on_tick Called once per tick while the timer component elapses,
 *                exclusive of the tick on which it was created and inclusive of
 *                the tick on which it expires (but before it is destroyed).
 *                @ref NULL is acceptable.
 * @param on_expiry Called when the timer component expires.  The timer
 *                  component has been destroyed by the time of calling.  NOT
 *                  called should the timer component be destroyed first.
 *                  @ref NULL is acceptable.
 * @param ticks The duration of the timer component, in ticks.  Must be at least
 *              1.
 * @return A handle to the created timer component.
 */
index timer_component(
    const index entity,
    const index meta,
    timer_component_ticked *const on_tick,
    timer_component_expired *const on_expiry,
    const quantity ticks);

/**
 * Creates a new timer component as a direct child of another component.
 * @remark Will throw a trap should there be no timer components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @param component A handle to the component to which to add a timer component.
 * @param meta An arbitrary index which can be used to look up use-case-specific
 *            data.
 * @param on_tick Called once per tick while the timer component elapses,
 *                exclusive of the tick on which it was created and inclusive of
 *                the tick on which it expires (but before it is destroyed).
 *                @ref NULL is acceptable.
 * @param on_expiry Called when the timer component expires.  The timer
 *                  component has been destroyed by the time of calling.  NOT
 *                  called should the timer component be destroyed first.
 *                  @ref NULL is acceptable.
 * @param ticks The duration of the timer component, in ticks.  Must be at least
 *              1.
 * @return A handle to the created timer component.
 */
component_handle timer_sub_component(
    const component_handle component,
    const index meta,
    timer_component_ticked *const on_tick,
    timer_component_expired *const on_expiry,
    const quantity ticks);

#ifndef DOXYGEN_IGNORE

/**
 * Called at the very start of the tick event handler.
 */
void before_executing_timer_components();

/**
 * Called by the tick event handler once to advance all timer components.
 */
void execute_timer_components();

#endif

#endif
