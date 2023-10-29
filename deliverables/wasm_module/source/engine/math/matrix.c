#include "matrix.h"
#include "relational.h"
#include "trigonometry.h"

const matrix identity_matrix = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}};

culled_by model_to_clip(
    const vector model,
    const matrix model_view_projection,
    vector clip)
{
  const f32 model_x = model[0];
  const f32 model_y = model[1];
  const f32 model_z = model[2];

  const f32 w = model_view_projection[3][0] * model_x + model_view_projection[3][1] * model_y + model_view_projection[3][2] * model_z + model_view_projection[3][3];

  if (w == 0.0f)
  {
    return CULLED_BY_W_ZERO;
  }
  else
  {
    const f32 w_reciprocal = 1.0f / w;

    const f32 z = (model_view_projection[2][0] * model_x + model_view_projection[2][1] * model_y + model_view_projection[2][2] * model_z + model_view_projection[2][3]) * w_reciprocal;

    if (z < -1.0f)
    {
      return CULLED_BY_Z_FAR;
    }
    else if (z > 1.0f)
    {
      return CULLED_BY_Z_NEAR;
    }
    else
    {
      clip[0] = (model_view_projection[0][0] * model_x + model_view_projection[0][1] * model_y + model_view_projection[0][2] * model_z + model_view_projection[0][3]) * w_reciprocal;
      clip[1] = (model_view_projection[1][0] * model_x + model_view_projection[1][1] * model_y + model_view_projection[1][2] * model_z + model_view_projection[1][3]) * w_reciprocal;
      clip[2] = z;

      return CULLED_BY_NOTHING;
    }
  }
}

culled_by clip_to_world(
    const vector clip,
    const matrix inverse_model_view,
    vector world)
{
  const f32 clip_x = clip[0];
  const f32 clip_y = clip[1];
  const f32 clip_z = clip[2];

  const f32 w = inverse_model_view[3][0] * clip_x + inverse_model_view[3][1] * clip_y + inverse_model_view[3][2] * clip_z + inverse_model_view[3][3];

  if (w == 0.0f)
  {
    return CULLED_BY_W_ZERO;
  }
  else
  {
    const f32 w_reciprocal = 1.0f / w;

    world[0] = (inverse_model_view[0][0] * clip_x + inverse_model_view[0][1] * clip_y + inverse_model_view[0][2] * clip_z + inverse_model_view[0][3]) * w_reciprocal;
    world[1] = (inverse_model_view[1][0] * clip_x + inverse_model_view[1][1] * clip_y + inverse_model_view[1][2] * clip_z + inverse_model_view[1][3]) * w_reciprocal;
    world[2] = (inverse_model_view[2][0] * clip_x + inverse_model_view[2][1] * clip_y + inverse_model_view[2][2] * clip_z + inverse_model_view[2][3]) * w_reciprocal;

    return CULLED_BY_NOTHING;
  }
}

void multiply_matrices(
    const matrix multiplier,
    const matrix multiplicand,
    matrix product)
{
  const f32 multiplier_0_0 = multiplier[0][0];
  const f32 multiplier_0_1 = multiplier[0][1];
  const f32 multiplier_0_2 = multiplier[0][2];
  const f32 multiplier_0_3 = multiplier[0][3];
  const f32 multiplier_1_0 = multiplier[1][0];
  const f32 multiplier_1_1 = multiplier[1][1];
  const f32 multiplier_1_2 = multiplier[1][2];
  const f32 multiplier_1_3 = multiplier[1][3];
  const f32 multiplier_2_0 = multiplier[2][0];
  const f32 multiplier_2_1 = multiplier[2][1];
  const f32 multiplier_2_2 = multiplier[2][2];
  const f32 multiplier_2_3 = multiplier[2][3];
  const f32 multiplier_3_0 = multiplier[3][0];
  const f32 multiplier_3_1 = multiplier[3][1];
  const f32 multiplier_3_2 = multiplier[3][2];
  const f32 multiplier_3_3 = multiplier[3][3];

  const f32 multiplicand_0_0 = multiplicand[0][0];
  const f32 multiplicand_0_1 = multiplicand[0][1];
  const f32 multiplicand_0_2 = multiplicand[0][2];
  const f32 multiplicand_0_3 = multiplicand[0][3];
  const f32 multiplicand_1_0 = multiplicand[1][0];
  const f32 multiplicand_1_1 = multiplicand[1][1];
  const f32 multiplicand_1_2 = multiplicand[1][2];
  const f32 multiplicand_1_3 = multiplicand[1][3];
  const f32 multiplicand_2_0 = multiplicand[2][0];
  const f32 multiplicand_2_1 = multiplicand[2][1];
  const f32 multiplicand_2_2 = multiplicand[2][2];
  const f32 multiplicand_2_3 = multiplicand[2][3];
  const f32 multiplicand_3_0 = multiplicand[3][0];
  const f32 multiplicand_3_1 = multiplicand[3][1];
  const f32 multiplicand_3_2 = multiplicand[3][2];
  const f32 multiplicand_3_3 = multiplicand[3][3];

  product[0][0] = multiplicand_0_0 * multiplier_0_0 + multiplicand_1_0 * multiplier_0_1 + multiplicand_2_0 * multiplier_0_2 + multiplicand_3_0 * multiplier_0_3;
  product[0][1] = multiplicand_0_1 * multiplier_0_0 + multiplicand_1_1 * multiplier_0_1 + multiplicand_2_1 * multiplier_0_2 + multiplicand_3_1 * multiplier_0_3;
  product[0][2] = multiplicand_0_2 * multiplier_0_0 + multiplicand_1_2 * multiplier_0_1 + multiplicand_2_2 * multiplier_0_2 + multiplicand_3_2 * multiplier_0_3;
  product[0][3] = multiplicand_0_3 * multiplier_0_0 + multiplicand_1_3 * multiplier_0_1 + multiplicand_2_3 * multiplier_0_2 + multiplicand_3_3 * multiplier_0_3;
  product[1][0] = multiplicand_0_0 * multiplier_1_0 + multiplicand_1_0 * multiplier_1_1 + multiplicand_2_0 * multiplier_1_2 + multiplicand_3_0 * multiplier_1_3;
  product[1][1] = multiplicand_0_1 * multiplier_1_0 + multiplicand_1_1 * multiplier_1_1 + multiplicand_2_1 * multiplier_1_2 + multiplicand_3_1 * multiplier_1_3;
  product[1][2] = multiplicand_0_2 * multiplier_1_0 + multiplicand_1_2 * multiplier_1_1 + multiplicand_2_2 * multiplier_1_2 + multiplicand_3_2 * multiplier_1_3;
  product[1][3] = multiplicand_0_3 * multiplier_1_0 + multiplicand_1_3 * multiplier_1_1 + multiplicand_2_3 * multiplier_1_2 + multiplicand_3_3 * multiplier_1_3;
  product[2][0] = multiplicand_0_0 * multiplier_2_0 + multiplicand_1_0 * multiplier_2_1 + multiplicand_2_0 * multiplier_2_2 + multiplicand_3_0 * multiplier_2_3;
  product[2][1] = multiplicand_0_1 * multiplier_2_0 + multiplicand_1_1 * multiplier_2_1 + multiplicand_2_1 * multiplier_2_2 + multiplicand_3_1 * multiplier_2_3;
  product[2][2] = multiplicand_0_2 * multiplier_2_0 + multiplicand_1_2 * multiplier_2_1 + multiplicand_2_2 * multiplier_2_2 + multiplicand_3_2 * multiplier_2_3;
  product[2][3] = multiplicand_0_3 * multiplier_2_0 + multiplicand_1_3 * multiplier_2_1 + multiplicand_2_3 * multiplier_2_2 + multiplicand_3_3 * multiplier_2_3;
  product[3][0] = multiplicand_0_0 * multiplier_3_0 + multiplicand_1_0 * multiplier_3_1 + multiplicand_2_0 * multiplier_3_2 + multiplicand_3_0 * multiplier_3_3;
  product[3][1] = multiplicand_0_1 * multiplier_3_0 + multiplicand_1_1 * multiplier_3_1 + multiplicand_2_1 * multiplier_3_2 + multiplicand_3_1 * multiplier_3_3;
  product[3][2] = multiplicand_0_2 * multiplier_3_0 + multiplicand_1_2 * multiplier_3_1 + multiplicand_2_2 * multiplier_3_2 + multiplicand_3_2 * multiplier_3_3;
  product[3][3] = multiplicand_0_3 * multiplier_3_0 + multiplicand_1_3 * multiplier_3_1 + multiplicand_2_3 * multiplier_3_2 + multiplicand_3_3 * multiplier_3_3;
}

void perspective(
    const f32 width,
    const f32 height,
    const f32 focal_length,
    const f32 shift_x,
    const f32 shift_y,
    const f32 clip_start,
    const f32 clip_end,
    const f32 sensor_size,
    matrix forward,
    matrix inverse)
{
  const f32 greatest_dimension = MAX(width, height);
  const f32 width_reciprocal = 1 / width;
  const f32 height_reciprocal = 1 / height;
  const f32 greatest_dimension_reciprocal = MIN(width_reciprocal, height_reciprocal);
  const f32 double_greatest_dimension = greatest_dimension * 2;
  const f32 focal_length_reciprocal = 1 / focal_length;
  const f32 b = sensor_size * focal_length_reciprocal * greatest_dimension_reciprocal;
  const f32 half_b = 0.5 * b;
  const f32 half_b_reciprocal = 1 / half_b;
  const f32 c = clip_start - clip_end;
  const f32 c_reciprocal = 1 / c;
  const f32 d = clip_start * clip_end * 2;
  const f32 d_reciprocal = 1 / d;
  const f32 e = clip_end + clip_start;
  forward[0][0] = width_reciprocal * half_b_reciprocal;
  forward[0][1] = 0;
  forward[0][2] = shift_x * double_greatest_dimension * width_reciprocal;
  forward[0][3] = 0;
  forward[1][0] = 0;
  forward[1][1] = height_reciprocal * half_b_reciprocal;
  forward[1][2] = shift_y * double_greatest_dimension * height_reciprocal;
  forward[1][3] = 0;
  forward[2][0] = 0;
  forward[2][1] = 0;
  forward[2][2] = e * c_reciprocal;
  forward[2][3] = d * c_reciprocal;
  forward[3][0] = 0;
  forward[3][1] = 0;
  forward[3][2] = -1;
  forward[3][3] = 0;
  inverse[0][0] = width * half_b;
  inverse[0][1] = 0;
  inverse[0][2] = 0;
  inverse[0][3] = shift_x * sensor_size * focal_length_reciprocal;
  inverse[1][0] = 0;
  inverse[1][1] = height * half_b;
  inverse[1][2] = 0;
  inverse[1][3] = shift_y * sensor_size * focal_length_reciprocal;
  inverse[2][0] = 0;
  inverse[2][1] = 0;
  inverse[2][2] = 0;
  inverse[2][3] = -1;
  inverse[3][0] = 0;
  inverse[3][1] = 0;
  inverse[3][2] = c * d_reciprocal;
  inverse[3][3] = e * d_reciprocal;
}

void model(
    const vector location,
    const vector rotation,
    const vector scale,
    matrix forward,
    matrix inverse)
{
  const f32 scale_x_reciprocal = 1 / scale[0];
  const f32 scale_y_reciprocal = 1 / scale[1];
  const f32 scale_z_reciprocal = 1 / scale[2];
  const f32 sin_x = sine(rotation[0]);
  const f32 cos_x = cosine(rotation[0]);
  const f32 sin_y = sine(rotation[1]);
  const f32 cos_y = cosine(rotation[1]);
  const f32 sin_z = sine(rotation[2]);
  const f32 cos_z = cosine(rotation[2]);
  const f32 a = scale[1] * cos_x;
  const f32 b = -scale[2] * sin_x;
  const f32 c = scale[1] * sin_x;
  const f32 d = scale[2] * cos_x;
  const f32 e = scale[0] * cos_y;
  const f32 f = c * sin_y;
  const f32 g = d * sin_y;
  const f32 h = sin_x * sin_y;
  const f32 i = cos_x * sin_y;
  const f32 xx = cos_y * cos_z * scale_x_reciprocal;
  const f32 xy = cos_y * sin_z * scale_x_reciprocal;
  const f32 xz = -sin_y * scale_x_reciprocal;
  const f32 yx = (h * cos_z - cos_x * sin_z) * scale_y_reciprocal;
  const f32 yy = (cos_x * cos_z + h * sin_z) * scale_y_reciprocal;
  const f32 yz = sin_x * cos_y * scale_y_reciprocal;
  const f32 zx = (i * cos_z + sin_x * sin_z) * scale_z_reciprocal;
  const f32 zy = (-sin_x * cos_z + i * sin_z) * scale_z_reciprocal;
  const f32 zz = cos_x * cos_y * scale_z_reciprocal;
  forward[0][0] = e * cos_z;
  forward[0][1] = f * cos_z - a * sin_z;
  forward[0][2] = g * cos_z - b * sin_z;
  forward[0][3] = location[0];
  forward[1][0] = e * sin_z;
  forward[1][1] = a * cos_z + f * sin_z;
  forward[1][2] = b * cos_z + g * sin_z;
  forward[1][3] = location[1];
  forward[2][0] = -scale[0] * sin_y;
  forward[2][1] = c * cos_y;
  forward[2][2] = d * cos_y;
  forward[2][3] = location[2];
  forward[3][0] = 0;
  forward[3][1] = 0;
  forward[3][2] = 0;
  forward[3][3] = 1;
  inverse[0][0] = xx;
  inverse[0][1] = xy;
  inverse[0][2] = xz;
  inverse[0][3] = -(location[0] * xx + location[1] * xy + location[2] * xz);
  inverse[1][0] = yx;
  inverse[1][1] = yy;
  inverse[1][2] = yz;
  inverse[1][3] = -(location[0] * yx + location[1] * yy + location[2] * yz);
  inverse[2][0] = zx;
  inverse[2][1] = zy;
  inverse[2][2] = zz;
  inverse[2][3] = -(location[0] * zx + location[1] * zy + location[2] * zz);
  inverse[3][0] = 0;
  inverse[3][1] = 0;
  inverse[3][2] = 0;
  inverse[3][3] = 1;
}
