# Indifference Engine

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
