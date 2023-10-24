# Building the WASM module

The WASM module is written using C, using Make as a build tool.

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

- Open a terminal in the `deliverables/wasm_module` directory.
- Type `make production` for an optimized build or `make development` for a fast
  build.
- The built WASM module can be found at
  `deliverables/wasm_module/ephemeral/production/build/module.wasm`
  or `deliverables/wasm_module/ephemeral/development/build/module.wasm`
  respectively.
