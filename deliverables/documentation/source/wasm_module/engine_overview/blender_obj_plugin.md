# Blender OBJ Plugin

## Installing

- Download and install [Blender](https://www.blender.org/download/).  The plugin
  was developed using version 3.6.5.  Other versions will likely work.
- Open Blender.
- From the menu bar, select "Edit" > "Preferences".
- A "Blender Preferences" window will open.
- Select the "Add-ons" tab from the left.
- Click the "Install..." button in the top right corner.
- Select the `tools/blender-obj.py` file.
- Tick the checkbox left of the "Import-Export: Indifference Engine OBJ" which
  appears.

## Importing OBJ files into Blender

Select `File` > `Import` > `Indifference Engine OBJ (*.obj)`.  This will prompt
for an OBJ file (and corresponding MTLLIB file) to import.

This will add to the content of the current scene and configure Blender to
display assets similarly to Indifference Engine.

## Creating new materials

Select `Add` > `Indifference Engine` > `Material` from the menu bar and select
the appropriate material type:

| Type     | Description                                                                                                                                                                                                                                                                                                                              |
| -------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Opaque   | Solid geometry.  Should be used for all surfaces not matching any of the below.                                                                                                                                                                                                                                                          |
| Cutout   | Similar to "Opaque", but where any opacity values below 50% are fully transparent, with a hard edge.  Useful for grates, fences, foliage, etc.  Prefer over "Blend" where possible.                                                                                                                                                      |
| Blend    | Blends between the surface and those behind it (similar to the "Normal" layer blend mode in Adobe Photoshop).  May draw in the wrong order if layered with "Blend" or "Additive" materials.  Useful for smoke, glass, etc.                                                                                                               |
| Additive | Brightens the background (similar to the "Linear Dodge (Add)" layer blend mode in Adobe Photoshop).  May draw in the wrong order if layered together with "Blend" materials, but _will_ combine correctly with "Opaque", "Cutout" and/or "Additive" materials.  Useful for lasers, lens flares, flames, explosions, muzzle flashes, etc. |

You will then be prompted for a TGA file.  If no material of the specified type
exists for the TGA file, one is created using the name of the TGA file excluding
its file extension and the selected TGA file as a base color.  It will be
prefixed with the material type.

If an object other than the navigation mesh is selected and no material slot
uses that TGA file, the material found or created above is added to a new slot.

This will also configure Blender to display assets similarly to Indifference
Engine.

## Creating navigation meshes

Select `Add` > `Indifference Engine` > `Navigation Mesh` from the menu bar.

A new plane with an appropriate name will be added to the scene, selected, and
the camera will focus upon it.

This will also configure Blender to display assets similarly to Indifference
Engine.

## Exporting OBJ files from Blender

Select `File` > `Export` > `Indifference Engine OBJ (*.obj)`.  This will prompt
for the file path to export to, overwriting if it already exists.  A MTLLIB file
will also be exported at the same time.

## Restrictions

- This plugin will export all objects from the current scene, even if they are
  hidden.
- It will NOT apply any transformations; any moving, rotating,
  scaling etc. of objects will NOT be applied.
- Modifiers will NOT be applied (e.g. mirror modifiers).
- Shape keys are NOT supported.
- Non-mesh objects will NOT be exported.
- All object and material names must start with a lower case letter, and all
  following characters must be lower case letters, digits or underscores.
- All texture names must start with a lower case letter, and all following
  characters must be lower case letters, digits or underscores, until the ending
  `.tga`.
- All textures must be in the same directory as the OBJ file, or a subdirectory.
- Detached vertices and edges are not permitted.
- All mesh objects must have at least one face.

### Meshes

- Each face must have a material with an image texture applied.
- Only exactly one UV map is supported.
- Only exactly one color attribute is supported.

### Navigation Meshes

- Materials cannot be applied.
- UV maps are NOT supported.
- Color attributes are NOT supported.
- All faces must be convex.
- All faces must be planar.
