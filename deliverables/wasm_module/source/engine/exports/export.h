#ifndef EXPORT_H

#define EXPORT_H

/**
 * Exports the following declaration from the WASM module.
 */
#define EXPORT __attribute__((visibility("default")))

/**
 * Aligns a variable to a number of bytes.
 * @param to The type to which to align.
 */
#define ALIGN(to) __attribute__((aligned(sizeof(to))))

#endif
