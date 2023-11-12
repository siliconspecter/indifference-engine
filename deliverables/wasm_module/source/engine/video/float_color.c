#include "../primitives/f32.h"
#include "../primitives/index.h"
#include "../math/relational.h"
#include "color.h"
#include "float_color.h"

void convert_to_float_color(const color color, float_color float_color)
{
  float_color[0] = ((f32)EXTRACT_RED(color)) * (1.0f / 255.0f);
  float_color[1] = ((f32)EXTRACT_GREEN(color)) * (1.0f / 255.0f);
  float_color[2] = ((f32)EXTRACT_BLUE(color)) * (1.0f / 255.0f);
  float_color[3] = ((f32)EXTRACT_OPACITY(color)) * (1.0f / 255.0f);
}

color convert_to_color(const float_color float_color)
{
  const s32 red = float_color[0] * 255.0f;
  const s32 green = float_color[1] * 255.0f;
  const s32 blue = float_color[2] * 255.0f;
  const s32 opacity = float_color[3] * 255.0f;

  return COLOR(
      CLAMP(red, 0, 255),
      CLAMP(green, 0, 255),
      CLAMP(blue, 0, 255),
      CLAMP(opacity, 0, 255));
}

void add_float_colors(
    const float_color augend,
    const float_color addend,
    float_color sum)
{
  for (index channel = 0; channel < FLOAT_COLOR_CHANNELS; channel++)
  {
    sum[channel] = augend[channel] + addend[channel];
  }
}

void subtract_float_colors(
    const float_color minuend,
    const float_color subtrahend,
    float_color difference)
{
  for (index channel = 0; channel < FLOAT_COLOR_CHANNELS; channel++)
  {
    difference[channel] = minuend[channel] - subtrahend[channel];
  }
}

void multiply_float_color_by_scalar(
    const float_color multiplier,
    const f32 multiplicand,
    float_color product)
{
  for (index channel = 0; channel < FLOAT_COLOR_CHANNELS; channel++)
  {
    product[channel] = multiplier[channel] * multiplicand;
  }
}

void modulate_float_colors(
    const float_color multiplier,
    const float_color multiplicand,
    float_color product)
{
  for (index channel = 0; channel < FLOAT_COLOR_CHANNELS; channel++)
  {
    product[channel] = multiplier[channel] * multiplicand[channel];
  }
}
