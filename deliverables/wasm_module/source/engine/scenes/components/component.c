
#include "../../primitives/index.h"
#include "../../primitives/s32.h"
#include "../../../game/project_settings/limits.h"
#include "../../miscellaneous.h"
#include "component.h"
#include "../../exports/buffers/error.h"

#define COMPONENT_STATE_INACTIVE 0
#define COMPONENT_STATE_ACTIVE 1
#define COMPONENT_STATE_DELETING 2

static s32 states[MAXIMUM_COMPONENTS];
static s32 handles[MAXIMUM_COMPONENTS];
static component_destroyed *destructors[MAXIMUM_COMPONENTS];

static index first_occupied = INDEX_NONE;
static index last_occupied;

component_handle component(
    const index entity,
    const index meta,
    component_destroyed *const on_destroy)
{
  // TODO: Check entity exists
  FIND_EMPTY_INDEX(states, COMPONENT_STATE_INACTIVE, MAXIMUM_COMPONENTS, first_occupied, last_occupied, ERROR_NO_COMPONENTS_TO_ALLOCATE, index)
  const component_handle component = meta | (1 << COMPONENT_HANDLE_BITS_FOR_META) | (entity << (COMPONENT_HANDLE_BITS_FOR_META + 1)) | (index << (COMPONENT_HANDLE_BITS_FOR_META + 1 + COMPONENT_HANDLE_BITS_FOR_PARENT));
  states[index] = COMPONENT_STATE_ACTIVE;
  handles[index] = component;
  destructors[index] = on_destroy;
  return component;
}

component_handle sub_component(
    const component_handle component,
    const index meta,
    component_destroyed *const on_destroy)
{
  const index parent = COMPONENT_HANDLE_COMPONENT(component);

  if (states[parent] == COMPONENT_STATE_ACTIVE)
  {
    FIND_EMPTY_INDEX(states, COMPONENT_STATE_INACTIVE, MAXIMUM_COMPONENTS, first_occupied, last_occupied, ERROR_NO_COMPONENTS_TO_ALLOCATE, index)
    const component_handle component = meta | (1 << COMPONENT_HANDLE_BITS_FOR_META) | (parent << (COMPONENT_HANDLE_BITS_FOR_META + 1)) | (index << (COMPONENT_HANDLE_BITS_FOR_META + 1 + COMPONENT_HANDLE_BITS_FOR_PARENT));
    states[index] = COMPONENT_STATE_ACTIVE;
    handles[index] = component;
    destructors[index] = on_destroy;
    return component;
  }
  else
  {
    throw(ERROR_COMPONENT_DOES_NOT_EXIST);
  }
}

static void destroy_all_sub_components_of_index(const index component)
{
  if (first_occupied != INDEX_NONE)
  {
    for (index index = first_occupied; index <= last_occupied; index++)
    {
      if (states[index] == COMPONENT_STATE_ACTIVE)
      {
        const component_handle handle = handles[index];

        if (COMPONENT_HANDLE_IS_CHILD_OF_COMPONENT(handle) && COMPONENT_HANDLE_PARENT(handle) == component)
        {
          states[index] = COMPONENT_STATE_DELETING;
          destroy_all_sub_components_of_index(index);
          destructors[index](handle);
          states[index] = COMPONENT_STATE_INACTIVE;
        }
      }
    }
  }
}

void destroy_component(const component_handle component)
{
  const index index = COMPONENT_HANDLE_COMPONENT(component);

  if (states[index] == COMPONENT_STATE_ACTIVE)
  {
    states[index] = COMPONENT_STATE_DELETING;
    destroy_all_sub_components_of_index(index);
    destructors[index](component);
    states[index] = COMPONENT_STATE_INACTIVE;
    INDEX_VACATED(index, states, COMPONENT_STATE_INACTIVE, first_occupied, last_occupied)
  }
  else
  {
    throw(ERROR_COMPONENT_DOES_NOT_EXIST);
  }
}

index parent_entity_of(const component_handle component)
{
  if (states[COMPONENT_HANDLE_COMPONENT(component)] == COMPONENT_STATE_ACTIVE)
  {
    component_handle recursed = component;

    while (COMPONENT_HANDLE_IS_CHILD_OF_COMPONENT(recursed))
    {
      recursed = handles[COMPONENT_HANDLE_PARENT(recursed)];
    }

    return COMPONENT_HANDLE_PARENT(recursed);
  }
  else
  {
    throw(ERROR_COMPONENT_DOES_NOT_EXIST);
  }
}

void destroy_all_components_of(const index entity)
{
  if (first_occupied != INDEX_NONE)
  {
    for (index component = first_occupied; component <= last_occupied; component++)
    {
      if (states[component] == COMPONENT_STATE_ACTIVE)
      {
        const component_handle handle = handles[component];

        if (COMPONENT_HANDLE_IS_CHILD_OF_ENTITY(handle) && COMPONENT_HANDLE_PARENT(handle) == entity)
        {
          destroy_component(handles[component]);
        }
      }
    }
  }
}
