# WASM module engine API

All types, variables, functions and preprocessor macros which form the engine's
API are documented through comments and are listed here for quick reference.

| Header                                                      | Description                                                                            |
| ----------------------------------------------------------- | -------------------------------------------------------------------------------------- |
| @ref deliverables/wasm_module/source/engine/miscellaneous.h | General-purpose helpers yet to be organized elsewhere.                                 |
| @ref deliverables/wasm_module/source/engine/scripting_api.h | A single header which includes most headers which might be needed for writing scripts. |

## Assets

| Header                                                               | Description                                                    |
| -------------------------------------------------------------------- | -------------------------------------------------------------- |
| @ref deliverables/wasm_module/source/engine/assets/font.h            | Bitmap fonts.                                                  |
| @ref deliverables/wasm_module/source/engine/assets/texture.h         | Textures with 128-bits-per-pixel color.                        |
| @ref deliverables/wasm_module/source/engine/assets/mesh.h            | Triangulated meshes with texture coordinates and vertex color. |
| @ref deliverables/wasm_module/source/engine/assets/navigation_mesh.h | Surfaces used for collision and navigation.                    |
| @ref deliverables/wasm_module/source/engine/assets/song.h            | Pieces of tracker music.                                       |
| @ref deliverables/wasm_module/source/engine/assets/sound.h           | Monaural sound effects.                                        |

## Exports

| Header                                                                      | Description                                                     |
| --------------------------------------------------------------------------- | --------------------------------------------------------------- |
| @ref deliverables/wasm_module/source/engine/exports/buffers/audio.h         | The audio output buffer and associated state.                   |
| @ref deliverables/wasm_module/source/engine/exports/buffers/controllers.h   | The state of the controllers.                                   |
| @ref deliverables/wasm_module/source/engine/exports/buffers/error.h         | Helpers for throwing traps to the hosting platform.             |
| @ref deliverables/wasm_module/source/engine/exports/buffers/persist.h       | Helpers for notifying the hosting platform what it may persist. |
| @ref deliverables/wasm_module/source/engine/exports/buffers/pointer.h       | The state of the pointing device.                               |
| @ref deliverables/wasm_module/source/engine/exports/buffers/video.h         | The video output buffer and associated state.                   |

## Math

| Header                                                          | Description                                           |
| --------------------------------------------------------------- | ----------------------------------------------------- |
| @ref deliverables/wasm_module/source/engine/math/culled_by.h    | Culling results.                                      |
| @ref deliverables/wasm_module/source/engine/math/float.h        | Floating point helpers.                               |
| @ref deliverables/wasm_module/source/engine/math/matrix.h       | 4x4 transformation matrices.                          |
| @ref deliverables/wasm_module/source/engine/math/relational.h   | Number comparison helpers.                            |
| @ref deliverables/wasm_module/source/engine/math/trigonometry.h | Trigonometry helpers.                                 |
| @ref deliverables/wasm_module/source/engine/math/vector.h       | 3D vectors.                                           |

## Primitives

| Header                                                         | Description                                                       |
| -------------------------------------------------------------- | ----------------------------------------------------------------- |
| @ref deliverables/wasm_module/source/engine/primitives/f32.h   | 32-bit floating-point numbers (equivalent to WASM `f32`).         |
| @ref deliverables/wasm_module/source/engine/primitives/index.h | Array indices.                                                    |
| @ref deliverables/wasm_module/source/engine/primitives/s32.h   | Signed 2's complement 32-bit integers (equivalent to WASM `i32`). |

## Scenes

| Header                                                      | Description                                   |
| ----------------------------------------------------------- | --------------------------------------------- |
| @ref deliverables/wasm_module/source/engine/scenes/entity.h | Entities to which components can be attached. |
| @ref deliverables/wasm_module/source/engine/scenes/scene.h  | Helpers for managing scenes.                  |

### Components

| Header                                                                                              | Description                                                                                    |
| --------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- |
| @ref deliverables/wasm_module/source/engine/scenes/components/animated_opaque_billboard_component.h | Components which display non-translucent "billboard" sprites with "flip-book" style animation. |
| @ref deliverables/wasm_module/source/engine/scenes/components/animated_opaque_mesh_component.h      | Components which display non-translucent meshes with "flip-book" style animation.              |
| @ref deliverables/wasm_module/source/engine/scenes/components/camera_component.h                    | Components which render the scene to a viewport.                                               |
| @ref deliverables/wasm_module/source/engine/scenes/components/component.h                           | Tools for making custom components to attach to entities.                                      |
| @ref deliverables/wasm_module/source/engine/scenes/components/navigation_mesh_component.h           | Components for which collision and navigation.                                                 |
| @ref deliverables/wasm_module/source/engine/scenes/components/opaque_billboard_component.h          | Components which display non-translucent "billboard" sprites.                                  |
| @ref deliverables/wasm_module/source/engine/scenes/components/opaque_mesh_component.h               | Components which display non-translucent meshes.                                               |
| @ref deliverables/wasm_module/source/engine/scenes/components/selectable_component.h                | Components which convert their attached entities into UI elements.                             |
| @ref deliverables/wasm_module/source/engine/scenes/components/song_component.h                      | Components which play songs.                                                                   |
| @ref deliverables/wasm_module/source/engine/scenes/components/sound_component.h                     | Components which play sounds.                                                                  |
| @ref deliverables/wasm_module/source/engine/scenes/components/tick_component.h                      | Components which execute callbacks on each tick.                                               |
| @ref deliverables/wasm_module/source/engine/scenes/components/timer_component.h                     | Components which execute callbacks after fixed numbers of ticks.                               |
