#include "../primitives/f32.h"
#include "../primitives/index.h"
#include "vector.h"

void add_vectors(
    const vector augend,
    const vector addend,
    vector sum)
{
  for (index component = 0; component < VECTOR_COMPONENTS; component++)
  {
    sum[component] = augend[component] + addend[component];
  }
}

void subtract_vectors(
    const vector minuend,
    const vector subtrahend,
    vector difference)
{
  for (index component = 0; component < VECTOR_COMPONENTS; component++)
  {
    difference[component] = minuend[component] - subtrahend[component];
  }
}

void multiply_vector_by_scalar(
    const vector multiplier,
    const f32 multiplicand,
    vector product)
{
  for (index component = 0; component < VECTOR_COMPONENTS; component++)
  {
    product[component] = multiplier[component] * multiplicand;
  }
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
  const f32 inverse_progress = 1 - progress;

  for (index component = 0; component < VECTOR_COMPONENTS; component++)
  {
    interpolated[component] = origin[component] * inverse_progress + destination[component] * progress;
  }
}
