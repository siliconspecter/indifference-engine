#include "../primitives/index.h"
#include "../primitives/s32.h"
#include "../math/matrix.h"
#include "../../game/project_settings/limits.h"
#include "components/component.h"
#include "../exports/buffers/error.h"
#include "../exports/buffers/video.h"

matrix previous_entity_transforms[MAXIMUM_ENTITIES];
matrix next_entity_transforms[MAXIMUM_ENTITIES];
matrix interpolated_entity_transforms[MAXIMUM_ENTITIES];
matrix previous_inverse_entity_transforms[MAXIMUM_ENTITIES];
matrix next_inverse_entity_transforms[MAXIMUM_ENTITIES];
matrix interpolated_inverse_entity_transforms[MAXIMUM_ENTITIES];

#define ENTITY_STATE_INACTIVE 0
#define ENTITY_STATE_ACTIVE 1
#define ENTITY_STATE_DELETING 2

static s32 states[MAXIMUM_ENTITIES];

static index first_occupied = INDEX_NONE;
static index last_occupied;

index entity()
{
  FIND_EMPTY_INDEX(states, ENTITY_STATE_INACTIVE, MAXIMUM_ENTITIES, first_occupied, last_occupied, ERROR_NO_ENTITIES_TO_ALLOCATE, entity)

  states[entity] = ENTITY_STATE_ACTIVE;
  copy_matrix(identity_matrix, previous_entity_transforms[entity]);
  copy_matrix(identity_matrix, next_entity_transforms[entity]);
  copy_matrix(identity_matrix, previous_inverse_entity_transforms[entity]);
  copy_matrix(identity_matrix, next_inverse_entity_transforms[entity]);

  return entity;
}

void destroy_entity(const index entity)
{
  if (states[entity] == ENTITY_STATE_ACTIVE)
  {
    states[entity] = ENTITY_STATE_DELETING;
    destroy_all_components_of(entity);
    states[entity] = ENTITY_STATE_INACTIVE;
    INDEX_VACATED(entity, states, ENTITY_STATE_INACTIVE, first_occupied, last_occupied)
  }
  else
  {
    throw(ERROR_ENTITY_DOES_NOT_EXIST);
  }
}

void destroy_all_entities()
{
  if (first_occupied != INDEX_NONE)
  {
    for (index entity = first_occupied; entity <= last_occupied; entity++)
    {
      if (states[entity] == ENTITY_STATE_ACTIVE)
      {
        destroy_entity(entity);
      }
    }
  }
}

static void prepare_entity_column_for_video(
    const f32 *const previous,
    const f32 *const next,
    f32 *const interpolated,
    const s32 total)
{
  for (index index = 0; index < total; index++)
  {
    interpolated[index] = tick_progress * next[index] + inverse_tick_progress * previous[index];
  }
}

void prepare_entities_for_video()
{
  if (first_occupied != INDEX_NONE)
  {
    const s32 total = (last_occupied - first_occupied + 1) * 16;

    prepare_entity_column_for_video(
        &previous_entity_transforms[first_occupied][0][0],
        &next_entity_transforms[first_occupied][0][0],
        &interpolated_entity_transforms[first_occupied][0][0],
        total);

    prepare_entity_column_for_video(
        &previous_inverse_entity_transforms[first_occupied][0][0],
        &next_inverse_entity_transforms[first_occupied][0][0],
        &interpolated_inverse_entity_transforms[first_occupied][0][0],
        total);
  }
}
