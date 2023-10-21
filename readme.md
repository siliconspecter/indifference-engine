![Indifference Engine](./branding/logo_readme.gif)

> We have purposely designed it wrong, as a joke.

The goal of this project is to provide an engine for small-scale retro-styled
games which will be as easy to build and run in fifty years' time as now.  To
achieve this:

- All file formats, programming languages and other tools used are the amongst
  the simplest, longest-established and widely supported.
- Third-party dependencies are kept to a minimum, both at build and run time.
- Full but minimalistic platform abstraction, preferring portability over
  platform-provided features or performance.

The game is split into two components which are packaged together:

- An entirely self-contained [WASM module](./deliverables/wasm_module/readme.md)
  containing all logic and data needed to run the game to completion.
- A platform-specific runtime which hosts the WASM module.

## Protocol

### Buffers

After loading the WASM module, the runtime executes following functions exported
by the WASM module to discover a set of buffers used to communicate between the
WASM module and its hosting runtime:

| Export               | Description                                                                                                                                                                                                                         |
| -------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `buffer_count`       | Returns an `i32` specifying the number of buffers which exist.                                                                                                                                                                      |
| `buffer_pointers`    | Returns a pointer to a packed sequence of pointers (one per buffer, each indicating the start of a buffer).  Must be aligned to a multiple of the size of a pointer.                                                                |
| `buffer_sizes`       | Returns a pointer to a packed sequence of `i32`s (one per buffer, each indicating the size of a buffer in bytes, in the same order as that returned by `buffer_pointers`).  Must be aligned to a multiple of the size of an `i32`.  |
| `buffer_identifiers` | Returns a pointer to a packed sequence of `i32`s (one per buffer, each uniquely identifying a buffer (see below), in the same order as that returned by `buffer_pointers`).  Must be aligned to a multiple of the size of an `i32`. |

#### Identifiers

##### Game State

Buffer identifiers of `0` or greater represent game state which the hosting
runtime is permitted to persist a copy of when the game requests it (see below).
When the game next starts, the hosting runtime is permitted to overwrite the
memory for these buffers with the copy it took, even if the WASM module has been
updated.  Buffers which have changed in size will NOT be overwritten.

##### Other

| Identifier | Type  | Count                                            | Pointer aligned to multiple of | Description                                                                                                                                                                                                                                                                   |
| ---------- | ----- | ------------------------------------------------ | ------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `-1`       | `i32` | 1.                                               | Size of `i32`.                 | The number of audio buffers per second.                                                                                                                                                                                                                                       |
| `-2`       | `i32` | 1.                                               | Size of `i32`.                 | The number of audio samples per buffer.                                                                                                                                                                                                                                       |
| `-3`       | `f32` | 2 * number of audio samples per buffer.          | Size of `f32`.                 | The audio output buffer, interleaved left-right where each sample ranges from -1 to 1.                                                                                                                                                                                        |
| `-4`       | `i32` | 1.                                               | Size of `i32`.                 | The height of the video buffer, in pixel rows.                                                                                                                                                                                                                                |
| `-5`       | `i32` | 1.                                               | Size of `i32`.                 | The width of the video buffer, in pixel columns.                                                                                                                                                                                                                              |
| `-6`       | `i32` | Height of video buffer * height of video buffer. | Size of `i32`.                 | The video output buffer, running from left to right, then top to bottom.  The bytes in each value, from least to most significant, represent the red, green and blue intensity where 0 is dark and 255 is bright, then the opacity, where 0 is transparent and 255 is opaque. |
| `-7`       | `i32` | 1.                                               | Size of `i32`.                 | The hosting runtime sets this value to state of the pointing device before calling event handlers (see below).                                                                                                                                                                |
| `-8`       | `f32` | 1.                                               | Size of `f32`.                 | The hosting runtime sets this value to the location of the pointing device, in pixel rows down from the top of the video buffer when it is known, otherwise undefined (see below).  May fall outside the bounds of the video buffer.                                          |
| `-9`       | `f32` | 1.                                               | Size of `f32`.                 | The hosting runtime sets this value to the location of the pointing device, in pixel columns right from the left of the video buffer when it is known, otherwise undefined (see below).  May fall outside the bounds of the video buffer.                                     |
| `-10`      | `i32` | 1.                                               | Size of `i32`.                 | The hosting runtime sets this value to `0` before calling event handlers (see below).  If the event handler changed this to `1`, the hosting runtime is permitted to persist all game state buffers, otherwise, it is not.                                                    |
| `-11`      | `f32` | 1.                                               | Size of `f32`.                 | The hosting runtime sets this value before calling `video` (see below).  This indicates the progress through the current audio buffer, where `0` is the start and `1` is the end.                                                                                             |
| `-12`      | `i32` | 1 per controller.                                | Size of `i32`.                 | The hosting runtime sets each value within to the corresponding controller's state before calling `tick` (see below).                                                                                                                                                         |
| `-13`      | `f32` | 1 per controller.                                | Size of `f32`.                 | The hosting runtime sets each value within to the corresponding controller's X axis value before calling `tick`, where `-1` is fully left, `0` is centered and `1` is fully right.                                                                                            |
| `-14`      | `f32` | 1 per controller.                                | Size of `f32`.                 | The hosting runtime sets each value within to the corresponding controller's Y axis value before calling `tick`, where `-1` is fully down, `0` is centered and `1` is fully up.                                                                                               |

TODO: Required identifiers
TODO: sizes

###### Pointing Device States

| State | Location Known | Selecting | Description                                                                                   |
| ----- | -------------- | --------- | --------------------------------------------------------------------------------------------- |
| `0`   | No.            | No.       | No pointing device is known to be available.                                                  |
| `1`   | Yes.           | No.       | The pointing device is a mouse.  Its location is known but it is not currently clicking.      |
| `2`   | Yes.           | Yes.      | The pointing device is a mouse.  Its location is known and the button is currently held down. |

###### Controller States

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

### Events

The WASM module additionally exports the following functions which represent
event handlers:

| Export  | Description                                                                                                                                                                               |
| ------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `audio` | Executed once per audio buffer; populates the audio output buffer.  Usually advances game state for a fixed time step.                                                                    |
| `video` | Executed each time the game needs to be displayed, to populate the video output buffer.  May be called any number of times per call to `audio`, or even before the first call to `audio`. |
