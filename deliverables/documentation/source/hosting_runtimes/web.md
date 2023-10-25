# Web hosting runtime

This runtime bundles the WASM module alongside static files which form a hosting
runtime when distributed via a HTTPS web server.

It can be installed as a Progressive Web Application, appearing as a standalone
application in some contexts.

## Installing and configuring dependencies

These instructions assume that you are running Windows 10 or 11 without any
development tools installed, and that you have a local copy of this repository.

- Run the [NodeJS installer](https://nodejs.org/en).  The latest LTS version is
  recommended.  All of the default settings should suffice.
- Perform a production build of the
  @ref deliverables/documentation/source/wasm_module/index.md "WASM module".
- Open a terminal in the `deliverables/hosting_runtimes/web` directory.
- Type `npm install` to install further dependencies.

## Building

- Open a terminal in the `deliverables/hosting_runtimes/web` directory.
- Type `npm run-script build`.
- The static files to be distributed via the HTTPS web server can be found in
  the `deliverables/hosting_runtimes/web/ephemeral/build` directory.
- Additionally, a zip archive of those static files can be found at
  `deliverables/hosting_runtimes/web/ephemeral/build.zip`.

## Local Testing

You will not be able to use the static files generated in the above steps
directly from disk (`file://`).  A basic HTTP web server is included however,
and can be ran using the following steps:

- Open a terminal in the `deliverables/hosting_runtimes/web` directory.
- Type `npm run-script host`.

This will host the most recent build at
[http://127.0.0.1:8080](http://127.0.0.1:8080) until `Ctrl+C` is pressed in the
terminal window.

## Branding and configuration

Branding and configuration can be changed by editing
`deliverables/hosting_runtimes/web/scripts/build.js`.

Look for the following:

| Item                               | Description                                                                                                                            |
| ---------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| `appName`                          | This should be the name of the game.                                                                                                   |
| `appShortName`                     | This should the short form of the name of the game, for example, on the iOS home screen.  Usually this will be identical to `appName`. |
| `appDescription`                   | Brief description of the game; one or two short sentences.                                                                             |
| `developerName`                    | The name by which the game's developer is known.                                                                                       |
| `developerURL`                     | The game developer's website.                                                                                                          |
| `scope`                            | TODO.                                                                                                                                  |
| `startURL`                         | TODO.                                                                                                                                  |
| `pixel_art`                        | Set to `false` when the game's logo is not pixel art.                                                                                  |
| `logo.gif`                         | The game's logo; used as the app icon.                                                                                                 |
| `INSERT-CACHE-PREFIX-HERE`         | A prefix applied to all service worker cache items.  Likely the name of the game.                                                      |
| `INSERT-LOCAL-STORAGE-PREFIX-HERE` | A prefix applied to all local storage items (save data).  Likely the name of the game.                                                 |

#### Linting

[ESLint](https://eslint.org/) is configured for this hosting runtime to find
potential mistakes and enforce code style.  It can be ran using the following
steps:

- Open a terminal in the `deliverables/hosting_runtimes/web` directory.
- Type `npm run-script lint` to list any problems which should be fixed.
- Type `npm run-script lint-fix` to fix all problems which can be fixed
  automatically.
