# API Documentation

| Header                      | Description                                                    |
| --------------------------- | -------------------------------------------------------------- |
| @ref source/engine/limits.h | Constants for engine limits which can be adjusted as required. |

## Assets

| Header                                      | Description                                                    |
| ------------------------------------------- | -------------------------------------------------------------- |
| @ref source/engine/assets/font.h            | Bitmap fonts.                                                  |
| @ref source/engine/assets/image.h           | Images with 32-bits-per-pixel color.                           |
| @ref source/engine/assets/mesh.h            | Triangulated meshes with texture coordinates and vertex color. |
| @ref source/engine/assets/navigation_mesh.h | Surfaces used for collision and navigation.                    |
| @ref source/engine/assets/song.h            | Pieces of tracker music.                                       |
| @ref source/engine/assets/sound.h           | Monoaural sound effects.                                       |

## Exports

| Header                               | Description                                                     |
| ------------------------------------ | --------------------------------------------------------------- |
| @ref source/engine/exports/error.h   | Helpers for throwing traps to the hosting platform.             |
| @ref source/engine/exports/persist.h | Helpers for notifying the hosting platform what it may persist. |

## Math

| Header                                 | Description                                           |
| -------------------------------------- | ----------------------------------------------------- |
| @ref source/engine/math/culled_by.h    | Culling results.                                      |
| @ref source/engine/math/float.h        | Floating point helpers.                               |
| @ref source/engine/math/matrix_pair.h  | Forward/inverse pairs of 4x4 transformation matrices. |
| @ref source/engine/math/matrix.h       | 4x4 transformation matrices.                          |
| @ref source/engine/math/relational.h   | Number comparison helpers.                            |
| @ref source/engine/math/trigonometry.h | Trigonometry helpers.                                 |
| @ref source/engine/math/vector.h       | 3D vectors.                                           |

## Primitives

| Header                                | Description                                                       |
| ------------------------------------- | ----------------------------------------------------------------- |
| @ref source/engine/primitives/byte.h  | Unsigned 8-bit integers.                                          |
| @ref source/engine/primitives/f32.h   | 32-bit floating-point numbers (equivalent to WASM `f32`).         |
| @ref source/engine/primitives/index.h | Array indices.                                                    |
| @ref source/engine/primitives/s32.h   | Signed 2's complement 32-bit integers (equivalent to WASM `i32`). |

## Scenes

| Header                             | Description                                   |
| ---------------------------------- | --------------------------------------------- |
| @ref source/engine/scenes/entity.h | Entities to which components can be attached. |

### Components

| Header                                                                     | Description                                                                                    |
| -------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- |
| @ref source/engine/scenes/components/animated_opaque_billboard_component.h | Components which display non-translucent "billboard" sprites with "flip-book" style animation. |
| @ref source/engine/scenes/components/animated_opaque_mesh_component.h      | Components which display non-translucent meshes with "flip-book" style animation.              |
| @ref source/engine/scenes/components/camera_component.h                    | Components which render the scene to a viewport.                                               |
| @ref source/engine/scenes/components/component.h                           | Tools for making custom components to attach to entities.                                      |
| @ref source/engine/scenes/components/navigation_mesh_component.h           | Components for which collision and navigation.                                                 |
| @ref source/engine/scenes/components/opaque_billboard_component.h          | Components which display non-translucent "billboard" sprites.                                  |
| @ref source/engine/scenes/components/opaque_mesh_component.h               | Components which display non-translucent meshes.                                               |
| @ref source/engine/scenes/components/selectable_component.h                | Components which convert their attached entities into UI elements.                             |
| @ref source/engine/scenes/components/song_component.h                      | Components which play songs.                                                                   |
| @ref source/engine/scenes/components/sound_component.h                     | Components which play sounds.                                                                  |
| @ref source/engine/scenes/components/tick_component.h                      | Components which execute callbacks on each tick.                                               |
| @ref source/engine/scenes/components/timer_component.h                     | Components which execute callbacks after fixed numbers of ticks.                               |

## Video

| Header                           | Description                                      |
| -------------------------------- | ------------------------------------------------ |
| @ref source/engine/video/color.h | 32-bits-per-pixel red-green-blue-opacity-colors. |
