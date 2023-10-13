# [Indifference Engine](../../readme.md)

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
- Type `make`.

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

The [engine/primitives](./deliverables/wasm_module/source/engine/primitives)
directory contains headers for primitive types:

| Type/Header File                              | Description        | Constants                                  |
| --------------------------------------------- | ------------------ | ------------------------------------------ |
| [`s32`](./source/engine/primitives/s32.h)     | Signed WASM `i32`. | `S32_MIN` and `S32_MAX`.                   |
| [`f32`](./source/engine/primitives/f32.h)     | WASM `f32`.        | None.                                      |
| [`index`](./source/engine/primitives/index.h) | Array index.       | `INDEX_MIN`, `INDEX_MAX` and `INDEX_NONE`. |

## Math

The C math library is not available, but a partial substitute is available in
the [engine/math](./source/engine/math) directory.

| Header File                                           | Declarations                                                                                                                                                                   |
| ----------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [`float`](./source/engine/math/float.h)               | `NAN`, `POSITIVE_INFINITY`, `NEGATIVE_INFINITY` and `IS_NAN`.                                                                                                                  |
| [`trigonometry`](./source/engine/math/trigonometry.h) | `PI`, `sin` and `cos`.                                                                                                                                                         |
| [`relational`](./source/engine/math/relational.h)     | `MIN`, `MAX` and `CLAMP`.                                                                                                                                                      |
| [`vector`](./source/engine/math/vector.h)             | `VECTOR_COMPONENTS`, `vector`, `add_vectors`, `multiply_vector_by_scalar`, `subtract_vectors`, `dot_product`, `distance_squared` and `linearly_interpolate_vectors_by_scalar`. |
