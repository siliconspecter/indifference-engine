#include "f32.h"
#include "index.h"
#include "quantity.h"

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
