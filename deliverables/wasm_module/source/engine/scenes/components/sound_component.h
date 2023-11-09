/** @file */

#ifndef SOUND_COMPONENT_H

#define SOUND_COMPONENT_H

#include "../../primitives/index.h"
#include "../../primitives/f32.h"
#include "../../assets/sound.h"
#include "../../miscellaneous.h"
#include "../../../game/project_settings/limits.h"

/**
 * A callback which is called when a sound component is replaced by a newly
 * created one.
 * @remark This can happen only during a script or the tick event handler.
 * @param meta The arbitrary index of the sound component which was replaced.
 */
typedef void(sound_component_replaced)(index meta);

/**
 * A callback which is called when a sound component runs to its end.  NOT
 * called should the sound component be destroyed or replaced prior to its end.
 * Should sounds loop, called each time the end is reached.  Otherwise called
 * after the sound component is destroyed.
 * @remark This can happen only during a script or the tick event handler.
 * @param meta The arbitrary index of the sound component which ran to
 *             completion.
 */
typedef void(sound_component_ended)(index meta);

/**
 * Creates a new sound component as a direct child of an entity.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will replace the oldest playing sound component should there be no
 *         sound components left to allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified entity not exist at the time
 *         of calling.
 * @remark This defaults to playing at unity gain (1.0).
 * @remark This defaults to playing at unity speed (1.0).
 * @param entity The index of the entity to which to add a sound component.
 * @param meta An arbitrary index which can be used to look up use-case-specific
 *            data.
 * @param on_replacement Called when the sound component is replaced by a newly
 *                       created one.  @ref NULL is acceptable.
 * @param on_end Called when the sound component runs to its end.  NOT called
 *               should the sound component be destroyed or replaced prior to
 *               its end.  Should the sound loop, each time the end is reached.
 *               @ref NULL is acceptable.
 * @param sound The sound to play.
 * @return A handle to the created sound component.
 */
component_handle sound_component(
    const index entity,
    const index meta,
    const sound_component_replaced *const on_replacement,
    const sound_component_ended *const on_end,
    const sound *const sound);

/**
 * Creates a new sound component as a direct child of another component.
 * @remark Call only during scripts or the tick event handler (doing so in other
 *         situations may produce unexpected results).
 * @remark Will replace the oldest playing sound component should there be no
 *         sound components left to allocate.
 * @remark Will throw a trap should there be no components left to allocate.
 * @remark Will throw a trap should the specified component not exist at the
 *         time of calling.
 * @remark This defaults to playing at unity gain (1.0).
 * @remark This defaults to playing at unity speed (1.0).
 * @param component A handle to the component to which to add a component.
 * @param meta An arbitrary index which can be used to look up use-case-specific
 *            data.
 * @param on_replacement Called when the sound component is replaced by a newly
 *                       created one.  @ref NULL is acceptable.
 * @param on_end Called when the sound component runs to its end.  NOT called
 *               should the sound component be destroyed or replaced prior to
 *               its end.  Should the sound loop, each time the end is reached.
 *               @ref NULL is acceptable.
 * @param sound The sound to play.
 * @return A handle to the created sound component.
 */
component_handle sound_sub_component(
    const component_handle component,
    const index meta,
    const sound_component_replaced *const on_replacement,
    const sound_component_ended *const on_end,
    const sound *const sound);

/**
 * The gain levels of all sound components, where 0.0 is silent and 1.0 is unity
 * gain.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a sound component.
 */
extern f32 sound_component_gains[MAXIMUM_SOUND_COMPONENTS];

/**
 * The playback speeds of all sound components, where 1.0 is the default and
 * 2.0 is twice as fast.  Affects both tempo and pitch.
 * @remark Modify only during scripts or the tick event handler (doing so in
 *         other situations may produce unexpected results).
 * @remark Use @ref COMPONENT_HANDLE_META to extract the index to use here from
 *         a handle to a sound component.
 */
extern f32 sound_component_speeds[MAXIMUM_SOUND_COMPONENTS];

#endif
