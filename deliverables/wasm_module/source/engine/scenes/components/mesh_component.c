#include "component.h"
#include "../entity.h"
#include "../../assets/mesh.h"
#include "../../primitives/index.h"
#include "../../primitives/quantity.h"
#include "../../math/matrix.h"
#include "../../../game/project_settings/limits.h"
#include "../../exports/buffers/error.h"

static index first_occupied_opaque_cutout;
static index last_occupied_opaque_cutout;
static quantity total_occupied_opaque_cutout;
static const matrix *opaque_cutout_transforms[MAXIMUM_OPAQUE_CUTOUT_MESH_COMPONENTS];
static const matrix *inverse_opaque_cutout_transforms[MAXIMUM_OPAQUE_CUTOUT_MESH_COMPONENTS];
static const mesh *opaque_cutout_meshes[MAXIMUM_OPAQUE_CUTOUT_MESH_COMPONENTS];

static index first_occupied_additive_blended;
static index last_occupied_additive_blended;
static quantity total_occupied_additive_blended;
static const matrix *additive_blended_transforms[MAXIMUM_ADDITIVE_BLENDED_MESH_COMPONENTS];
static const matrix *inverse_additive_blended_transforms[MAXIMUM_ADDITIVE_BLENDED_MESH_COMPONENTS];
static const mesh *additive_blended_meshes[MAXIMUM_ADDITIVE_BLENDED_MESH_COMPONENTS];

static index first_occupied;
static index last_occupied;
static quantity total_occupied;
static const matrix *transforms[MAXIMUM_MESH_COMPONENTS];
static const matrix *inverse_transforms[MAXIMUM_MESH_COMPONENTS];
static index opaque_cutout[MAXIMUM_MESH_COMPONENTS];
static index additive_blended[MAXIMUM_MESH_COMPONENTS];

static void set_mesh(const index meta, const mesh *const mesh)
{
  const index opaque_cutout_index = opaque_cutout[meta];

  if (mesh->opaque_cutout_vertices == 0)
  {
    if (opaque_cutout_index != INDEX_NONE)
    {
      INDEX_VACATE(opaque_cutout_index, opaque_cutout_meshes, NULL, first_occupied_opaque_cutout, last_occupied_opaque_cutout, total_occupied_opaque_cutout)
      opaque_cutout[meta] = INDEX_NONE;
    }
  }
  else if (opaque_cutout_index == INDEX_NONE)
  {
    FIND_EMPTY_INDEX(opaque_cutout_meshes, NULL, MAXIMUM_OPAQUE_CUTOUT_MESH_COMPONENTS, first_occupied_opaque_cutout, last_occupied_opaque_cutout, total_occupied_opaque_cutout, ERROR_NO_OPAQUE_CUTOUT_MESH_COMPONENTS_TO_ALLOCATE, opaque_cutout_index)
    opaque_cutout_transforms[opaque_cutout_index] = transforms[meta];
    inverse_opaque_cutout_transforms[opaque_cutout_index] = inverse_transforms[meta];
    opaque_cutout_meshes[opaque_cutout_index] = mesh;
    opaque_cutout[meta] = opaque_cutout_index;
  }

  const index additive_blended_index = additive_blended[meta];

  if (mesh->additive_blended_vertices == 0)
  {
    if (additive_blended_index != INDEX_NONE)
    {
      INDEX_VACATE(additive_blended_index, additive_blended_meshes, NULL, first_occupied_additive_blended, last_occupied_additive_blended, total_occupied_additive_blended)
      additive_blended[meta] = INDEX_NONE;
    }
  }
  else if (additive_blended_index == INDEX_NONE)
  {
    FIND_EMPTY_INDEX(additive_blended_meshes, NULL, MAXIMUM_ADDITIVE_BLENDED_MESH_COMPONENTS, first_occupied_additive_blended, last_occupied_additive_blended, total_occupied_additive_blended, ERROR_NO_ADDITIVE_BLENDED_MESH_COMPONENTS_TO_ALLOCATE, additive_blended_index)
    additive_blended_transforms[additive_blended_index] = transforms[meta];
    inverse_additive_blended_transforms[additive_blended_index] = inverse_transforms[meta];
    additive_blended_meshes[additive_blended_index] = mesh;
    additive_blended[meta] = additive_blended_index;
  }
}

static index allocate(const index entity, const mesh *const mesh)
{
  FIND_EMPTY_INDEX(transforms, NULL, MAXIMUM_MESH_COMPONENTS, first_occupied, last_occupied, total_occupied, ERROR_NO_MESH_COMPONENTS_TO_ALLOCATE, meta)

  transforms[meta] = &entity_model_view_projections[entity];
  inverse_transforms[meta] = &inverse_entity_model_view_projections[entity];
  opaque_cutout[meta] = INDEX_NONE;
  additive_blended[meta] = INDEX_NONE;
  set_mesh(meta, mesh);

  return meta;
}

static void destroy(const component_handle component)
{
  const index meta = COMPONENT_HANDLE_META(component);

  const index opaque_cutout_index = opaque_cutout[meta];

  if (opaque_cutout_index != INDEX_NONE)
  {
    INDEX_VACATE(opaque_cutout_index, opaque_cutout_meshes, NULL, first_occupied_opaque_cutout, last_occupied_opaque_cutout, total_occupied_opaque_cutout)
    opaque_cutout[meta] = INDEX_NONE;
  }

  const index additive_blended_index = additive_blended[meta];

  if (additive_blended_index != INDEX_NONE)
  {
    INDEX_VACATE(additive_blended_index, additive_blended_meshes, NULL, first_occupied_additive_blended, last_occupied_additive_blended, total_occupied_additive_blended)
    additive_blended[meta] = INDEX_NONE;
  }

  transforms[meta] = NULL;
}

component_handle mesh_component(
    const index entity,
    const mesh *const mesh)
{
  const index meta = allocate(entity, mesh);
  return component(entity, meta, destroy);
}

component_handle mesh_sub_component(
    const component_handle component,
    const mesh *const mesh)
{
  const index meta = allocate(parent_entity_of(component), mesh);
  return sub_component(component, meta, destroy);
}

void render_opaque_cutout_mesh_components()
{
  if (first_occupied_opaque_cutout != INDEX_NONE)
  {
    for (index index = first_occupied_opaque_cutout; index <= last_occupied_opaque_cutout; index++)
    {
      const mesh *const mesh = opaque_cutout_meshes[index];

      if (mesh != NULL)
      {
        render_opaque_cutout_mesh(mesh, *opaque_cutout_transforms[index]);
      }
    }
  }
}

void render_additive_blended_mesh_components()
{
  if (first_occupied_additive_blended != INDEX_NONE)
  {
    for (index index = first_occupied_additive_blended; index <= last_occupied_additive_blended; index++)
    {
      const mesh *const mesh = additive_blended_meshes[index];

      if (mesh != NULL)
      {
        render_additive_blended_mesh(mesh, *additive_blended_transforms[index]);
      }
    }
  }
}
