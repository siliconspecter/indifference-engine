# WASM module engine overview

What follows is a description of how the [engine API](./engine_api.md) is
intended to be used to build your game.

## Timing

Game logic advances on a fixed interval called a "tick", synchronized with audio
whenever possible.

### Changing the tick rate

The default tick rate is fine for most games, but it can be adjusted in
@ref deliverables/wasm_module/source/game/project_settings/timing_settings.h
(`TICKS_PER_SECOND`).

#### Side effects

##### System load

Increasing the number of ticks per second increases the amount of system load
accordingly.  For example, a game with a tick rate of 5Hz requires 25% more CPU
time to update each second than a game with a tick rate of 4Hz.

For games which have complex or large-scale simulation aspects but do not
require low-latency input such as real-time strategy games, it may be necessary
to reduce the tick rate.

##### Input latency

Decreasing the number of ticks per second increases worst-case input latency.
For example, while at 5 ticks per second, it might be 200msec between pressing
a button and a tick event handling that input, at 4 ticks per second it could be
as much as 250msec.

For games which require low-latency input such as fighting games or e-sports
titles, it may be necessary to increase the tick rate.

##### Game speed

Increasing or decreasing the number of ticks per second causes the game itself
to change in speed accordingly; a car traveling at 5 units per tick goes twice
as fast when there are twice as many ticks per second.  You may therefore need
to adjust physics and animations after changing the tick rate.

##### Audio fidelity

A fixed number of audio samples is rendered for each tick event, so by default,
increasing the tick rate from 60Hz to 100Hz would increase the sample rate from
44100Hz to 73500Hz.  This would marginally increase audio quality at the cost of
significantly increased system load.

To correct this, specify the number of audio samples that your game should
render each tick in
@ref deliverables/wasm_module/source/game/project_settings/audio_settings.h
(`SAMPLES_PER_TICK`).  This should be your target sample rate divided by the
tick rate (e.g. 44100 / 60Hz = 735 samples per tick).

### Interpolation

The video render loop is decoupled from the game tick.  As a consequence, it is
possible that on displays with high refresh rates, the same game state may be
displayed multiple times.

Interpolation is supported throughout the engine to support this; generally,
linear interpolation is performed with `previous_` and `next_` variable pairs.

For example:

- Given a `previous_camera_component_sensor_sizes[a_camera_index]` of `32`.
- Given a `next_camera_component_sensor_sizes[a_camera_index]` of `40`.
- Should a video render be attempted at 1/4 of the way through a tick, the
  sensor size will effectively be `34`.

The engine will not automatically assign `previous_` values based upon `next_`
values in any scenario.

## Coordinate space

Indifference Engine imitates Blender's coordinate space, in which a camera with
an all-zero rotation looks towards -Z, where right points towards X+ and up
points towards Y+.

Positional units are not strictly defined, but are usually taken to be meters.

## Persistence

Entries need to be added to the list in
@ref deliverables/wasm_module/source/game/state/index.h for each region of
memory which needs to be retained between runs of the game.  Note that the exact
raw bytes specified in those buffers will be persisted; indices into data which
is _not_ explicitly persisted and all data and function pointers will become
invalid when the game re-starts.

If any state buffers are added or resized between versions of the WASM module,
those specific buffers will _not_ be changed from their initial values by the
hosting runtime.  Others which seemingly matching data is available for will
still be populated.

State can only be saved by the hosting runtime after `persist_state` is called
and the current event handler subsequently returns successfully.

## Scripts

A script is a function executed to populate an empty scene (see below).  The
engine will automatically run the script with identifier `0` the first time the
game starts.

It is possible to run a new script by calling `enter` with the identifier of the
script to run.  This will:

- Clear the scene down fully, destroying all entities within.
- Record the given script identifier in persistable state.
- Call `persist_state` on your behalf so that the scene transition and any
  changes made around the same time are saved.
- Finally, execute the script.

### Adding a new script

The scripts which can be executed are defined in
@ref deliverables/wasm_module/source/game/scripts/scripts.h as macros (e.g.
`SCRIPT_START`).

The switch-case statement in `script_body` within
@ref deliverables/wasm_module/source/game/scripts/scripts.c then defines what
each script does when executed.

These scripts are likely to become numerous and lengthy.  It is recommended that
a separate C file be written for each one, exporting a single function executed
by `script_body`.  @ref deliverables/wasm_module/engine/scripting_api.h can be
included to easily access most types, constants, macros and functions a script
would need.

## Scenes

There is one scene at a time.  This is a void which can be populated with
entities.  The scene is _not_ persisted as state.

### Entities

An entity is, at its core, a transform (location, rotation, scale, etc.) and a
collection of associated components.

#### Creating

A new entity can be created with an initial transform by calling `entity`:

```c
const index created = entity();
```

The index returned _will_ be re-used when the entity is destroyed, directly or
indirectly, so ensure that you do not attempt to interact with entities which
may have been destroyed - this may be caught in some cases by safety checks
within the engine, but in other cases, those interactions may end up working
with an unexpected, different entity which has since been given the same index!

Additionally, entities are allocated out of a finite pool.  The engine will
throw a trap should this resource be exhausted.  Games requiring large amounts
of entities may require adjustments to
@ref deliverables/wasm_module/source/game/project_settings/limits.h to increase
the size of the appropriate pools.

#### Transforming

Initially, all entities are given the "identity" transform, that is, equivalent
to being located at 0, 0, 0, rotated by 0, 0, 0 and scaled by 1, 1, 1.

This can be adjusted during scripts, selectable callbacks, tick callbacks, timer
expiry callbacks or animation callbacks (doing so in other situations may
produce unexpected results):

```c
copy_matrix(
  previous_entity_transforms[previously_created_entity],
  next_entity_transforms[previously_created_entity]
);

copy_matrix(
  previous_inverse_ntity_transforms[previously_created_entity],
  next_inverse_entity_transforms[previously_created_entity]
);

transform(
  location_vector,
  rotation_vector,
  scale_vector,
  next_entity_transforms[previously_created_entity],
  next_entity_inverse_transforms[previously_created_entity]
);
```

Note that there is a "previous" and "next" copy of each.  Should the game render
multiple frames of video during a single game tick, these act as "key frames" to
"tween" between over the course of the game tick.  Most components in the engine
follow this pattern.

#### Destroying

Call `destroy_entity` and ensure that you do not keep any references to it:

```c
destroy_entity(previously_created_entity);
previously_created_entity = INDEX_NONE;
```

### Components

Components are how the player interacts with entities; they produce video,
audio, handle input and advance game state.  Destroying an entity automatically
destroys all components within it.

Unlike creating entities, each type of component is created and interacted with
through different means.

Much like creating entities, however, component indices are re-used and
components must not be interacted with once deleted through any means.  Creating
components also uses up finite resources; there is a fixed-size pool of
components, and most component types themselves have fixed-size pools of data
which can be exhausted.

#### Camera components

TODO

#### Selectable components

While you are unlikely to be directly creating or interacting with selectable
components, many other component types internally use them and expose their
states/callbacks so it is important to understand what they do.

Selectable components accept pointer input (mice, touch, lightguns, etc.) and
can provide the typical button-like interactions expected.

A selectable component can be in the following states:

| Name       | Description                                                                                                                                                                                                                                                                     |
| ---------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Idle       | The selectable component appears inactive, but possibly interactive.                                                                                                                                                                                                            |
| Hovering   | The selectable component appears partially activated; for example, without actively selecting it, the pointer may be hovering over it, or the player may have navigated to it.                                                                                                  |
| Confirming | The selectable component is likely to be selected, but is awaiting confirmation from the user; for example, the mouse button may be held down, and while releasing it would trigger selection, dragging the mouse away would cause the component to return to the "idle" state. |
| Selected   | The selectable component was selected by the user.  The "idle" state should be shown should further selections be possible.                                                                                                                                                     |

An optional callback can be provided to the selectable component which is called
whenever a state change occurs.  All selectable components start in the "idle"
state.

Where multiple selectable components cover the same pixel, the closest by Z
depth is considered to be hovered/confirmed/selected/etc. when the pointer
occupies said pixel, even if it lacks a state change callback.

#### Mesh components

TODO

#### Animated mesh components

TODO

#### Billboard components

TODO

#### Animated billboard components

TODO

#### Tick components

TODO

#### Timer components

TODO

#### Navigation mesh components

TODO

Navigation meshes are a core feature of Indifference Engine.  Much like in other
engines, they are surfaces covering the ground indicating where AI is able to
navigate:

TODO: image

Unlike in other engines, however, they also define where _players_ can go.  You
can almost imagine that any edges without a neighboring face have an invisible
wall extending upwards into infinity:

TODO: image

##### Structure

TODO

##### Finding closest

TODO

##### Constraining to volumes

TODO

##### Constraining to surfaces

TODO

##### Collision

TODO

##### Navigation

TODO

#### Sound components

TODO

#### Song components

TODO

#### Destroying components

Components of any type can be destroyed by calling `destroy_component` and
ensuring that you do not keep any references to them:

```c
destroy_component(previously_created_component);
previously_created_component = INDEX_NONE;
```

#### Creating custom components

TODO

## Assets

TODO

### *.tga files

Files with an extension of ".tga" will be automatically converted into texture
factories which can be used directly within your C code.

For example, the file name `source/game/path/to/your_file.tga` can be imported
using the following:

```c
const texture * game_path_to_your_file();
```

At present, this only supports RLE-compressed palletized TGA files.

### *.obj files

TODO

#### Meshes

TODO

#### Navigation meshes

TODO

#### Blender plug-in

Although Blender, like most 3D editors, can natively import from and export to
OBJ format, a [plugin](./blender_obj_plugin.md) has been written to help resolve
minor discontinuities between how Blender and Indifference Engine use the
format.

### *.wav files

TODO

### *.mod files

TODO

## Project Settings

TODO
