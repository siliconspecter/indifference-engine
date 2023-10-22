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
