/** @file */

#ifndef F32_H

#define F32_H

#include "../miscellaneous.h"
#include "quantity.h"

/**
 * Represents a WASM f32.
 */
typedef float f32;

ASSERT(f32_size, sizeof(f32) == 4);

/**
 * Copies a f32 over a memory region.
 * @param source The f32 to copy.
 * @param destination The f32s to copy to.
 * @param quantity The number of f32s to copy.
 */
void copy_f32(
    const f32 source,
    f32 *const destination,
    const quantity quantity);

/**
 * Copies f32s from one memory region to another.
 * @param source The f32s to copy from.
 * @param destination The f32s to copy to.
 * @param quantity The number of f32s to copy.
 */
void copy_f32s(
    const f32 *const source,
    f32 *const destination,
    const quantity quantity);

/**
 * Swaps f32s between two memory regions.
 * @param a The first memory region to swap.
 * @param b The second memory region to swap.
 * @param quantity The number of f32s to swap.
 */
void swap_f32s(
    f32 *const a,
    f32 *const b,
    const quantity quantity);

/**
 * Calculates the sums of the f32s in two memory regions.
 * @param augends The f32s to add to.
 * @param addends The f32s to add.
 * @param sums The f32s to write the results to.  May be either or both of the
 *             operands, but may not otherwise overlap with them.
 * @param quantity The number of f32s to add.
 */
void add_f32s_f32s(
    const f32 *const augends,
    const f32 *const addends,
    f32 *const sums,
    const quantity quantity);

/**
 * Calculates the sums of the f32s in a memory region and a f32.
 * @param augends The f32s to add to.
 * @param addend The f32 to add by.
 * @param sums The f32s to write the results to.  May be the augends, but may
 *             not otherwise overlap with it.
 * @param quantity The number of f32s to add.
 */
void add_f32s_f32(
    const f32 *const augends,
    const f32 addend,
    f32 *const sums,
    const quantity quantity);

/**
 * Calculates the differences of the f32s in two memory regions.
 * @param minuends The f32s to subtract from.
 * @param subtrahends The f32s to subtract by.
 * @param differences The f32s to write the results to.  May be either or both
 *                    of the operands, but may not otherwise overlap with them.
 * @param quantity The number of f32s to subtract.
 */
void subtract_f32s_f32s(
    const f32 *const minuends,
    const f32 *const subtrahends,
    f32 *const differences,
    const quantity quantity);

/**
 * Calculates the differences between f32s in a memory regions and a f32.
 * @param minuends The f32s to subtract from.
 * @param subtrahend The f32 to subtract by.
 * @param differences The f32s to write the results to.  May be the minuends,
 *                    but may not otherwise overlap with them.
 * @param quantity The number of f32s to subtract.
 */
void subtract_f32s_f32(
    const f32 *const minuends,
    const f32 subtrahend,
    f32 *const differences,
    const quantity quantity);

/**
 * Calculates the differences between a f32 and the f32s in a memory region.
 * @param minuend The f32 to subtract from.
 * @param subtrahends The f32s to subtract by.
 * @param differences The f32s to write the results to.  May be the subtrahends,
 *                    but may not otherwise overlap with them.
 * @param quantity The number of f32s to subtract.
 */
void subtract_f32_f32s(
    const f32 minuend,
    const f32 *const subtrahends,
    f32 *const differences,
    const quantity quantity);

/**
 * Calculates the products of the f32s in two memory regions.
 * @param multipliers The f32s to multiply.
 * @param multiplicands The f32s to multiply by.
 * @param products The f32s to write the results to.  May be either or both of
 *                 the operands, but may not otherwise overlap with them.
 * @param quantity The number of f32s to multiply.
 */
void multiply_f32s_f32s(
    const f32 *const multipliers,
    const f32 *const multiplicands,
    f32 *const products,
    const quantity quantity);

/**
 * Calculates the products of the f32s in a memory region and a f32.
 * @param multipliers The f32s to multiply.
 * @param multiplicand The f32 to multiply by.
 * @param products The f32s to write the results to.  May be the multipliers,
 *                 but may not otherwise overlap with them.
 * @param quantity The number of f32s to multiply.
 */
void multiply_f32s_f32(
    const f32 *const multipliers,
    const f32 multiplicand,
    f32 *const products,
    const quantity quantity);

/**
 * Calculates the products of a f32 and the f32s in a memory region.
 * @param multiplier The f32 to multiply.
 * @param multiplicands The f32s to multiply by.
 * @param products The f32s to write the results to.  May be the multiplicands,
 *                 but may not otherwise overlap with them.
 * @param quantity The number of f32s to multiply.
 */
void multiply_f32_f32s(
    const f32 multiplier,
    const f32 *const multiplicands,
    f32 *const products,
    const quantity quantity);

/**
 * Performs a multiply then an add on each f32 in a memory region.
 * @param multipliers The f32s to multiply.
 * @param multiplicands The f32s to multiply by.  May be the multipliers, but
 *                      not otherwise overlap with them.
 * @param addends The f32s to add.  May be the other operands, but may not
 *                otherwise overlap with them.
 * @param results The f32s to write the results to.  May be the operands, but
 *                may not otherwise overlap with them.
 * @param quantity The number of f32s to multiply-add.
 */
void multiply_add_f32s_f32s_f32s(
    const f32 *const multipliers,
    const f32 *const multiplicands,
    const f32 *const addends,
    f32 *const results,
    const quantity quantity);

/**
 * Performs a multiply then an add on each f32 in a memory region.
 * @param multipliers The f32s to multiply.
 * @param multiplicands The f32s to multiply by.  May be the multipliers, but
 *                      not otherwise overlap with them.
 * @param addend The f32 to add.
 * @param results The f32s to write the results to.  May be the operands, but
 *                may not otherwise overlap with them.
 * @param quantity The number of f32s to multiply-add.
 */
void multiply_add_f32s_f32s_f32(
    const f32 *const multipliers,
    const f32 *const multiplicands,
    const f32 addend,
    f32 *const results,
    const quantity quantity);

/**
 * Performs a multiply then an add on each f32 in a memory region.
 * @param multipliers The f32s to multiply.
 * @param multiplicand The f32 to multiply by.
 * @param addends The f32s to add.  May be the multipliers, but not otherwise
 *                overlap with them.
 * @param results The f32s to write the results to.  May be the operands, but
 *                may not otherwise overlap with them.
 * @param quantity The number of f32s to multiply-add.
 */
void multiply_add_f32s_f32_f32s(
    const f32 *const multipliers,
    const f32 multiplicand,
    const f32 *const addends,
    f32 *const results,
    const quantity quantity);

/**
 * Performs a multiply then an add on each f32 in a memory region.
 * @param multipliers The f32s to multiply.
 * @param multiplicand The f32 to multiply by.
 * @param addend The f32 to add.
 * @param results The f32s to write the results to.  May be the multipliers,
 *                but may not otherwise overlap with them.
 * @param quantity The number of f32s to multiply-add.
 */
void multiply_add_f32s_f32_f32(
    const f32 *const multipliers,
    const f32 multiplicand,
    const f32 addend,
    f32 *const results,
    const quantity quantity);

/**
 * Calculates the sum of the f32s in a memory region.
 * @param addends The f32s to sum.
 * @param quantity The number of f32s to sum.
 * @return The sum of the f32s.
 */
f32 sum_f32s(
    const f32 *const addends,
    const quantity quantity);

/**
 * Calculates the absolute of a f32.
 * @param real The value of which to calculate the absolute.
 * @return The absolute of the given value.
 */
f32 absolute_f32(const f32 real);

/**
 * Linearly interpolates between two f32s.
 * @param start The f32 to linearly interpolate from.
 * @param end The f32 to linearly interpolate to.
 * @param progress When 0, "start", when 1, "end".
 * @param inverse_progress 1 - "progress".
 * @return The linearly interpolated f32.
 */
f32 linearly_interpolate_f32_f32_f32(
    const f32 start,
    const f32 end,
    const f32 progress,
    const f32 inverse_progress);

/**
 * Linearly interpolates between the f32s in two memory regions.
 * @param start The f32s to linearly interpolate from.
 * @param end The f32s to linearly interpolate to.
 * @param progress When 0, "start", when 1, "end".
 * @param inverse_progress 1 - "progress".
 * @param results The f32s to write the results to.  May be the start and/or
 *                end, but may not otherwise overlap with them.
 */
void linearly_interpolate_f32s_f32s_f32(
    const f32 *const start,
    const f32 *const end,
    const f32 progress,
    const f32 inverse_progress,
    f32 *const results,
    const quantity quantity);

#endif
