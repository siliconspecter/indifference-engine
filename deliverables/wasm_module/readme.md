# [![Indifference Engine](../../branding/logo_readme.gif)](../../readme.md) > WASM Module

The WASM Module is written using C, using Make as a build tool.

## Installing and configuring dependencies

These instructions assume that you are running Windows 10 or 11 without any
development tools installed, and that you have a local copy of this repository.

- Run the [Git installer](https://git-scm.com/).  All of the default settings
  should suffice.
- Run the
  [GNU make installer](https://gnuwin32.sourceforge.net/packages/make.htm)
  (Download > Complete package, except sources > Setup).  All of the default
  settings should suffice.
- Run the [LLVM installer](https://releases.llvm.org/).  The latest version
  and all default settings should suffice, other than adding LLVM to the system
  PATH for all users.
- Run the
  [MinGW-W64-binaries installer](https://github.com/niXman/mingw-builds-binaries).
  All default settings should suffice, other than selecting `64 bit`
  architecture.
- Press the Windows key.
- Type `environment`.
- Select `Edit the system environment variables`.
- Click `Environment Variables...`.
- In the upper pane (`User variables for `(name)), double click on the row with
  a `Variable` of `Path`.
- Click `New`.
- Type `C:\Program Files (x86)\GnuWin32\bin` and press enter.
- Click `Ok`.
- Click `Ok`.
- Click `Ok`.
- Restart your machine.

## Building

- Open a terminal in this directory.
- Type `make production` for an optimized build or `make development` for a fast
  build.
- The built WASM module can be found at
  [ephemeral/production/build/module.wasm](./ephemeral/production/build/module.wasm)
  or
  [ephemeral/development/build/module.wasm](./ephemeral/development/build/module.wasm)
  respectively.

## Conventions

American English in `snake_case` is used for declaration names.

Preprocessor macro names are in `UPPER_CASE` while everything else is in
`lower_case`.

Lines are kept to 80 characters in length where possible.  Sometimes, IDE
formatting or large singular words make this impossible.

### Namespacing

Header files contain a header guard named `{file name}_H`.

For example, a file named `car.h` might resemble the following:

```c
#ifndef CAR_H

#define CAR_H

// Declarations go here.

#endif
```

### Callbacks

TODO revise
A typedef is created for each callback type, named `{verb}_callback`.  Any
parameters using the callback are named `on_{verb}`.  Verbs are in
non-continuous present tense, e.g. `click` rather than `clicked` or `clicking`.

### Parameter order

Where possible, parameters are ordered as a form of pipeline from left to right.
For example, a function filtering light through a gel to produce a color would
be ordered the same (light, gel, color).

### Memory management

No dynamic memory allocator is available, so all state which must persist
between event handlers must be statically allocated as global variables.  For
small-scale games, which are bounded simulations, this is less of a problem than
may be expected.

## API documentation

All types, variables, functions and preprocessor macros which form the engine's
API are documented through comments.  You can browse the documentation generated
from these documents by Doxygen
[here](https://siliconspecter.github.io/indifference-engine-api-documentation/).

### Installing dependencies to generate API documentation

These instructions assume that you are running Windows 10 or 11 without any
development tools installed, and that you have a local copy of this repository.

- Run the [Doxygen installer](https://www.doxygen.nl/download.html).  All of the
  default settings should suffice.
- Press the Windows key.
- Type `environment`.
- Select `Edit the system environment variables`.
- Click `Environment Variables...`.
- In the upper pane (`User variables for `(name)), double click on the row with
  a `Variable` of `Path`.
- Click `New`.
- Type `C:\Program Files\doxygen\bin` and press enter.
- Click `Ok`.
- Click `Ok`.
- Click `Ok`.
- Restart your machine.

### Generating API documentation

- Open a terminal in this directory.
- Type `make api_documentation`.
- The generated API documentation can be found at
  [ephemeral/api_documentation](./ephemeral/api_documentation).

## Creating games

What follows is a description of how the engine API is intended to be used to
build your game.

### Persistence

Entries need to be added to the list in
[source/game/state/index.h](./source/game/state/index.h) for each region of
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

### Scripts

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

TODO: adding a new script

### Scenes

There is one scene at a time.  This is a void which can be populated with
entities.  The scene is _not_ persisted as state.

TODO: units, axes, etc.

#### Entities

An entity is, at its core, a transform (location, rotation, scale, etc.) and a
collection of associated components.

##### Creating

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
[source/engine/limits.h](./source/engine/limits.h) to increase the size of the
pool.

##### Transforming

Initially, all entities are given the "identity" transform, that is, equivalent
to being located at 0, 0, 0, rotated by 0, 0, 0 and scaled by 1, 1, 1.

This can be adjusted during scripts, selectable callbacks, tick callbacks, timer
expiry callbacks or animation callbacks (doing so in other situations may
produce unexpected results):

```c
transform(
  location_vector,
  rotation_vector,
  scale_vector,
  entity_transforms[previously_created_entity]
);
```

##### Destroying

Call `destroy_entity` and ensure that you do not keep any references to it:

```c
destroy_entity(previously_created_entity);
previously_created_entity = INDEX_NONE;
```

#### Components

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

##### Camera components

TODO

##### Selectable components

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

##### Opaque mesh components

TODO

##### Animated opaque mesh components

TODO

##### Opaque billboard components

TODO

##### Animated opaque billboard components

TODO

##### Tick components

TODO

##### Timer components

TODO

##### Navigation mesh components

TODO

###### Structure

TODO

###### Finding closest

TODO

###### Constraining to volumes

TODO

###### Constraining to surfaces

TODO

###### Collision

TODO

###### Navigation

TODO

##### Sound components

TODO

##### Song components

TODO

##### Destroying components

Components of any type can be destroyed by calling `destroy_component` and
ensuring that you do not keep any references to them:

```c
destroy_component(previously_created_component);
previously_created_component = INDEX_NONE;
```

##### Creating custom components

TODO

### Assets

TODO

#### `*.tga` files

TODO

#### `*.obj` files

TODO

##### Meshes

TODO

##### Navigation meshes

TODO

##### Blender plug-in

TODO

#### `*.wav` files

TODO

#### `*.mod` files

TODO
