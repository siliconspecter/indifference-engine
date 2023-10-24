# Generating documentation

These instructions detail how to generate this documentation on your own system.

## Installing dependencies

These instructions assume that you are running Windows 10 or 11 without any
development tools installed.

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

## Generating documentation

- Open a terminal in the `deliverables/documentation` directory.
- Type `doxygen`.
- The generated API documentation can be found at
  `deliverables/documentation/ephemeral`.
