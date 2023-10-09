# Indifference Engine

## Conventions

American English in `snake_case` is used for declaration names.

Preprocessor macro names are in `UPPER_CASE` while everything else is in
`lower_case`.

Lines are kept to 80 characters in length where possible.  Sometimes, IDE
formatting or large singular words make this impossible.

### Namespacing

All engine-related declarations are prefixed `ie_{file name}` or
`IE_{file name}` as appropriate to reduce the chance of collisions with
declarations made by the game, compiler or libraries.

Additionally, header files are to contain a header guard named
`IE_{file name}_H`.

For example, a file named `car.h` might resemble the following:

```c
#ifndef IE_CAR_H

#define IE_CAR_H

#include "../engine/primitive.h"

/**
 * The value of the car, in dollars.
 */
#define IE_CAR_VALUE_DOLLARS 32000

/**
 * The altitude of the car, in meters.
 */
extern ie_primitive_f32 ie_car_altitude_meters;

/**
 * Drives the car.
 */
void ie_car_drive();

#endif
```

### Callbacks

A typedef is created for each callback type.  It is named
`ie_{file name}_{verb}_callback`.  Any parameters using the callback are
named `on_{verb}`.  Verbs are in non-continuous present tense, e.g. `click`
rather than `clicked` or `clicking`.

## Primitive Types

The [engine/primitive.h](./deliverables/client/source/engine/primitive.h) file
declares the following types which largely map directly to WASM types:

| Type               | Description        | Constants                                                                                                                         |
| ------------------ | ------------------ | --------------------------------------------------------------------------------------------------------------------------------- |
| `ie_primitive_u32` | Unsigned WASM i32. | `IE_PRIMITIVE_U32_MIN` and `IE_PRIMITIVE_U32_MAX`.                                                                                |
| `ie_primitive_s32` | Signed WASM i32.   | `IE_PRIMITIVE_S32_MIN` and `IE_PRIMITIVE_S32_MAX`.                                                                                |
| `ie_primitive_u64` | Unsigned WASM i64. | `IE_PRIMITIVE_U64_MIN` and `IE_PRIMITIVE_U64_MAX`.                                                                                |
| `ie_primitive_s64` | Signed WASM i64.   | `IE_PRIMITIVE_S64_MIN` and `IE_PRIMITIVE_S64_MAX`.                                                                                |
| `ie_primitive_f32` | WASM f32.          | `IE_PRIMITIVE_F32_NAN`, `IE_PRIMITIVE_F32_IS_NAN`, `IE_PRIMITIVE_F32_POSITIVE_INFINITY` and `IE_PRIMITIVE_F32_NEGATIVE_INFINITY`. |
| `ie_primitive_f64` | WASM f64.          | `IE_PRIMITIVE_F64_NAN`, `IE_PRIMITIVE_F64_IS_NAN`, `IE_PRIMITIVE_F64_POSITIVE_INFINITY` and `IE_PRIMITIVE_F64_NEGATIVE_INFINITY`. |

For integers, `ie_primitive_s32` is recommended.  Bugs are common when using the
other types as many operations can interact with the built-in C operators in
unexpected ways (unsigned integers can easily underflow during subtraction,
while operators and literals can unexpectedly truncate 64-bit integers to
32 bits).
