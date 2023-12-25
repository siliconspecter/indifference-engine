#include "../primitives/f32.h"
#include "../primitives/index.h"
#include "vector.h"

void add_vectors(
    const vector augend,
    const vector addend,
    vector sum)
{
  add_f32s_f32s(augend, addend, sum, VECTOR_COMPONENTS);
}

void subtract_vectors(
    const vector minuend,
    const vector subtrahend,
    vector difference)
{
  subtract_f32s_f32s(minuend, subtrahend, difference, VECTOR_COMPONENTS);
}

void multiply_vector_by_scalar(
    const vector multiplier,
    const f32 multiplicand,
    vector product)
{
  multiply_f32s_f32(multiplier, multiplicand, product, VECTOR_COMPONENTS);
}

f32 dot_product(
    const vector multiplier,
    const vector multiplicand)
{
  f32 product = 0;

  for (index component = 0; component < VECTOR_COMPONENTS; component++)
  {
    product += multiplier[component] * multiplicand[component];
  }

  return product;
}

f32 distance_squared(
    const vector origin,
    const vector destination)
{
  f32 distance = 0;

  for (index component = 0; component < VECTOR_COMPONENTS; component++)
  {
    const f32 difference = destination[component] - origin[component];
    distance += difference * difference;
  }

  return distance;
}

void linearly_interpolate_vectors_by_scalar(
    const vector origin,
    const f32 progress,
    const vector destination,
    vector interpolated)
{
  linearly_interpolate_f32s_f32s_f32(origin, destination, progress, 1 - progress, interpolated, VECTOR_COMPONENTS);
}
