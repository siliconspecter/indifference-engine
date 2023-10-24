# WASM module conventions

American English in `snake_case` is used for declaration names.

Preprocessor macro names are in `UPPER_CASE` while everything else is in
`lower_case`.

Lines are kept to 80 characters in length where possible.  Sometimes, IDE
formatting or large singular words make this impossible.

## Namespacing

Header files contain a header guard named `{file name}_H`.

For example, a file named `car.h` might resemble the following:

```c
#ifndef CAR_H

#define CAR_H

// Declarations go here.

#endif
```

## Callbacks

TODO revise
A typedef is created for each callback type, named `{verb}_callback`.  Any
parameters using the callback are named `on_{verb}`.  Verbs are in
non-continuous present tense, e.g. `click` rather than `clicked` or `clicking`.

## Parameter order

Where possible, parameters are ordered as a form of pipeline from left to right.
For example, a function filtering light through a gel to produce a color would
be ordered the same (light, gel, color).

## Memory management

No dynamic memory allocator is available, so all state which must persist
between event handlers must be statically allocated as global variables.  For
small-scale games, which are bounded simulations, this is less of a problem than
may be expected.
