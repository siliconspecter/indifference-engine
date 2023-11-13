#include "f32.h"
#include "index.h"
#include "quantity.h"

void copy_f32(
    const f32 source,
    f32 *const destination,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    destination[index] = source;
  }
}

void copy_f32s(
    const f32 *const source,
    f32 *const destination,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    destination[index] = source[index];
  }
}

void add_f32s_f32s(
    const f32 *const augends,
    const f32 *const addends,
    f32 *const sums,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    sums[index] = augends[index] + addends[index];
  }
}

void add_f32s_f32(
    const f32 *const augends,
    const f32 addend,
    f32 *const sums,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    sums[index] = augends[index] + addend;
  }
}

void subtract_f32s_f32s(
    const f32 *const minuends,
    const f32 *const subtrahends,
    f32 *const differences,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    differences[index] = minuends[index] - subtrahends[index];
  }
}

void subtract_f32s_f32(
    const f32 *const minuends,
    const f32 subtrahend,
    f32 *const differences,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    differences[index] = minuends[index] - subtrahend;
  }
}

void subtract_f32_f32s(
    const f32 minuend,
    const f32 *const subtrahends,
    f32 *const differences,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    differences[index] = minuend - subtrahends[index];
  }
}

void multiply_f32s_f32s(
    const f32 *const multipliers,
    const f32 *const multiplicands,
    f32 *const products,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    products[index] = multipliers[index] * multiplicands[index];
  }
}

void multiply_f32s_f32(
    const f32 *const multipliers,
    const f32 multiplicand,
    f32 *const products,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    products[index] = multipliers[index] * multiplicand;
  }
}

void multiply_f32_f32s(
    const f32 multiplier,
    const f32 *const multiplicands,
    f32 *const products,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    products[index] = multiplier * multiplicands[index];
  }
}

void multiply_add_f32s_f32s_f32s(
    const f32 *const multipliers,
    const f32 *const multiplicands,
    const f32 *const addends,
    f32 *const results,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    results[index] = multipliers[index] * multiplicands[index] + addends[index];
  }
}

void multiply_add_f32s_f32s_f32(
    const f32 *const multipliers,
    const f32 *const multiplicands,
    const f32 addend,
    f32 *const results,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    results[index] = multipliers[index] * multiplicands[index] + addend;
  }
}

void multiply_add_f32s_f32_f32s(
    const f32 *const multipliers,
    const f32 multiplicand,
    const f32 *const addends,
    f32 *const results,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    results[index] = multipliers[index] * multiplicand + addends[index];
  }
}

void multiply_add_f32s_f32_f32(
    const f32 *const multipliers,
    const f32 multiplicand,
    const f32 addend,
    f32 *const results,
    const quantity quantity)
{
  for (index index = 0; index < quantity; index++)
  {
    results[index] = multipliers[index] * multiplicand + addend;
  }
}

// TODO: alignment?

f32 sum_f32s(
    const f32 *const addends,
    const quantity quantity)
{
  f32 sum = 0;

  for (index index = 0; index < quantity; index++)
  {
    sum += addends[index];
  }

  return sum;
}

f32 absolute_f32(const f32 real)
{
  return real >= 0.0f ? real : -real;
}

f32 linearly_interpolate_f32_f32_f32(
    const f32 start,
    const f32 end,
    const f32 progress,
    const f32 inverse_progress)
{
  return start * inverse_progress + end * progress;
}
