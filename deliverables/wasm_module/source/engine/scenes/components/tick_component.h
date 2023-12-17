/** @file */

#ifndef TICK_COMPONENT_H

#define TICK_COMPONENT_H

#include "../../primitives/index.h"

/**
 * A callback which is called once per tick, exclusive of the tick in which the
 * tick component was created.
 * @remark This can happen only during a script or the tick event handler.
 * @param meta The arbitrary index of the tick component which ticked.
 */
typedef void(tick_component_ticked)(index meta);

/**
 * Creates a new tick component as a direct child of an entity.
 * @remark Will throw a trap should there be no tick components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @param entity The index of the entity to which to add a tick component.
 * @param meta An arbitrary index which can be used to look up use-case-specific
 *            data.
 * @param on_tick Called once per tick, exclusive of the tick on which the tick
 *                component was created.
 * @return A handle to the created tick component.
 */
index tick_component(
    const index entity,
    const index meta,
    tick_component_ticked *const on_tick);

/**
 * Creates a new tick component as a direct child of another component.
 * @remark Will throw a trap should there be no tick components left to
 *         allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @param component A handle to the component to which to add a tick component.
 * @param meta An arbitrary index which can be used to look up use-case-specific
 *            data.
 * @param on_tick Called once per tick, exclusive of the tick on which the tick
 *                component was created.
 * @return A handle to the created tick component.
 */
component_handle tick_sub_component(
    const component_handle component,
    const index meta,
    tick_component_ticked *const on_tick);

#endif
