#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef __MINGW32__
#include <fcntl.h>
#endif

static void *malloc_or_exit(const char *error_message, size_t size)
{
  void *output = malloc(size);

  if (output == NULL)
  {
    fprintf(stderr, "%s\n", error_message);
    exit(1);
  }
  else
  {
    return output;
  }
}

static uint8_t read_u8(const char *error_message)
{
  uint8_t output;

  if (fread(&output, sizeof(uint8_t), 1, stdin))
  {
    return output;
  }
  else
  {
    fprintf(stderr, "%s\n", error_message);
    exit(1);
  }
}

static int8_t read_s8(const char *error_message)
{
  int8_t output;

  if (fread(&output, sizeof(int8_t), 1, stdin))
  {
    return output;
  }
  else
  {
    fprintf(stderr, "%s\n", error_message);
    exit(1);
  }
}

static const uint8_t *read_u8s(const char *error_message, const size_t quantity)
{
  uint8_t *output = malloc_or_exit(error_message, sizeof(uint8_t) * quantity);

  if (fread(output, sizeof(uint8_t), quantity, stdin) == quantity)
  {
    return output;
  }
  else
  {
    fprintf(stderr, "%s\n", error_message);
    exit(1);
  }
}

static uint16_t read_u16(const char *error_message)
{
  uint8_t first_byte = read_u8(error_message);
  uint8_t second_byte = read_u8(error_message);

  return first_byte | (second_byte << 8);
}

static void write_stdout(const char *error_message, const char *format, ...)
{
  va_list args;
  va_start(args, format);

  if (vfprintf(stdout, format, args) <= 0)
  {
    va_end(args);
    fprintf(stderr, "%s\n", error_message);
    exit(1);
  }
  else
  {
    va_end(args);
  }
}

static void reopen(const char *error_message, const char *mode, FILE *file)
{
#ifdef __MINGW32__
  (void)(mode);

  if (_setmode(_fileno(file), O_BINARY) == -1)
#else
  if (freopen(NULL, mode, file) == NULL)
#endif
  {
#ifdef errno
    fprintf(stderr, "%d\n", errno);
#endif
    fprintf(stderr, "%s\n", error_message);
    exit(1);
  }
}

int main(int argc, char **argv)
{
  reopen("Failed to switch standard input to binary.", "rb", stdin);
  reopen("Failed to switch standard output to binary.", "wb", stdout);

  if (argc != 5)
  {
    fprintf(stderr, "Expected 5 arguments, actual %d.\n", argc);
    exit(1);
  }

  uint8_t id_length = read_u8("Failed to read the length of the ID length field.");

  if (id_length)
  {
    fprintf(stderr, "ID length %d is unsupported (only 0).\n", id_length);
    exit(1);
  }

  uint8_t color_map_type = read_u8("Failed to read the color map type.");

  if (color_map_type != 1)
  {
    fprintf(stderr, "Color map type %d is unsupported (only 1).\n", color_map_type);
    exit(1);
  }

  uint8_t image_type = read_u8("Failed to read the image map type.");

  if (image_type != 9)
  {
    fprintf(stderr, "Image type %d is unsupported (only 9).\n", image_type);
    exit(1);
  }

  uint16_t color_map_specification_first_entry_index = read_u16("Failed to read the first entry index from the color map specification.");

  if (color_map_specification_first_entry_index)
  {
    fprintf(stderr, "Color map specification first entry index %d is unsupported (only 0).\n", color_map_specification_first_entry_index);
    exit(1);
  }

  uint16_t color_map_specification_color_map_length = read_u16("Failed to read the color map length from the color map specification.");

  if (!color_map_specification_color_map_length)
  {
    fprintf(stderr, "Color map specification length %d is unsupported (must be at least 1).\n", color_map_specification_color_map_length);
    exit(1);
  }

  uint8_t color_map_specification_color_map_entry_size = read_u8("Failed to read the color map entry size from the color map specification.");

  if (!color_map_specification_color_map_entry_size)
  {
    fprintf(stderr, "Color map specification entry size %d is unsupported (only 32).\n", color_map_specification_color_map_entry_size);
    exit(1);
  }

  uint16_t image_specification_x_origin = read_u16("Failed to read the x origin from the image specification.");

  if (image_specification_x_origin)
  {
    fprintf(stderr, "Image specification x origin %d is unsupported (only 0).\n", image_specification_x_origin);
    exit(1);
  }

  uint16_t image_specification_y_origin = read_u16("Failed to read the y origin from the image specification.");

  if (image_specification_y_origin)
  {
    fprintf(stderr, "Image specification y origin %d is unsupported (only 0).\n", image_specification_y_origin);
    exit(1);
  }

  uint16_t image_specification_width = read_u16("Failed to read the width from the image specification.");

  if (!image_specification_width)
  {
    fprintf(stderr, "Image specification width %d is unsupported.\n", image_specification_width);
    exit(1);
  }

  uint16_t image_specification_height = read_u16("Failed to read the height from the image specification.");

  if (!image_specification_height)
  {
    fprintf(stderr, "Image specification height %d is unsupported.\n", image_specification_height);
    exit(1);
  }

  uint8_t image_specification_bits_per_pixel = read_u8("Failed to read the bits per pixel from the image specification.");

  if (image_specification_bits_per_pixel != 8)
  {
    fprintf(stderr, "Image specification bits per pixel %d is unsupported (only 8).\n", image_specification_bits_per_pixel);
    exit(1);
  }

  uint8_t image_specification_image_descriptor = read_u8("Failed to read the image descriptor from the image specification.");

  if (image_specification_image_descriptor != 32)
  {
    fprintf(stderr, "Image specification image descriptor %d is unsupported (only 32).\n", image_specification_image_descriptor);
    exit(1);
  }

  const int color_map_size = (color_map_specification_color_map_length * color_map_specification_color_map_entry_size) / (sizeof(uint8_t) * 8);

  const uint8_t *color_map_data = read_u8s("Failed to read the color map.", color_map_size);

  write_stdout("Failed to write the header.", "#include \"%s\"\n#include \"%s\"\n#include \"%s\"\n\nstatic const f32 %s_reds[] = {", argv[1], argv[2], argv[3], argv[4]);

  float *const greens = malloc_or_exit("Failed to allocate memory for the green channel.", sizeof(float) * image_specification_width * image_specification_height);
  float *const blues = malloc_or_exit("Failed to allocate memory for the blue channel.", sizeof(float) * image_specification_width * image_specification_height);
  float *const opacities = malloc_or_exit("Failed to allocate memory for opacity.", sizeof(float) * image_specification_width * image_specification_height);

  int output_pixels = 0;

  while (output_pixels < image_specification_width * image_specification_height)
  {
    int8_t instruction = read_s8("Failed to read next RLE instruction.");

    if (instruction >= 0)
    {
      instruction++;

      if (output_pixels + instruction > image_specification_width * image_specification_height)
      {
        fprintf(stderr, "Excess pixels in compressed data.\n");
        exit(1);
      }
      else
      {
        while (instruction)
        {
          int offset = (read_u8("Failed to read a color map index.") * color_map_specification_color_map_entry_size) / (sizeof(uint8_t) * 8);

          if (offset >= color_map_size)
          {
            fprintf(stderr, "Out-of-range color map index in compressed data.\n");
            exit(1);
          }

          const float red = ((float)color_map_data[offset + 2]) / 255.0f;
          greens[output_pixels] = ((float)color_map_data[offset + 1]) / 255.0f;
          blues[output_pixels] = ((float)color_map_data[offset]) / 255.0f;
          opacities[output_pixels] = ((float)color_map_data[offset + 3]) / 255.0f;

          write_stdout("Failed to write a red intensity.", "%s%s%f", output_pixels ? "," : "", output_pixels % image_specification_width ? "" : "\n    ", red);
          output_pixels++;
          instruction--;
        }
      }
    }
    else
    {
      instruction += 129;

      int offset = (read_u8("Failed to read the color map index to repeat.") * color_map_specification_color_map_entry_size) / (sizeof(uint8_t) * 8);

      if (offset >= color_map_size)
      {
        fprintf(stderr, "Out-of-range color map index in compressed data.\n");
        exit(1);
      }

      while (instruction)
      {
        if (output_pixels < image_specification_width * image_specification_height)
        {
          const float red = ((float)color_map_data[offset + 2]) / 255.0f;
          greens[output_pixels] = ((float)color_map_data[offset + 1]) / 255.0f;
          blues[output_pixels] = ((float)color_map_data[offset]) / 255.0f;
          opacities[output_pixels] = ((float)color_map_data[offset + 3]) / 255.0f;

          write_stdout("Failed to write a red intensity.", "%s%s%f", output_pixels ? ", " : "", output_pixels % image_specification_width ? "" : "\n    ", red);
          output_pixels++;
          instruction--;
        }
        else
        {
          fprintf(stderr, "Excess pixels in compressed data.\n");
          exit(1);
        }
      }
    }
  }

  write_stdout("Failed to write the footer of the red channel and the header of the green channel.", "};\n\nstatic const f32 %s_greens[] = {", argv[4]);

  for (int pixel = 0; pixel < image_specification_width * image_specification_height; pixel++)
  {
    write_stdout("Failed to write a green intensity.", "%s%s%f", pixel ? ", " : "", pixel % image_specification_width ? "" : "\n    ", greens[pixel]);
  }
  free(greens);

  write_stdout("Failed to write the footer of the green channel and the header of the blue channel.", "};\n\nstatic const f32 %s_blues[] = {", argv[4]);

  for (int pixel = 0; pixel < image_specification_width * image_specification_height; pixel++)
  {
    write_stdout("Failed to write a blue intensity.", "%s%s%f", pixel ? ", " : "", pixel % image_specification_width ? "" : "\n    ", blues[pixel]);
  }
  free(blues);

  write_stdout("Failed to write the footer of the blue channel and the header of the opacities.", "};\n\nstatic const f32 %s_opacities[] = {", argv[4]);

  for (int pixel = 0; pixel < image_specification_width * image_specification_height; pixel++)
  {
    write_stdout("Failed to write an opacity.", "%s%s%f", pixel ? ", " : "", pixel % image_specification_width ? "" : "\n    ", opacities[pixel]);
  }
  free(opacities);

  write_stdout("Failed to write the footer.", "};\n\nstatic const texture * const %s_texture = &((texture){%d, %d, %s_reds, %s_greens, %s_blues, %s_opacities});\n\nconst texture * %s()\n{\n  return %s_texture;\n}\n", argv[4], image_specification_height, image_specification_width, argv[4], argv[4], argv[4], argv[4], argv[4], argv[4]);

  return 0;
}
