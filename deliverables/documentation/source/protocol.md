# Protocol

## Buffers

After loading the WASM module, the runtime executes following functions exported
by the WASM module to discover a set of buffers used to communicate between the
WASM module and its hosting runtime:

| Export               | Description                                                                                                                                                                                                                                                                |
| -------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `buffer_count`       | Returns an `i32` specifying the number of buffers which exist.                                                                                                                                                                                                             |
| `buffer_pointers`    | Returns a pointer to a packed sequence of pointers (one per buffer, each indicating the start of a buffer).  Must be aligned to a multiple of the size of a pointer.  Contents are not permitted to change.                                                                |
| `buffer_sizes`       | Returns a pointer to a packed sequence of `i32`s (one per buffer, each indicating the size of a buffer in bytes, in the same order as that returned by `buffer_pointers`).  Must be aligned to a multiple of the size of an `i32`.  Contents are not permitted to change.  |
| `buffer_identifiers` | Returns a pointer to a packed sequence of `i32`s (one per buffer, each uniquely identifying a buffer (see below), in the same order as that returned by `buffer_pointers`).  Must be aligned to a multiple of the size of an `i32`.  Contents are not permitted to change. |

The hosting runtime is to act as though buffers of an unexpected size do not
exist.

### Identifiers

Buffer identifiers fall into the following ranges, inclusive:

#### 0 to 1073741823 - State

All buffers in this range represent state which the hosting runtime is permitted
to persist a copy of when the game requests it (see below).  When the game next
starts, the hosting runtime is permitted to overwrite the memory for these
buffers with the copy it persisted, even if the WASM module has been updated.
Buffers which have changed in size must NOT be overwritten.

Identifiers `0` to `536870911` are for the game itself, while identifiers
`536870912` to `1073741823` are reserved for the engine.

| Identifier  | Description                                            |
| ----------- | ------------------------------------------------------ |
| `536870912` | Identifier of script to execute at next game start-up. |

#### 1073741824 to 2147483647 - Optional IO

All buffers in this range represent fully optional IO:

- If the hosting runtime does not recognize an identifier within the range, the
  hosting runtime does not read from nor write to it.
- If the WASM module does not list an identifier recognized by the hosting
  runtime within the range, the hosting runtime accepts this and continues
  running the game without attempting to read from nor write to the range.

| Identifier   | Type  | Count                                            | Pointer aligned to multiple of | Description                                                                                                                                                                                                                                                                   |
| ------------ | ----- | ------------------------------------------------ | ------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `1073741824` | `i32` | 1.                                               | Size of `i32`.                 | The number of ticks per second.  Not permitted to change.                                                                                                                                                                                                                     |
| `1073741825` | `i32` | 1.                                               | Size of `i32`.                 | The number of audio samples per tick.  Not permitted to change.                                                                                                                                                                                                               |
| `1073741826` | `f32` | Number of audio samples per buffer.              | Size of `f32`.                 | The audio output buffer, running from earlier to later, where each sample ranges from -1 to 1.                                                                                                                                                                                |
| `1073741827` | `i32` | 1.                                               | Size of `i32`.                 | The height of the video buffer, in pixel rows.  Not permitted to change.  Must be at least 1.                                                                                                                                                                                 |
| `1073741828` | `i32` | 1.                                               | Size of `i32`.                 | The width of the video buffer, in pixel columns.  Not permitted to change.  Must be at least 1.                                                                                                                                                                               |
| `1073741829` | `i32` | 1.                                               | Size of `i32`.                 | The height of the video buffer's safe area, in pixel rows.  Not permitted to change.  Must range between 1 and the height of the video buffer.                                                                                                                                |
| `1073741830` | `i32` | 1.                                               | Size of `i32`.                 | The width of the video buffer's safe area, in pixel columns.  Not permitted to change.  Must range between 1 and the width of the video buffer.                                                                                                                               |
| `1073741831` | `i32` | Height of video buffer * height of video buffer. | Size of `i32`.                 | The video output buffer, running from left to right, then top to bottom.  The bytes in each value, from least to most significant, represent the red, green and blue intensity where 0 is dark and 255 is bright, then the opacity, where 0 is transparent and 255 is opaque. |
| `1073741832` | `i32` | 1.                                               | Size of `i32`.                 | The hosting runtime sets this value to state of the pointing device before calling event handlers (see below).                                                                                                                                                                |
| `1073741833` | `f32` | 1.                                               | Size of `f32`.                 | The hosting runtime sets this value to the location of the pointing device, in pixel rows down from the top of the video buffer when it is known, otherwise undefined (see below).  May fall outside the bounds of the video buffer.                                          |
| `1073741834` | `f32` | 1.                                               | Size of `f32`.                 | The hosting runtime sets this value to the location of the pointing device, in pixel columns right from the left of the video buffer when it is known, otherwise undefined (see below).  May fall outside the bounds of the video buffer.                                     |
| `1073741835` | `i32` | 1.                                               | Size of `i32`.                 | The hosting runtime sets this value to `0` before calling event handlers (see below).  If the event handler changed this to `1` and did not throw a trap, the hosting runtime is permitted to persist all game state buffers, otherwise, it is not.                           |
| `1073741837` | `f32` | 1.                                               | Size of `f32`.                 | The hosting runtime sets this value before calling `video` (see below).  This contains the progress through the current tick, where `0` is the start and `1` is the end.                                                                                                      |
| `1073741838` | `i32` | 1.                                               | Size of `i32`.                 | The hosting runtime sets this value before calling `video` (see below).  This contains the height of the area of the video buffer which will be displayed.  Must range between the height of the video buffer's safe area and that of the video buffer itself.                |
| `1073741839` | `i32` | 1.                                               | Size of `i32`.                 | The hosting runtime sets this value before calling `video` (see below).  This contains the width of the area of the video buffer which will be displayed.  Must range between the width of the video buffer's safe area and that of the video buffer itself.                  |
| `1073741840` | `f32` | 3.                                               | Size of `f32`.                 | The hosting runtime sets this value before calling `audio` (see below).  This contains the position of the listener on the X, Y and Z axes in world space, in meters.                                                                                                         |
| `1073741841` | `f32` | 3.                                               | Size of `f32`.                 | The hosting runtime sets this value before calling `audio` (see below).  This contains a unit vector pointing in the direction of the listener on the X, Y and Z axes in world space.                                                                                         |
| `1073741842` | `i32` | 1 per controller, at least one controller.       | Size of `i32`.                 | The hosting runtime sets each value within to the corresponding controller's state before calling `tick` (see below).                                                                                                                                                         |
| `1073741843` | `f32` | 1 per controller, at least one controller.       | Size of `f32`.                 | The hosting runtime sets each value within to the corresponding controller's Y axis value before calling `tick`, where `-1` is fully down, `0` is centered and `1` is fully up.                                                                                               |
| `1073741844` | `f32` | 1 per controller, at least one controller.       | Size of `f32`.                 | The hosting runtime sets each value within to the corresponding controller's X axis value before calling `tick`, where `-1` is fully left, `0` is centered and `1` is fully right.                                                                                            |
| `1073741845` | `i32` | 1.                                               | Size of `i32`.                 | The hosting runtime sets this value to `0` before calling event handlers (see below).  If the event handler changed this to any other value, this represents an error (see below).                                                                                            |

##### Pointing Device States

| State | Location Known | Selecting | Description                                                                                           |
| ----- | -------------- | --------- | ----------------------------------------------------------------------------------------------------- |
| `0`   | No.            | No.       | No pointing device is known to be available.                                                          |
| `1`   | Yes.           | No.       | The pointing device is a mouse.  Its location is known but it is not currently clicking.              |
| `2`   | Yes.           | Yes.      | The pointing device is a mouse.  Its location is known and its primary button is currently held down. |

##### Controller States

Bits 0 to 7 indicate the type of controller connected, if any.

| Bits 0-7 | Description                           |
| -------- | ------------------------------------- |
| `0`      | No controller connected.              |
| `1`      | Xbox controller connected.            |
| `2`      | PlayStation controller connected.     |
| `3`      | Nintendo Switch controller connected. |

All other values for these bits are reserved, but should be taken to mean that
some kind of controller is connected.

The following bits represent button state, where set bits indicate currently
pressed buttons, and cleared bits indicate currently released buttons.  Their
value is undefined if no controller is connected.

| Bit  | Xbox  | PlayStation              | Nintendo Switch |
| ---- | ----- | ------------------------ | --------------- |
| `8`  | Y     | Triangle                 | X               |
| `9`  | B     | Circle                   | A               |
| `10` | A     | Cross                    | B               |
| `11` | X     | Square                   | Y               |
| `12` | LB/LT | L1/L2                    | L/ZL/SL         |
| `12` | RB/RT | R1/R2                    | R/ZR/SR         |
| `14` | Menu  | Options/Touch pad button | +/-             |

All other bits are reserved for future controller features and are to be
ignored for now.

##### Errors

Positive values are generated by the game, while negative values are generated
by the engine.

| Value | Description            |
| ----- | ---------------------- |
| `0`   | No error has occurred. |

#### -2147483648 to -1073741825 - IO required by hosting runtime

All buffers in this range represent IO required by the hosting runtime:

- If the hosting runtime does not recognize an identifier within the range, the
  hosting runtime does not read from nor write to it.
- If the WASM module does not list an identifier recognized by the hosting
  runtime within the range, the hosting runtime cannot run the game.

Currently nothing is defined in this range.

#### -1073741824 to -1 - IO required by WASM module

All buffers in this range represent IO required by the WASM module:

- If the hosting runtime does not recognize an identifier within the range, the
  hosting runtime cannot run the game.
- If the WASM module does not list an identifier recognized by the hosting
  runtime within the range, the hosting runtime accepts this and continues
  running the game without attempting to read from nor write to the range.

Currently nothing is defined in this range.

## Events

The WASM module additionally exports the following functions which represent
event handlers:

| Export  | Description                                                                                                                                                                             |
| ------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `tick`  | Executed once per tick, to advance game state.                                                                                                                                          |
| `audio` | Executed once per audio buffer per channel; populates the audio output buffer.  May be called any number of times per call to `tick`, or even before the first call to `tick`.          |
| `video` | Executed each time the game needs to be displayed, to populate the video output buffer.  May be called any number of times per call to `tick`, or even before the first call to `tick`. |

If any event handler throws a trap, the hosting runtime must NOT execute any
further event handlers nor persist any buffers until the game is fully
restarted (i.e. all memory reset to its state before the first event handler was
called).
