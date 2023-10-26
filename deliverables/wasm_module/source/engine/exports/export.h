#ifndef EXPORT_H

#define EXPORT_H

/**
 * Exports the following declaration from the WASM module.
 */
#define EXPORT __attribute__((visibility("default")))

#endif
