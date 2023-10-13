# Indifference Engine

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

A typedef is created for each callback type, named `{verb}_callback`.  Any
parameters using the callback are named `on_{verb}`.  Verbs are in
non-continuous present tense, e.g. `click` rather than `clicked` or `clicking`.

### Parameter Order

Where possible, parameters are ordered as a form of pipeline from left to right.
For example, a function filtering light through a gel to produce a color would
be ordered the same (light, gel, color).

## Primitive Types

The [engine/primitives](./deliverables/client/source/engine/primitives)
directory contains headers for primitive types:

| Type/Header File                                                  | Description        | Constants                                  |
| ----------------------------------------------------------------- | ------------------ | ------------------------------------------ |
| [`s32`](./deliverables/client/source/engine/primitives/s32.h)     | Signed WASM `i32`. | `S32_MIN` and `S32_MAX`.                   |
| [`f32`](./deliverables/client/source/engine/primitives/f32.h)     | WASM `f32`.        | None.                                      |
| [`index`](./deliverables/client/source/engine/primitives/index.h) | Array index.       | `INDEX_MIN`, `INDEX_MAX` and `INDEX_NONE`. |

## Math

The C math library is not available, but a partial substitute is available in
the [engine/math](./deliverables/client/source/engine/math) directory.

| Header File                                                               | Declarations                                                                                                                                                                   |
| ------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [`float`](./deliverables/client/source/engine/math/float.h)               | `NAN`, `POSITIVE_INFINITY`, `NEGATIVE_INFINITY` and `IS_NAN`.                                                                                                                  |
| [`trigonometry`](./deliverables/client/source/engine/math/trigonometry.h) | `PI`, `sin` and `cos`.                                                                                                                                                         |
| [`relational`](./deliverables/client/source/engine/math/relational.h)     | `MIN`, `MAX` and `CLAMP`.                                                                                                                                                      |
| [`vector`](./deliverables/client/source/engine/math/vector.h)             | `VECTOR_COMPONENTS`, `vector`, `add_vectors`, `multiply_vector_by_scalar`, `subtract_vectors`, `dot_product`, `distance_squared` and `linearly_interpolate_vectors_by_scalar`. |
