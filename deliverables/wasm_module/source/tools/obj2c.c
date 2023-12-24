#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>

static const char *name_prefix;
static const char *name;
static const char *mtl_prefix;
static const char *texture_prefix;

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

static void realloc_or_exit(const char *error_message, void **block, size_t size)
{
  *block = realloc(*block, size);

  if (*block == NULL)
  {
    fprintf(stderr, "%s\n", error_message);
    exit(1);
  }
}

#define STATE_INITIAL 0
#define STATE_IGNORING_LINE 1

#define STATE_MTL_N 2
#define STATE_MTL_NE 3
#define STATE_MTL_NEW 4
#define STATE_MTL_NEWM 5
#define STATE_MTL_NEWMT 6
#define STATE_MTL_NEWMTL 7
#define STATE_MTL_NEWMTL_SPACE 8
#define STATE_MTL_NEWMTL_CHARACTERS 9
#define STATE_MTL_NEWMTL_AFTER 10
#define STATE_MTL_NS 11
#define STATE_MTL_NS_SPACE 12
#define STATE_MTL_K 13
#define STATE_MTL_KA 14
#define STATE_MTL_KA_SPACE 15
#define STATE_MTL_KS 16
#define STATE_MTL_KS_SPACE 17
#define STATE_MTL_KE 18
#define STATE_MTL_KE_SPACE 19
#define STATE_MTL_NI 20
#define STATE_MTL_NI_SPACE 21
#define STATE_MTL_D 22
#define STATE_MTL_D_SPACE 23
#define STATE_MTL_I 24
#define STATE_MTL_IL 25
#define STATE_MTL_ILL 26
#define STATE_MTL_ILLU 27
#define STATE_MTL_ILLUM 28
#define STATE_MTL_ILLUM_SPACE 29
#define STATE_MTL_M 30
#define STATE_MTL_MA 31
#define STATE_MTL_MAP 32
#define STATE_MTL_MAP_UNDERSCORE 33
#define STATE_MTL_MAP_UNDERSCORE_K 34
#define STATE_MTL_MAP_UNDERSCORE_KD 35
#define STATE_MTL_MAP_UNDERSCORE_KD_SPACE 36
#define STATE_MTL_MAP_UNDERSCORE_KD_CHARACTERS 37
#define STATE_MTL_MAP_UNDERSCORE_KD_AFTER 38

#define STATE_OBJ_M 2
#define STATE_OBJ_MT 3
#define STATE_OBJ_MTL 4
#define STATE_OBJ_MTLL 5
#define STATE_OBJ_MTLLI 6
#define STATE_OBJ_MTLLIB 7
#define STATE_OBJ_MTLLIB_SPACE 8
#define STATE_OBJ_MTLLIB_CHARACTERS 9
#define STATE_OBJ_MTLLIB_AFTER 10
#define STATE_OBJ_O 11
#define STATE_OBJ_O_SPACE 12
#define STATE_OBJ_O_CHARACTERS 13
#define STATE_OBJ_O_AFTER 14
#define STATE_OBJ_V 15
#define STATE_OBJ_V_SPACE 16
#define STATE_OBJ_V_X_DIGITS 17
#define STATE_OBJ_V_X_DECIMAL 18
#define STATE_OBJ_V_X_SPACE 19
#define STATE_OBJ_V_Y_DIGITS 20
#define STATE_OBJ_V_Y_DECIMAL 21
#define STATE_OBJ_V_Y_SPACE 22
#define STATE_OBJ_V_Z_DIGITS 23
#define STATE_OBJ_V_Z_DECIMAL 24
#define STATE_OBJ_V_Z_SPACE 25
#define STATE_OBJ_V_R_DIGITS 26
#define STATE_OBJ_V_R_DECIMAL 27
#define STATE_OBJ_V_R_SPACE 28
#define STATE_OBJ_V_G_DIGITS 29
#define STATE_OBJ_V_G_DECIMAL 30
#define STATE_OBJ_V_G_SPACE 31
#define STATE_OBJ_V_B_DIGITS 32
#define STATE_OBJ_V_B_DECIMAL 33
#define STATE_OBJ_V_B_SPACE 34
#define STATE_OBJ_V_O_DIGITS 35
#define STATE_OBJ_V_O_DECIMAL 36
#define STATE_OBJ_V_O_AFTER 37
#define STATE_OBJ_VT 38
#define STATE_OBJ_VT_SPACE 39
#define STATE_OBJ_VT_X_DIGITS 40
#define STATE_OBJ_VT_X_DECIMAL 41
#define STATE_OBJ_VT_X_SPACE 42
#define STATE_OBJ_VT_Y_DIGITS 43
#define STATE_OBJ_VT_Y_DECIMAL 44
#define STATE_OBJ_VT_Y_AFTER 45
#define STATE_OBJ_F 46
#define STATE_OBJ_F_SPACE 47
#define STATE_OBJ_F_V_MINUS 48
#define STATE_OBJ_F_V 49
#define STATE_OBJ_F_V_SEPARATOR 50
#define STATE_OBJ_F_VT 51
#define STATE_OBJ_F_VT_MINUS 52
#define STATE_OBJ_F_VT_SEPARATOR 53
#define STATE_OBJ_U 54
#define STATE_OBJ_US 55
#define STATE_OBJ_USE 56
#define STATE_OBJ_USEM 57
#define STATE_OBJ_USEMT 58
#define STATE_OBJ_USEMTL 59
#define STATE_OBJ_USEMTL_SPACE 60
#define STATE_OBJ_USEMTL_CHARACTERS 61
#define STATE_OBJ_USEMTL_AFTER 62

static bool object_is_navigation_mesh;
static char *object_name = NULL;

static float *vertices = NULL;
static int vertices_length = 0;

static float *texture_coordinates = NULL;
static int texture_coordinates_length = 0;

static int *face_vertex_indices = NULL;
static int *face_texture_coordinate_indices = NULL;
static int *face_material_indices = NULL;
static int face_indices_length = 0;

static int *face_lengths = NULL;
static int faces = 0;

#define MATERIAL_TYPE_OPAQUE 0
#define MATERIAL_TYPE_CUTOUT 1
#define MATERIAL_TYPE_ADDITIVE 2
#define MATERIAL_TYPE_BLENDED 3

static char **material_names = NULL;
static int *material_diffuse_texture_indices = NULL;
static int *material_types = NULL;
static int materials = 0;
static int material_index = -1;

static char **texture_names = NULL;
static char **texture_variable_names = NULL;
static bool *textures_imported = NULL;
static int textures = 0;

static int get_material_index_by_name(const char *name)
{
  for (int material_index = 0; material_index < materials; material_index++)
  {
    const char *compare = material_names[material_index];

    if (strlen(name) == strlen(compare) && !strcmp(name, compare))
    {
      return material_index;
    }
  }

  return -1;
}

static int get_texture_index_by_name(const char *name)
{
  for (int texture_index = 0; texture_index < textures; texture_index++)
  {
    const char *compare = texture_names[texture_index];

    if (strlen(name) == strlen(compare) && !strcmp(name, compare))
    {
      return texture_index;
    }
  }

  return -1;
}

static char *string = NULL;
static int string_length;

static void string_append(char character)
{
  if (string == NULL)
  {
    string = malloc_or_exit("Failed to allocate memory to start a new string.", sizeof(char) * 2);
    string[0] = character;
    string[1] = 0;
    string_length = 1;
  }
  else
  {
    realloc_or_exit("Failed to reallocate memory to extend a previously started string.", (void **)&string, sizeof(char) * (string_length + 2));
    string[string_length] = character;
    string_length++;
    string[string_length] = 0;
  }
}

static float get_and_reset_float()
{
  float output = atof(string);

  free(string);
  string = NULL;

  return output;
}

static int get_and_reset_integer()
{
  int output = atoi(string);

  free(string);
  string = NULL;

  return output;
}

static void write(const char *error_message, const char *format, ...)
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

static void write_float_array(
    const char *error_message,
    const float *floats,
    const int quantity,
    const char *name_format,
    ...)
{
  if (quantity)
  {
    write(error_message, "static const f32 ");

    va_list args;
    va_start(args, name_format);

    if (vfprintf(stdout, name_format, args) <= 0)
    {
      va_end(args);
      fprintf(stderr, "%s\n", error_message);
      exit(1);
    }
    else
    {
      va_end(args);
    }

    write(error_message, "[] = { ");

    for (int index = 0; index < quantity; index++)
    {
      write(error_message, index ? ", %ff" : "%ff", floats[index]);
    }

    write(error_message, " };\n");
  }
}

static void write_vector_array(
    const char *error_message,
    const float *vectors,
    const int quantity,
    const char *name_format,
    ...)
{
  if (quantity)
  {
    write(error_message, "static const vector ");

    va_list args;
    va_start(args, name_format);

    if (vfprintf(stdout, name_format, args) <= 0)
    {
      va_end(args);
      fprintf(stderr, "%s\n", error_message);
      exit(1);
    }
    else
    {
      va_end(args);
    }

    write(error_message, "[] = { ");

    for (int index = 0; index < quantity * 3; index += 3)
    {
      write(error_message, index ? ", {%ff, %ff, %ff}" : "{%ff, %ff, %ff}", vectors[index], vectors[index + 1], vectors[index + 2]);
    }

    write(error_message, " };\n");
  }
}

static void write_int_array(
    const char *error_message,
    const int *ints,
    const int quantity,
    const char *type,
    const char *name_format,
    ...)
{
  if (quantity)
  {
    write(error_message, "static const %s ", type);

    va_list args;
    va_start(args, name_format);

    if (vfprintf(stdout, name_format, args) <= 0)
    {
      va_end(args);
      fprintf(stderr, "%s\n", error_message);
      exit(1);
    }
    else
    {
      va_end(args);
    }

    write(error_message, "[] = { ");

    for (int index = 0; index < quantity; index++)
    {
      write(error_message, index ? ", %d" : "%d", ints[index]);
    }

    write(error_message, " };\n");
  }
}

static void write_texture_reference_array(
    const char *error_message,
    const int *material_indices,
    const int quantity,
    const char *name_format,
    ...)
{
  if (quantity)
  {
    for (int index = 0; index < quantity; index++)
    {
      const int texture_index = material_diffuse_texture_indices[material_indices[index]];

      if (!textures_imported[texture_index])
      {
        write(error_message, "const texture * %s();\n", texture_variable_names[material_diffuse_texture_indices[material_indices[index]]]);

        textures_imported[texture_index] = true;
      }
    }

    write(error_message, "static texture_factory * const ");

    va_list args;
    va_start(args, name_format);

    if (vfprintf(stdout, name_format, args) <= 0)
    {
      va_end(args);
      fprintf(stderr, "%s\n", error_message);
      exit(1);
    }
    else
    {
      va_end(args);
    }

    write(error_message, "[] = { ");

    for (int index = 0; index < quantity; index++)
    {
      write(error_message, index ? ", %s" : "%s", texture_variable_names[material_diffuse_texture_indices[material_indices[index]]]);
    }

    write(error_message, " };\n");
  }
}

static void cross_product(
    const float a[3],
    const float b[3],
    float result[3])
{
  const float x = a[1] * b[2] - b[1] * a[2];
  const float y = a[2] * b[0] - b[2] * a[0];
  const float z = a[0] * b[1] - b[0] * a[1];
  result[0] = x;
  result[1] = y;
  result[2] = z;
}

static void normalize(float vector[3])
{
  const float magnitude = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
  // TODO: handle zero
  vector[0] /= magnitude;
  vector[1] /= magnitude;
  vector[2] /= magnitude;
}

static void normal_between(
    const float from[3],
    const float to[3],
    float result[3])
{
  const float delta_x = to[0] - from[0];
  const float delta_y = to[1] - from[1];
  const float delta_z = to[2] - from[2];
  const float magnitude = sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);

  // TODO: handle zero

  result[0] = delta_x / magnitude;
  result[1] = delta_y / magnitude;
  result[2] = delta_z / magnitude;
}

static bool equal(
    const float from[3],
    const float to[3])
{
  return from[0] == to[0] && from[1] == to[1] && from[2] == to[2];
}

static void obj_type(
    int *target_vertices,
    float **locations,
    int passes,
    const int *const material_indices,
    int **triangles,
    const int total_triangles,
    int **indices,
    float **rows,
    float **columns,
    float **opacities,
    float **reds,
    float **greens,
    float **blues)
{
  if (total_triangles)
  {
    *triangles = malloc_or_exit("Failed to allocate memory for triangle counts.", sizeof(int) * passes);
    *indices = malloc_or_exit("Failed to allocate memory for indices.", sizeof(int) * total_triangles * 3);
    *rows = malloc_or_exit("Failed to allocate memory for texture coordinate rows.", sizeof(float) * total_triangles * 3);
    *columns = malloc_or_exit("Failed to allocate memory for texture coordinate columns.", sizeof(float) * total_triangles * 3);

    if (opacities != NULL)
    {
      *opacities = malloc_or_exit("Failed to allocate memory for opacities.", sizeof(float) * total_triangles * 3);
    }

    *reds = malloc_or_exit("Failed to allocate memory for reds.", sizeof(float) * total_triangles * 3);
    *greens = malloc_or_exit("Failed to allocate memory for greens.", sizeof(float) * total_triangles * 3);
    *blues = malloc_or_exit("Failed to allocate memory for blues.", sizeof(float) * total_triangles * 3);

    int write_index = 0;

    for (int pass = 0; pass < passes; pass++)
    {
      const int pass_material = material_indices[pass];

      (*triangles)[pass] = 0;

      int read_index = 0;

      for (int face = 0; face < faces; face++)
      {
        const int face_material = face_material_indices[face];

        const int length = face_lengths[face];

        if (face_material == pass_material)
        {
          int first_index = 0, previous_index = 0;
          float first_row = 0.0f, first_column = 0.0f, first_opacity = 0.0f, first_red = 0.0f, first_green = 0.0f, first_blue = 0.0f;
          float previous_row = 0.0f, previous_column = 0.0f, previous_opacity = 0.0f, previous_red = 0.0f, previous_green = 0.0f, previous_blue = 0.0f;

          for (int vertex = 0; vertex < length; vertex++)
          {
            int vertex_index = face_vertex_indices[read_index];

            const float *vertex_data = &vertices[vertex_index * 7];
            int next_index;
            const int next_texture_coordinate_index = face_texture_coordinate_indices[read_index];
            const float next_row = 1 - texture_coordinates[next_texture_coordinate_index * 2 + 1];
            const float next_column = texture_coordinates[next_texture_coordinate_index * 2];
            const float next_opacity = vertex_data[6];
            const float next_red = vertex_data[3];
            const float next_green = vertex_data[4];
            const float next_blue = vertex_data[5];

            read_index++;

            if ((*locations) == NULL)
            {
              *locations = malloc_or_exit("Failed to allocate memory for vertex locations.", sizeof(float) * ((*target_vertices) + 1) * 3);

              (*locations)[0] = vertex_data[0];
              (*locations)[1] = vertex_data[1];
              (*locations)[2] = vertex_data[2];

              (*target_vertices)++;

              next_index = 0;
            }
            else
            {
              for (next_index = 0; next_index < (*target_vertices); next_index++)
              {
                if ((*locations)[next_index * 3] == vertex_data[0] && (*locations)[next_index * 3 + 1] == vertex_data[1] && (*locations)[next_index * 3 + 2] == vertex_data[2])
                {
                  break;
                }
              }

              if (next_index == (*target_vertices))
              {
                realloc_or_exit("Failed to reallocate memory for vertex locations.", (void **)locations, sizeof(float) * ((*target_vertices) + 1) * 3);

                (*locations)[(*target_vertices) * 3] = vertex_data[0];
                (*locations)[(*target_vertices) * 3 + 1] = vertex_data[1];
                (*locations)[(*target_vertices) * 3 + 2] = vertex_data[2];

                (*target_vertices)++;
              }
            }

            switch (vertex)
            {
            case 0:
              first_index = next_index;
              first_row = next_row;
              first_column = next_column;
              first_opacity = next_opacity;
              first_red = next_red;
              first_green = next_green;
              first_blue = next_blue;
              break;

            case 1:
              break;

            default:
              (*indices)[write_index] = first_index;
              (*rows)[write_index] = first_row;
              (*columns)[write_index] = first_column;

              if (opacities != NULL)
              {
                (*opacities)[write_index] = first_opacity;
              }

              (*reds)[write_index] = first_red;
              (*greens)[write_index] = first_green;
              (*blues)[write_index] = first_blue;

              write_index++;

              (*indices)[write_index] = previous_index;
              (*rows)[write_index] = previous_row;
              (*columns)[write_index] = previous_column;

              if (opacities != NULL)
              {
                (*opacities)[write_index] = previous_opacity;
              }

              (*reds)[write_index] = previous_red;
              (*greens)[write_index] = previous_green;
              (*blues)[write_index] = previous_blue;

              write_index++;

              (*indices)[write_index] = next_index;
              (*rows)[write_index] = next_row;
              (*columns)[write_index] = next_column;

              if (opacities != NULL)
              {
                (*opacities)[write_index] = next_opacity;
              }

              (*reds)[write_index] = next_red;
              (*greens)[write_index] = next_green;
              (*blues)[write_index] = next_blue;

              write_index++;
              break;
            }

            previous_index = next_index;
            previous_row = next_row;
            previous_column = next_column;
            previous_opacity = next_opacity;
            previous_red = next_red;
            previous_green = next_green;
            previous_blue = next_blue;
          }

          (*triangles)[pass] += length - 2;
        }
        else
        {
          read_index += length;
        }
      }
    }
  }
}

static void obj_end_object()
{
  if (object_name != NULL)
  {
    if (face_lengths == NULL)
    {
      fprintf(stderr, "Object \"%s\" is empty.\n", object_name);
      exit(1);
    }
    else if (object_is_navigation_mesh)
    {
      write("Failed to write the header of a navigation mesh's vertex counts.", "\nstatic const quantity %s_%s_%s_vertex_counts[] = {", name_prefix, name, object_name);

      for (int face_index = 0; face_index < faces; face_index++)
      {
        write("Failed to write a navigation mesh's vertex counts.", "%s%d", face_index ? ", " : "", face_lengths[face_index]);
      }

      write("Failed to write the footer of a navigation mesh's vertex counts and the header of its neighbor faces.", "};\n\nstatic const index %s_%s_%d_neighbor_faces[] = {", name_prefix, name, object_name);

      float *face_surface_normals = malloc_or_exit("Failed to allocate a list of face surface normals.", sizeof(float) * faces * 3);
      int *face_neighbor_faces = malloc_or_exit("Failed to allocate a list of face neighbor faces.", sizeof(int) * face_indices_length);
      int *face_neighbor_edges = malloc_or_exit("Failed to allocate a list of face neighbor edges.", sizeof(int) * face_indices_length);

      int first_face_index_offset = 0;

      for (int first_face_index = 0; first_face_index < faces; first_face_index++)
      {
        // TODO: double check that faces are planar
        // TODO: double check that faces are convex

        const int first_face_length = face_lengths[first_face_index];

        for (int first_vertex_index = 0; first_vertex_index < first_face_length; first_vertex_index++)
        {
          const int first_index = face_vertex_indices[first_face_index_offset + first_vertex_index];
          const int next_first_index = face_vertex_indices[first_face_index_offset + ((first_vertex_index + 1) % first_face_length)];
          const float *const first_vertex_location = &vertices[first_index];

          for (int second_vertex_index = first_vertex_index + 1; second_vertex_index < first_face_length; second_vertex_index++)
          {
            const float *const second_vertex_location = &vertices[face_vertex_indices[first_face_index_offset + second_vertex_index]];

            if (equal(first_vertex_location, second_vertex_location))
            {
              fprintf(stderr, "A navigation mesh face is degenerate.\n");
              exit(1);
            }
          }

          int neighbor_face = -1;
          int neighbor_edge = -1;
          int second_face_index_offset = 0;

          for (int second_face_index = 0; second_face_index < faces; second_face_index++)
          {
            const int second_face_length = face_lengths[second_face_index];

            for (int second_vertex_index = 0; second_vertex_index < second_face_length; second_vertex_index++)
            {
              const int second_index = face_vertex_indices[second_face_index_offset + second_vertex_index];
              const int next_second_index = face_vertex_indices[second_face_index_offset + ((second_vertex_index + 1) % second_face_length)];

              if (first_index == next_second_index && second_index == next_first_index)
              {
                if (neighbor_face == -1)
                {
                  neighbor_face = second_face_index;
                  neighbor_edge = second_vertex_index;
                }
                else
                {
                  fprintf(stderr, "A navigation mesh face shares an edge with multiple other faces.\n");
                  exit(1);
                }
              }
            }

            second_face_index_offset += second_face_length;
          }

          face_neighbor_faces[first_face_index_offset + first_vertex_index] = neighbor_face;
          face_neighbor_edges[first_face_index_offset + first_vertex_index] = neighbor_edge;

          if (neighbor_face == -1)
          {
            write("Failed to write a navigation mesh's neighbor faces.", first_face_index || first_vertex_index ? ", INDEX_NONE" : "INDEX_NONE");
          }
          else
          {
            write("Failed to write a navigation mesh's neighbor faces.", "%s%d", first_face_index || first_vertex_index ? ", " : "", neighbor_face);
          }
        }

        const float *const first_vertex_location = &vertices[face_vertex_indices[first_face_index_offset] * 7];
        const float *const second_vertex_location = &vertices[face_vertex_indices[first_face_index_offset + 1] * 7];
        const float *const third_vertex_location = &vertices[face_vertex_indices[first_face_index_offset + 2] * 7];

        float normal_first_second[3];
        normal_between(first_vertex_location, second_vertex_location, normal_first_second);

        float normal_first_third[3];
        normal_between(first_vertex_location, third_vertex_location, normal_first_third);

        float *surface_normal = &face_surface_normals[first_face_index * 3];
        cross_product(normal_first_second, normal_first_third, surface_normal);
        normalize(surface_normal);

        first_face_index_offset += first_face_length;
      }

      write("Failed to write the footer of a navigation mesh's neighbor faces and the header of its neighbor face re-mappings.", "};\n\nstatic const index * const %s_%s_%s_neighbor_face_re_mappings[] = {", name_prefix, name, object_name);

      int face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        write("Failed to write a navigation mesh's neighbor face re-mappings.", "%s&%s_%s_%s_neighbor_faces[%d]", face_index ? ", " : "", name_prefix, name, object_name, face_index_offset);

        face_index_offset += face_lengths[face_index];
      }

      write("Failed to write the footer of a navigation mesh's neighbor face re-mappings and the header of its neighbor edges.", "};\n\nstatic const index %s_%s_%s_neighbor_edges[] = {", name_prefix, name, object_name);

      for (int face_edge_index = 0; face_edge_index < face_indices_length; face_edge_index++)
      {
        const int face_neighbor_edge = face_neighbor_edges[face_edge_index];

        if (face_neighbor_edge == -1)
        {
          write("Failed to write a navigation mesh's neighbor edges.", "%sINDEX_NONE", face_edge_index ? ", " : "");
        }
        else
        {
          write("Failed to write a navigation mesh's neighbor edges.", "%s%d", face_edge_index ? ", " : "", face_neighbor_edge);
        }
      }

      write("Failed to write the footer of a navigation mesh's neighbor edges and the header of its neighbor edges re-mappings.", "};\n\nstatic const index * const %s_%s_%s_neighbor_edge_re_mappings[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        write("Failed to write a navigation mesh's neighbor edge re-mappings.", "%s&%s_%s_%s_neighbor_edges[%d]", face_index ? ", " : "", name_prefix, name, object_name, face_index_offset);

        face_index_offset += face_lengths[face_index];
      }

      write("Failed to write the footer of a navigation mesh's neighbor edge re-mappings and the header of its surface normals.", "};\n\nstatic const f32 %s_%s_%s_surface_normals[] = {", name_prefix, name, object_name);

      for (int face_axis_index = 0; face_axis_index < faces * 3; face_axis_index++)
      {
        write("Failed to write a navigation mesh's surface normals.", "%s%f", face_axis_index ? ", " : "", face_surface_normals[face_axis_index]);
      }

      write("Failed to write the footer of a navigation mesh's surface normals and the header of its vertex locations.", "};\n\nstatic const f32 %s_%s_%s_vertex_locations[] = {", name_prefix, name, object_name);

      for (int face_vertex_index = 0; face_vertex_index < face_indices_length; face_vertex_index++)
      {
        const float *const vertex = &vertices[face_vertex_indices[face_vertex_index] * 7];

        for (int axis = 0; axis < 3; axis++)
        {
          write("Failed to write a navigation mesh's vertex locations.", "%s%f", face_vertex_index || axis ? ", " : "", vertex[axis]);
        }
      }

      write("Failed to write the footer of a navigation mesh's vertex locations and the header of its vertex location re-mappings.", "};\n\nstatic const f32 * const %s_%s_%s_vertex_location_re_mappings[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        write("Failed to write a navigation mesh's vertex location re-mappings.", "%s&%s_%s_%s_vertex_locations[%d]", face_index ? ", " : "", name_prefix, name, object_name, face_index_offset);

        face_index_offset += face_lengths[face_index] * 3;
      }

      write("Failed to write the footer of a navigation mesh's vertex location re-mappings and the header of its edge coefficients.", "};\n\nstatic const f32 %s_%s_%s_edge_coefficients[] = {", name_prefix, name, object_name);

      float *face_edge_exit_normals = malloc_or_exit("Failed to allocate a list of face edge exit normals.", sizeof(float) * face_indices_length * 3);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        const float *const face_surface_normal = &face_surface_normals[face_index * 3];

        const int face_length = face_lengths[face_index];

        for (int edge_index = 0; edge_index < face_length; edge_index++)
        {
          const float *const vertex = &vertices[face_vertex_indices[face_index_offset + edge_index] * 7];
          const float *next_vertex = &vertices[face_vertex_indices[face_index_offset + ((edge_index + 1) % face_length)] * 7];

          const float delta_x = next_vertex[0] - vertex[0];
          const float delta_y = next_vertex[1] - vertex[1];
          const float delta_z = next_vertex[2] - vertex[2];
          const float magnitude_squared = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;

          write("Failed to write a navigation mesh's edge coefficients.", "%s%f, %f, %f", face_index || edge_index ? ", " : "", delta_x / magnitude_squared, delta_y / magnitude_squared, delta_z / magnitude_squared);

          const float magnitude = sqrt(magnitude_squared);
          const float normal[] = {delta_x / magnitude, delta_y / magnitude, delta_z / magnitude};

          const int neighbor_face = face_neighbor_faces[face_index_offset + edge_index];

          float *face_edge_exit_normal = &face_edge_exit_normals[(face_index_offset + edge_index) * 3];

          if (neighbor_face == -1)
          {
            cross_product(normal, face_surface_normal, face_edge_exit_normal);
          }
          else
          {
            const float *const neighbor_surface_normal = &face_surface_normals[neighbor_face * 3];

            float attempted_averaged_surface_normal[] = {
                face_surface_normal[0] + neighbor_surface_normal[0],
                face_surface_normal[1] + neighbor_surface_normal[1],
                face_surface_normal[2] + neighbor_surface_normal[2]};

            const float averaged_magnitude = sqrt(attempted_averaged_surface_normal[0] * attempted_averaged_surface_normal[0] + attempted_averaged_surface_normal[1] * attempted_averaged_surface_normal[1] + attempted_averaged_surface_normal[2] * attempted_averaged_surface_normal[2]);

            const float *averaged_surface_normal;

            if (averaged_magnitude == 0)
            {
              averaged_surface_normal = neighbor_surface_normal;
            }
            else
            {
              attempted_averaged_surface_normal[0] /= averaged_magnitude;
              attempted_averaged_surface_normal[1] /= averaged_magnitude;
              attempted_averaged_surface_normal[2] /= averaged_magnitude;
              averaged_surface_normal = attempted_averaged_surface_normal;
            }

            cross_product(normal, averaged_surface_normal, face_edge_exit_normal);
          }

          normalize(face_edge_exit_normal);
        }

        face_index_offset += face_length;
      }

      write("Failed to write the footer of a navigation mesh's edge coefficients and the header of its edge coefficient re-mappings.", "};\n\nstatic const f32 * const %s_%s_%s_edge_coefficient_re_mappings[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        write("Failed to write a navigation mesh's edge coefficient re-mappings.", "%s&%s_%s_%s_edge_coefficients[%d]", face_index ? ", " : "", name_prefix, name, object_name, face_index_offset * 3);

        face_index_offset += face_lengths[face_index];
      }

      write("Failed to write the footer of a navigation mesh's edge coefficient re-mappings and the header of its edge exit normals.", "};\n\nstatic const f32 %s_%s_%s_edge_exit_normals[] = {", name_prefix, name, object_name);

      for (int face_vertex_axis_index = 0; face_vertex_axis_index < face_indices_length * 3; face_vertex_axis_index++)
      {
        write("Failed to write a navigation mesh's edge exit normals.", "%s%f", face_vertex_axis_index ? ", " : "", face_edge_exit_normals[face_vertex_axis_index]);
      }

      write("Failed to write the footer of a navigation mesh's edge exit normals and the header of its edge exit normal re-mappings.", "};\n\nstatic const f32 * const %s_%s_%s_edge_exit_normal_re_mappings[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        write("Failed to write a navigation mesh's edge exit normal re-mappings.", "%s&%s_%s_%s_edge_exit_normals[%d]", face_index ? ", " : "", name_prefix, name, object_name, face_index_offset * 3);

        face_index_offset += face_lengths[face_index];
      }

      write("Failed to write the footer of a navigation mesh's edge exit normal re-mappings and the header of its edge exit intersection normals.", "};\n\nstatic const f32 %s_%s_%s_edge_exit_intersection_normals[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        const int face_length = face_lengths[face_index];

        int previous_edge_index = face_length - 1;

        for (int edge_index = 0; edge_index < face_length; edge_index++)
        {
          const float *const previous_face_edge_exit_normal = &face_edge_exit_normals[(face_index_offset + previous_edge_index) * 3];
          const float *const face_edge_exit_normal = &face_edge_exit_normals[(face_index_offset + edge_index) * 3];

          float result[3];

          cross_product(previous_face_edge_exit_normal, face_edge_exit_normal, result);
          normalize(result);

          write("Failed to write a navigation mesh's edge exit intersection normal.", "%s%f, %f, %f", face_index || edge_index ? ", " : "", result[0], result[1], result[2]);

          previous_edge_index = edge_index;
        }

        face_index_offset += face_length;
      }

      write("Failed to write the footer of a navigation mesh's edge exit intersection normals and the header of its edge exit intersection normal re-mappings.", "};\n\nstatic const f32 * const %s_%s_%s_edge_exit_intersection_normal_re_mappings[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        write("Failed to write a navigation mesh's edge exit intersection normal re-mappings.", "%s&%s_%s_%s_edge_exit_intersection_normals[%d]", face_index ? ", " : "", name_prefix, name, object_name, face_index_offset * 3);

        face_index_offset += face_lengths[face_index];
      }

      write("Failed to write the footer of a navigation mesh's edge exit intersection normal re-mappings and the header of its edge midpoint distances.", "};\n\nstatic const f32 %s_%s_%s_edge_midpoint_distances[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        const int face_length = face_lengths[face_index];

        for (int first_edge_index = 0; first_edge_index < face_length; first_edge_index++)
        {
          const float *const first_edge_first_vertex_location = &vertices[face_vertex_indices[face_index_offset + first_edge_index] * 7];
          const float *const first_edge_second_vertex_location = &vertices[face_vertex_indices[face_index_offset + ((first_edge_index + 1) % face_length)] * 7];

          const float first_edge_midpoint[3] = {
              (first_edge_first_vertex_location[0] + first_edge_second_vertex_location[0]) / 2,
              (first_edge_first_vertex_location[1] + first_edge_second_vertex_location[1]) / 2,
              (first_edge_first_vertex_location[2] + first_edge_second_vertex_location[2]) / 2,
          };

          for (int second_edge_index = 0; second_edge_index < face_length; second_edge_index++)
          {
            const float *const second_edge_first_vertex_location = &vertices[face_vertex_indices[face_index_offset + second_edge_index] * 7];
            const float *const second_edge_second_vertex_location = &vertices[face_vertex_indices[face_index_offset + ((second_edge_index + 1) % face_length)] * 7];

            const float second_edge_midpoint[3] = {
                (second_edge_first_vertex_location[0] + second_edge_second_vertex_location[0]) / 2,
                (second_edge_first_vertex_location[1] + second_edge_second_vertex_location[1]) / 2,
                (second_edge_first_vertex_location[2] + second_edge_second_vertex_location[2]) / 2,
            };

            const float delta[3] = {
                second_edge_midpoint[0] - first_edge_midpoint[0],
                second_edge_midpoint[1] - first_edge_midpoint[1],
                second_edge_midpoint[2] - first_edge_midpoint[2],
            };

            const float distance = sqrt(delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]);

            write("Failed to write a navigation mesh's edge midpoint distances.", "%s%f", face_index || first_edge_index || second_edge_index ? ", " : "", distance);
          }
        }

        face_index_offset += face_length;
      }

      write("Failed to write the footer of a navigation mesh's edge midpoint distances and the header of its edge midpoint distance re-mappings.", "};\n\nstatic const f32 * const %s_%s_%s_edge_midpoint_distance_re_mappings[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        write("Failed to write a navigation mesh's edge midpoint distance re-mappings.", "%s&%s_%s_%s_edge_midpoint_distances[%d]", face_index ? ", " : "", name_prefix, name, object_name, face_index_offset);

        face_index_offset += face_lengths[face_index] * face_lengths[face_index];
      }

      write("Failed to write the footer of a navigation mesh's edge exit intersection normal re-mappings and the header of its edge midpoint distances.", "};\n\nstatic const f32 %s_%s_%s_edge_midpoint_center_distances[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        const int face_length = face_lengths[face_index];

        float face_midpoint[3] = {0, 0, 0};

        for (int vertex_index = 0; vertex_index < face_length; vertex_index++)
        {
          const float *const vertex_location = &vertices[face_vertex_indices[face_index_offset + vertex_index] * 7];

          face_midpoint[0] += vertex_location[0];
          face_midpoint[1] += vertex_location[1];
          face_midpoint[2] += vertex_location[2];
        }

        face_midpoint[0] /= face_length;
        face_midpoint[1] /= face_length;
        face_midpoint[2] /= face_length;

        for (int edge_index = 0; edge_index < face_length; edge_index++)
        {
          const float *const edge_first_vertex_location = &vertices[face_vertex_indices[face_index_offset + edge_index] * 7];
          const float *const edge_second_vertex_location = &vertices[face_vertex_indices[face_index_offset + ((edge_index + 1) % face_length)] * 7];

          const float edge_midpoint[3] = {
              (edge_first_vertex_location[0] + edge_second_vertex_location[0]) / 2,
              (edge_first_vertex_location[1] + edge_second_vertex_location[1]) / 2,
              (edge_first_vertex_location[2] + edge_second_vertex_location[2]) / 2,
          };
          const float delta[3] = {
              face_midpoint[0] - edge_midpoint[0],
              face_midpoint[1] - edge_midpoint[1],
              face_midpoint[2] - edge_midpoint[2],
          };

          const float distance = sqrt(delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]);

          write("Failed to write a navigation mesh's edge midpoint center distances.", "%s%f", face_index || edge_index ? ", " : "", distance);
        }

        face_index_offset += face_length;
      }

      write("Failed to write the footer of a navigation mesh's edge midpoint center distances and the header of its edge midpoint distance re-mappings.", "};\n\nstatic const f32 * const %s_%s_%s_edge_midpoint_center_distance_re_mappings[] = {", name_prefix, name, object_name);

      face_index_offset = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        write("Failed to write a navigation mesh's edge midpoint center distance re-mappings.", "%s&%s_%s_%s_edge_midpoint_center_distances[%d]", face_index ? ", " : "", name_prefix, name, object_name, face_index_offset);

        face_index_offset += face_lengths[face_index];
      }

      write("Failed to write the footer of a navigation mesh.", "};\n\nconst navigation_mesh %s_%s_%s = {%d, %s_%s_%s_vertex_counts, %s_%s_%s_neighbor_face_re_mappings, %s_%s_%s_neighbor_edge_re_mappings, %s_%s_%s_vertex_location_re_mappings, %s_%s_%s_surface_normals, %s_%s_%s_edge_exit_normal_re_mappings, %s_%s_%s_edge_exit_intersection_normal_re_mappings, %s_%s_%s_edge_coefficient_re_mappings, %s_%s_%s_edge_midpoint_distance_re_mappings, %s_%s_%s_edge_midpoint_center_distance_re_mappings};\n", name_prefix, name, object_name, faces, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name);

      free(face_edge_exit_normals);
      free(face_surface_normals);
      free(face_neighbor_edges);
      free(face_neighbor_faces);
    }
    else
    {
      int opaque_cutout_vertices = 0;
      float *opaque_cutout_locations = NULL;
      int opaque_passes = 0;
      int *opaque_materials = NULL;
      int *opaque_triangles = NULL;
      int total_opaque_triangles = 0;
      int *opaque_indices = NULL;
      float *opaque_rows = NULL;
      float *opaque_columns = NULL;
      float *opaque_reds = NULL;
      float *opaque_greens = NULL;
      float *opaque_blues = NULL;
      int cutout_passes = 0;
      int *cutout_materials = NULL;
      int *cutout_triangles = NULL;
      int total_cutout_triangles = 0;
      int *cutout_indices = NULL;
      float *cutout_rows = NULL;
      float *cutout_columns = NULL;
      float *cutout_opacities = NULL;
      float *cutout_reds = NULL;
      float *cutout_greens = NULL;
      float *cutout_blues = NULL;
      int additive_blended_vertices = 0;
      float *additive_blended_locations = NULL;
      int additive_passes = 0;
      int *additive_materials = NULL;
      int *additive_triangles = NULL;
      int total_additive_triangles = 0;
      int *additive_indices = NULL;
      float *additive_rows = NULL;
      float *additive_columns = NULL;
      float *additive_reds = NULL;
      float *additive_greens = NULL;
      float *additive_blues = NULL;
      int blended_passes = 0;
      int *blended_materials = NULL;
      int *blended_triangles = NULL;
      int total_blended_triangles = 0;
      int *blended_indices = NULL;
      float *blended_rows = NULL;
      float *blended_columns = NULL;
      float *blended_opacities = NULL;
      float *blended_reds = NULL;
      float *blended_greens = NULL;
      float *blended_blues = NULL;

      for (int face = 0; face < faces; face++)
      {
        const int material = face_material_indices[face];

        int *target_passes;
        int **target_materials;
        int *total_target_triangles;

        switch (material_types[material])
        {
        case MATERIAL_TYPE_OPAQUE:
          target_passes = &opaque_passes;
          target_materials = &opaque_materials;
          total_target_triangles = &total_opaque_triangles;
          break;

        case MATERIAL_TYPE_CUTOUT:
          target_passes = &cutout_passes;
          target_materials = &cutout_materials;
          total_target_triangles = &total_cutout_triangles;
          break;

        case MATERIAL_TYPE_ADDITIVE:
          target_passes = &additive_passes;
          target_materials = &additive_materials;
          total_target_triangles = &total_additive_triangles;
          break;

        case MATERIAL_TYPE_BLENDED:
          target_passes = &blended_passes;
          target_materials = &blended_materials;
          total_target_triangles = &total_blended_triangles;
          break;

        default:
          fprintf(stderr, "Unimplemented material type %d\n.", material_types[material]);
          exit(1);
        }

        if ((*target_materials) == NULL)
        {
          *target_materials = malloc_or_exit("Failed to allocate memory for a list of materials.", sizeof(int));
          *target_passes = 1;

          (*target_materials)[0] = material;
        }
        else
        {

          int pass;

          for (pass = 0; pass < (*target_passes); pass++)
          {
            if ((*target_materials)[pass] == material)
            {
              break;
            }
          }

          if (pass == (*target_passes))
          {
            realloc_or_exit("Failed to reallocate memory for a list of materials.", (void **)target_materials, ((*target_passes) + 1) * sizeof(int));
            (*target_materials)[pass] = material;
            (*target_passes)++;
          }
        }

        // TODO: drop degenerate triangles
        // TODO: throw if non-convex
        // TODO: throw if non-planar

        (*total_target_triangles) += face_lengths[face] - 2;
      }

      obj_type(
          &opaque_cutout_vertices,
          &opaque_cutout_locations,
          opaque_passes,
          opaque_materials,
          &opaque_triangles,
          total_opaque_triangles,
          &opaque_indices,
          &opaque_rows,
          &opaque_columns,
          NULL,
          &opaque_reds,
          &opaque_greens,
          &opaque_blues);

      obj_type(
          &opaque_cutout_vertices,
          &opaque_cutout_locations,
          cutout_passes,
          cutout_materials,
          &cutout_triangles,
          total_cutout_triangles,
          &cutout_indices,
          &cutout_rows,
          &cutout_columns,
          &cutout_opacities,
          &cutout_reds,
          &cutout_greens,
          &cutout_blues);

      obj_type(
          &additive_blended_vertices,
          &additive_blended_locations,
          additive_passes,
          additive_materials,
          &additive_triangles,
          total_additive_triangles,
          &additive_indices,
          &additive_rows,
          &additive_columns,
          NULL,
          &additive_reds,
          &additive_greens,
          &additive_blues);

      obj_type(
          &additive_blended_vertices,
          &additive_blended_locations,
          blended_passes,
          blended_materials,
          &blended_triangles,
          total_blended_triangles,
          &blended_indices,
          &blended_rows,
          &blended_columns,
          &blended_opacities,
          &blended_reds,
          &blended_greens,
          &blended_blues);

      write_vector_array("Failed to write opaque/cutout locations.", opaque_cutout_locations, opaque_cutout_vertices, "%s_%s_%s_opaque_cutout_locations", name_prefix, name, object_name);
      write_texture_reference_array("Failed to write opaque textures.", opaque_materials, opaque_passes, "%s_%s_%s_opaque_textures", name_prefix, name, object_name);
      write_int_array("Failed to write opaque triangles.", opaque_triangles, opaque_passes, "quantity", "%s_%s_%s_opaque_triangles", name_prefix, name, object_name);
      write_int_array("Failed to write opaque indices.", opaque_indices, total_opaque_triangles * 3, "index", "%s_%s_%s_opaque_indices", name_prefix, name, object_name);
      write_float_array("Failed to write opaque rows.", opaque_rows, total_opaque_triangles * 3, "%s_%s_%s_opaque_rows", name_prefix, name, object_name);
      write_float_array("Failed to write opaque columns.", opaque_columns, total_opaque_triangles * 3, "%s_%s_%s_opaque_columns", name_prefix, name, object_name);
      write_float_array("Failed to write opaque reds.", opaque_reds, total_opaque_triangles * 3, "%s_%s_%s_opaque_reds", name_prefix, name, object_name);
      write_float_array("Failed to write opaque greens.", opaque_greens, total_opaque_triangles * 3, "%s_%s_%s_opaque_greens", name_prefix, name, object_name);
      write_float_array("Failed to write opaque blues.", opaque_blues, total_opaque_triangles * 3, "%s_%s_%s_opaque_blues", name_prefix, name, object_name);
      write_texture_reference_array("Failed to write cutout textures.", cutout_materials, cutout_passes, "%s_%s_%s_cutout_textures", name_prefix, name, object_name);
      write_int_array("Failed to write cutout triangles.", cutout_triangles, cutout_passes, "quantity", "%s_%s_%s_cutout_triangles", name_prefix, name, object_name);
      write_int_array("Failed to write cutout indices.", cutout_indices, total_cutout_triangles * 3, "index", "%s_%s_%s_cutout_indices", name_prefix, name, object_name);
      write_float_array("Failed to write cutout rows.", cutout_rows, total_cutout_triangles * 3, "%s_%s_%s_cutout_rows", name_prefix, name, object_name);
      write_float_array("Failed to write cutout columns.", cutout_columns, total_cutout_triangles * 3, "%s_%s_%s_cutout_columns", name_prefix, name, object_name);
      write_float_array("Failed to write cutout opacities.", cutout_opacities, total_cutout_triangles * 3, "%s_%s_%s_cutout_opacities", name_prefix, name, object_name);
      write_float_array("Failed to write cutout reds.", cutout_reds, total_cutout_triangles * 3, "%s_%s_%s_cutout_reds", name_prefix, name, object_name);
      write_float_array("Failed to write cutout greens.", cutout_greens, total_cutout_triangles * 3, "%s_%s_%s_cutout_greens", name_prefix, name, object_name);
      write_float_array("Failed to write cutout blues.", cutout_blues, total_cutout_triangles * 3, "%s_%s_%s_cutout_blues", name_prefix, name, object_name);
      write_vector_array("Failed to write additive/blended locations.", additive_blended_locations, additive_blended_vertices, "%s_%s_%s_additive_blended_locations", name_prefix, name, object_name);
      write_texture_reference_array("Failed to write additive textures.", additive_materials, additive_passes, "%s_%s_%s_additive_textures", name_prefix, name, object_name);
      write_int_array("Failed to write additive triangles.", additive_triangles, additive_passes, "quantity", "%s_%s_%s_additive_triangles", name_prefix, name, object_name);
      write_int_array("Failed to write additive indices.", additive_indices, total_additive_triangles * 3, "index", "%s_%s_%s_additive_indices", name_prefix, name, object_name);
      write_float_array("Failed to write additive rows.", additive_rows, total_additive_triangles * 3, "%s_%s_%s_additive_rows", name_prefix, name, object_name);
      write_float_array("Failed to write additive columns.", additive_columns, total_additive_triangles * 3, "%s_%s_%s_additive_columns", name_prefix, name, object_name);
      write_float_array("Failed to write additive reds.", additive_reds, total_additive_triangles * 3, "%s_%s_%s_additive_reds", name_prefix, name, object_name);
      write_float_array("Failed to write additive greens.", additive_greens, total_additive_triangles * 3, "%s_%s_%s_additive_greens", name_prefix, name, object_name);
      write_float_array("Failed to write additive blues.", additive_blues, total_additive_triangles * 3, "%s_%s_%s_additive_blues", name_prefix, name, object_name);
      write_texture_reference_array("Failed to write blended textures.", blended_materials, blended_passes, "%s_%s_%s_blended_textures", name_prefix, name, object_name);
      write_int_array("Failed to write blended triangles.", blended_triangles, blended_passes, "quantity", "%s_%s_%s_blended_triangles", name_prefix, name, object_name);
      write_int_array("Failed to write blended indices.", blended_indices, total_blended_triangles * 3, "index", "%s_%s_%s_blended_indices", name_prefix, name, object_name);
      write_float_array("Failed to write blended rows.", blended_rows, total_blended_triangles * 3, "%s_%s_%s_blended_rows", name_prefix, name, object_name);
      write_float_array("Failed to write blended columns.", blended_columns, total_blended_triangles * 3, "%s_%s_%s_blended_columns", name_prefix, name, object_name);
      write_float_array("Failed to write blended opacities.", blended_opacities, total_blended_triangles * 3, "%s_%s_%s_blended_opacities", name_prefix, name, object_name);
      write_float_array("Failed to write blended reds.", blended_reds, total_blended_triangles * 3, "%s_%s_%s_blended_reds", name_prefix, name, object_name);
      write_float_array("Failed to write blended greens.", blended_greens, total_blended_triangles * 3, "%s_%s_%s_blended_greens", name_prefix, name, object_name);
      write_float_array("Failed to write blended blues.", blended_blues, total_blended_triangles * 3, "%s_%s_%s_blended_blues", name_prefix, name, object_name);

      write("Failed to write an object's header.", "static const mesh * const %s_%s_%s_mesh = &((mesh){%d", name_prefix, name, object_name, opaque_cutout_vertices);

      if (opaque_cutout_locations == NULL)
      {
        write("Failed to write an object's opaque and cutout locations.", ", NULL");
      }
      else
      {
        write("Failed to write an object's opaque and cutout locations.", ", %s_%s_%s_opaque_cutout_locations", name_prefix, name, object_name);
      }

      write("Failed to write an object's opaque passes.", ", %d", opaque_passes);

      if (opaque_materials == NULL)
      {
        write("Failed to write an object's opaque textures.", ", NULL");
      }
      else
      {
        write("Failed to write an object's opaque textures.", ", %s_%s_%s_opaque_textures", name_prefix, name, object_name);
      }

      if (opaque_triangles == NULL)
      {
        write("Failed to write an object's opaque triangles.", ", NULL");
      }
      else
      {
        write("Failed to write an object's opaque triangles.", ", %s_%s_%s_opaque_triangles", name_prefix, name, object_name);
      }

      if (opaque_indices == NULL)
      {
        write("Failed to write an object's opaque indices.", ", NULL");
      }
      else
      {
        write("Failed to write an object's opaque indices.", ", %s_%s_%s_opaque_indices", name_prefix, name, object_name);
      }

      if (opaque_rows == NULL)
      {
        write("Failed to write an object's opaque rows.", ", NULL");
      }
      else
      {
        write("Failed to write an object's opaque rows.", ", %s_%s_%s_opaque_rows", name_prefix, name, object_name);
      }

      if (opaque_columns == NULL)
      {
        write("Failed to write an object's opaque columns.", ", NULL");
      }
      else
      {
        write("Failed to write an object's opaque columns.", ", %s_%s_%s_opaque_columns", name_prefix, name, object_name);
      }

      if (opaque_reds == NULL)
      {
        write("Failed to write an object's opaque reds.", ", NULL");
      }
      else
      {
        write("Failed to write an object's opaque reds.", ", %s_%s_%s_opaque_reds", name_prefix, name, object_name);
      }

      if (opaque_greens == NULL)
      {
        write("Failed to write an object's opaque greens.", ", NULL");
      }
      else
      {
        write("Failed to write an object's opaque greens.", ", %s_%s_%s_opaque_greens", name_prefix, name, object_name);
      }

      if (opaque_blues == NULL)
      {
        write("Failed to write an object's opaque blues.", ", NULL");
      }
      else
      {
        write("Failed to write an object's opaque blues.", ", %s_%s_%s_opaque_blues", name_prefix, name, object_name);
      }

      write("Failed to write an object's cutout passes.", ", %d", cutout_passes);

      if (cutout_materials == NULL)
      {
        write("Failed to write an object's cutout textures.", ", NULL");
      }
      else
      {
        write("Failed to write an object's cutout textures.", ", %s_%s_%s_cutout_textures", name_prefix, name, object_name);
      }

      if (cutout_triangles == NULL)
      {
        write("Failed to write an object's cutout triangles.", ", NULL");
      }
      else
      {
        write("Failed to write an object's cutout triangles.", ", %s_%s_%s_cutout_triangles", name_prefix, name, object_name);
      }

      if (cutout_indices == NULL)
      {
        write("Failed to write an object's cutout indices.", ", NULL");
      }
      else
      {
        write("Failed to write an object's cutout indices.", ", %s_%s_%s_cutout_indices", name_prefix, name, object_name);
      }

      if (cutout_rows == NULL)
      {
        write("Failed to write an object's cutout rows.", ", NULL");
      }
      else
      {
        write("Failed to write an object's cutout rows.", ", %s_%s_%s_cutout_rows", name_prefix, name, object_name);
      }

      if (cutout_columns == NULL)
      {
        write("Failed to write an object's cutout columns.", ", NULL");
      }
      else
      {
        write("Failed to write an object's cutout columns.", ", %s_%s_%s_cutout_columns", name_prefix, name, object_name);
      }

      if (cutout_opacities == NULL)
      {
        write("Failed to write an object's cutout opacities.", ", NULL");
      }
      else
      {
        write("Failed to write an object's cutout opacities.", ", %s_%s_%s_cutout_opacities", name_prefix, name, object_name);
      }

      if (cutout_reds == NULL)
      {
        write("Failed to write an object's cutout reds.", ", NULL");
      }
      else
      {
        write("Failed to write an object's cutout reds.", ", %s_%s_%s_cutout_reds", name_prefix, name, object_name);
      }

      if (cutout_greens == NULL)
      {
        write("Failed to write an object's cutout greens.", ", NULL");
      }
      else
      {
        write("Failed to write an object's cutout greens.", ", %s_%s_%s_cutout_greens", name_prefix, name, object_name);
      }

      if (cutout_blues == NULL)
      {
        write("Failed to write an object's cutout blues.", ", NULL");
      }
      else
      {
        write("Failed to write an object's cutout blues.", ", %s_%s_%s_cutout_blues", name_prefix, name, object_name);
      }

      write("Failed to write an object's additive and blended vertices.", ", %d", additive_blended_vertices);

      if (additive_blended_locations == NULL)
      {
        write("Failed to write an object's additive and blended locations.", ", NULL");
      }
      else
      {
        write("Failed to write an object's additive and blended locations.", ", %s_%s_%s_additive_blended_locations", name_prefix, name, object_name);
      }

      write("Failed to write an object's additive passes.", ", %d", additive_passes);

      if (additive_materials == NULL)
      {
        write("Failed to write an object's additive textures.", ", NULL");
      }
      else
      {
        write("Failed to write an object's additive textures.", ", %s_%s_%s_additive_textures", name_prefix, name, object_name);
      }

      if (additive_triangles == NULL)
      {
        write("Failed to write an object's additive triangles.", ", NULL");
      }
      else
      {
        write("Failed to write an object's additive triangles.", ", %s_%s_%s_additive_triangles", name_prefix, name, object_name);
      }

      if (additive_indices == NULL)
      {
        write("Failed to write an object's additive indices.", ", NULL");
      }
      else
      {
        write("Failed to write an object's additive indices.", ", %s_%s_%s_additive_indices", name_prefix, name, object_name);
      }

      if (additive_rows == NULL)
      {
        write("Failed to write an object's additive rows.", ", NULL");
      }
      else
      {
        write("Failed to write an object's additive rows.", ", %s_%s_%s_additive_rows", name_prefix, name, object_name);
      }

      if (additive_columns == NULL)
      {
        write("Failed to write an object's additive columns.", ", NULL");
      }
      else
      {
        write("Failed to write an object's additive columns.", ", %s_%s_%s_additive_columns", name_prefix, name, object_name);
      }

      if (additive_reds == NULL)
      {
        write("Failed to write an object's additive reds.", ", NULL");
      }
      else
      {
        write("Failed to write an object's additive reds.", ", %s_%s_%s_additive_reds", name_prefix, name, object_name);
      }

      if (additive_greens == NULL)
      {
        write("Failed to write an object's additive greens.", ", NULL");
      }
      else
      {
        write("Failed to write an object's additive greens.", ", %s_%s_%s_additive_greens", name_prefix, name, object_name);
      }

      if (additive_blues == NULL)
      {
        write("Failed to write an object's additive blues.", ", NULL");
      }
      else
      {
        write("Failed to write an object's additive blues.", ", %s_%s_%s_additive_blues", name_prefix, name, object_name);
      }

      write("Failed to write an object's blended passes.", ", %d", blended_passes);

      if (blended_materials == NULL)
      {
        write("Failed to write an object's blended textures.", ", NULL");
      }
      else
      {
        write("Failed to write an object's blended textures.", ", %s_%s_%s_blended_textures", name_prefix, name, object_name);
      }

      if (blended_triangles == NULL)
      {
        write("Failed to write an object's blended triangles.", ", NULL");
      }
      else
      {
        write("Failed to write an object's blended triangles.", ", %s_%s_%s_blended_triangles", name_prefix, name, object_name);
      }

      if (blended_indices == NULL)
      {
        write("Failed to write an object's blended indices.", ", NULL");
      }
      else
      {
        write("Failed to write an object's blended indices.", ", %s_%s_%s_blended_indices", name_prefix, name, object_name);
      }

      if (blended_rows == NULL)
      {
        write("Failed to write an object's blended rows.", ", NULL");
      }
      else
      {
        write("Failed to write an object's blended rows.", ", %s_%s_%s_blended_rows", name_prefix, name, object_name);
      }

      if (blended_columns == NULL)
      {
        write("Failed to write an object's blended columns.", ", NULL");
      }
      else
      {
        write("Failed to write an object's blended columns.", ", %s_%s_%s_blended_columns", name_prefix, name, object_name);
      }

      if (blended_opacities == NULL)
      {
        write("Failed to write an object's blended opacities.", ", NULL");
      }
      else
      {
        write("Failed to write an object's blended opacities.", ", %s_%s_%s_blended_opacities", name_prefix, name, object_name);
      }

      if (blended_reds == NULL)
      {
        write("Failed to write an object's blended reds.", ", NULL");
      }
      else
      {
        write("Failed to write an object's blended reds.", ", %s_%s_%s_blended_reds", name_prefix, name, object_name);
      }

      if (blended_greens == NULL)
      {
        write("Failed to write an object's blended greens.", ", NULL");
      }
      else
      {
        write("Failed to write an object's blended greens.", ", %s_%s_%s_blended_greens", name_prefix, name, object_name);
      }

      if (blended_blues == NULL)
      {
        write("Failed to write an object's blended blues.", ", NULL");
      }
      else
      {
        write("Failed to write an object's blended blues.", ", %s_%s_%s_blended_blues", name_prefix, name, object_name);
      }

      write("Failed to write an object's footer.", "});\nconst mesh * %s_%s_%s()\n{\n  return %s_%s_%s_mesh;\n}\n", name_prefix, name, object_name, name_prefix, name, object_name);

      if (opaque_cutout_locations != NULL)
      {
        free(opaque_cutout_locations);
      }

      if (opaque_materials != NULL)
      {
        free(opaque_materials);
        free(opaque_triangles);
        free(opaque_indices);
        free(opaque_rows);
        free(opaque_columns);
        free(opaque_reds);
        free(opaque_greens);
        free(opaque_blues);
      }

      if (cutout_materials != NULL)
      {
        free(cutout_materials);
        free(cutout_triangles);
        free(cutout_indices);
        free(cutout_rows);
        free(cutout_columns);
        free(cutout_opacities);
        free(cutout_reds);
        free(cutout_greens);
        free(cutout_blues);
      }

      if (additive_blended_locations != NULL)
      {
        free(additive_blended_locations);
      }

      if (additive_materials != NULL)
      {
        free(additive_materials);
        free(additive_triangles);
        free(additive_indices);
        free(additive_rows);
        free(additive_columns);
        free(additive_reds);
        free(additive_greens);
        free(additive_blues);
      }

      if (blended_materials != NULL)
      {
        free(blended_materials);
        free(blended_triangles);
        free(blended_indices);
        free(blended_rows);
        free(blended_columns);
        free(blended_opacities);
        free(blended_reds);
        free(blended_greens);
        free(blended_blues);
      }
    }

    free(face_vertex_indices);
    face_vertex_indices = NULL;

    face_indices_length = 0;

    free(face_lengths);
    face_lengths = NULL;
    faces = 0;

    free(object_name);
    object_name = NULL;
  }
}

static int state = STATE_INITIAL;

static void mtl_end_of_line()
{

  switch (state)
  {
  case STATE_INITIAL:
    return;

  case STATE_IGNORING_LINE:
    state = STATE_INITIAL;
    return;

  case STATE_MTL_NEWMTL_CHARACTERS:
    if (get_material_index_by_name(string) == -1)
    {
      materials++;

      if (materials == 1)
      {
        material_names = malloc_or_exit("Failed to allocate a list of material names.", sizeof(char *));
        material_diffuse_texture_indices = malloc_or_exit("Failed to allocate a list of material diffuse texture indices.", sizeof(int));
        material_types = malloc_or_exit("Failed to allocate a list of material types.", sizeof(int));
      }
      else
      {
        realloc_or_exit("Failed to allocate a list of material names.", (void **)&material_names, sizeof(char *) * materials);
        realloc_or_exit("Failed to allocate a list of material diffuse texture indices.", (void **)&material_diffuse_texture_indices, sizeof(int) * materials);
        realloc_or_exit("Failed to allocate a list of material types.", (void **)&material_types, sizeof(int) * materials);
      }

      material_names[materials - 1] = string;
      material_diffuse_texture_indices[materials - 1] = -1;

      if (strlen(string) > 9 && strncmp(string, "additive_", 9) == 0)
      {
        material_types[materials - 1] = MATERIAL_TYPE_ADDITIVE;
      }
      else if (strlen(string) > 7 && strncmp(string, "cutout_", 7) == 0)
      {
        material_types[materials - 1] = MATERIAL_TYPE_CUTOUT;
      }
      else if (strlen(string) > 8 && strncmp(string, "blended_", 8) == 0)
      {
        material_types[materials - 1] = MATERIAL_TYPE_BLENDED;
      }
      else
      {
        material_types[materials - 1] = MATERIAL_TYPE_OPAQUE;
      }

      string = NULL;
      state = STATE_INITIAL;
    }
    else
    {
      fprintf(stderr, "Duplicate material name \"%s\".\n", string);
      exit(1);
    }
    return;

  case STATE_MTL_MAP_UNDERSCORE_KD_CHARACTERS:
  {
    const int texture_index = get_texture_index_by_name(string);

    if (texture_index == -1)
    {
      const int length = strlen(string);

      if (length < 5 || string[length - 4] != '.' || string[length - 3] != 't' || string[length - 2] != 'g' || string[length - 1] != 'a')
      {
        fprintf(stderr, "Non-TGA diffuse texture \"%s\".\n", string);
        exit(1);
      }
      else
      {
        material_diffuse_texture_indices[materials - 1] = textures;

        textures++;

        if (textures == 1)
        {
          texture_names = malloc_or_exit("Failed to allocate a list of texture names.", sizeof(char *));
          texture_variable_names = malloc_or_exit("Failed to allocate a list of texture variable_names.", sizeof(char *));
          textures_imported = malloc_or_exit("Failed to allocate a list of whether textures have been imported.", sizeof(bool));
        }
        else
        {
          realloc_or_exit("Failed to allocate a list of texture names.", (void **)&texture_names, sizeof(char *) * textures);
          realloc_or_exit("Failed to allocate a list of texture variable names.", (void **)&texture_variable_names, sizeof(char *) * textures);
          realloc_or_exit("Failed to allocate a list of whether textures have been imported.", (void **)&textures_imported, sizeof(bool) * textures);
        }

        char *variable_name = malloc_or_exit("Failed to allocate a texture's variable name.", sizeof(char) * (length - 3));

        for (int index = 0; index < length - 4; index++)
        {
          const char character = string[index];
          variable_name[index] = character == '/' || character == '\\' ? '_' : character;
        }

        variable_name[length - 4] = 0;

        texture_names[textures - 1] = string;
        texture_variable_names[textures - 1] = variable_name;
        textures_imported[textures - 1] = false;
      }
    }
    else
    {
      material_diffuse_texture_indices[materials - 1] = texture_index;
      free(string);
    }

    string = NULL;
    state = STATE_INITIAL;
    return;
  }
  }

  fprintf(stderr, "Unexpected end-of-line in MTL state %d.\n", state);
  exit(1);
}

static void mtl_hash()
{
  mtl_end_of_line();
  state = STATE_IGNORING_LINE;
}

static void mtl_space()
{
  switch (state)
  {
  case STATE_INITIAL:
  case STATE_IGNORING_LINE:
    return;

  case STATE_MTL_NEWMTL:
    state = STATE_MTL_NEWMTL_SPACE;
    return;

  case STATE_MTL_NS:
  case STATE_MTL_KA:
  case STATE_MTL_KS:
  case STATE_MTL_KE:
  case STATE_MTL_NI:
  case STATE_MTL_D:
  case STATE_MTL_ILLUM:
    state = STATE_IGNORING_LINE;
    return;

  case STATE_MTL_MAP_UNDERSCORE_KD:
    if (materials)
    {
      if (material_diffuse_texture_indices[materials - 1] == -1)
      {
        state = STATE_MTL_MAP_UNDERSCORE_KD_SPACE;
        return;
      }
      else
      {
        fprintf(stderr, "Cannot add multiple diffuse maps to the same material.\n");
        exit(1);
      }
    }
    else
    {
      fprintf(stderr, "Cannot add a diffuse map to a material which has not been created.\n");
      exit(1);
    }
  }

  fprintf(stderr, "Unexpected space in MTL state %d.\n", state);
  exit(1);
}

static void mtl_other(const char character)
{
  switch (state)
  {
  case STATE_INITIAL:
    switch (character)
    {
    case 'n':
    case 'N':
      state = STATE_MTL_N;
      return;

    case 'K':
      state = STATE_MTL_K;
      return;

    case 'd':
      state = STATE_MTL_D;
      return;

    case 'i':
      state = STATE_MTL_I;
      return;

    case 'm':
      state = STATE_MTL_M;
      return;
    }
    break;

  case STATE_MTL_N:
    switch (character)
    {
    case 'e':
      state = STATE_MTL_NE;
      return;

    case 's':
      state = STATE_MTL_NS;
      return;

    case 'i':
      state = STATE_MTL_NI;
      return;
    }
    break;

  case STATE_MTL_NE:
    switch (character)
    {
    case 'w':
      state = STATE_MTL_NEW;
      return;
    }
    break;

  case STATE_MTL_NEW:
    switch (character)
    {
    case 'm':
      state = STATE_MTL_NEWM;
      return;
    }
    break;

  case STATE_MTL_NEWM:
    switch (character)
    {
    case 't':
      state = STATE_MTL_NEWMT;
      return;
    }
    break;

  case STATE_MTL_NEWMT:
    switch (character)
    {
    case 'l':
      state = STATE_MTL_NEWMTL;
      return;
    }
    break;

  case STATE_MTL_NEWMTL_SPACE:
    state = STATE_MTL_NEWMTL_CHARACTERS;
    string_append(character);
    return;

  case STATE_MTL_NEWMTL_CHARACTERS:
    string_append(character);
    return;

  case STATE_MTL_K:
    switch (character)
    {
    case 'a':
      state = STATE_MTL_KA;
      return;

    case 's':
      state = STATE_MTL_KS;
      return;

    case 'e':
      state = STATE_MTL_KE;
      return;
    }
    break;

  case STATE_MTL_I:
    switch (character)
    {
    case 'l':
      state = STATE_MTL_IL;
      return;
    }
    break;

  case STATE_MTL_IL:
    switch (character)
    {
    case 'l':
      state = STATE_MTL_ILL;
      return;
    }
    break;

  case STATE_MTL_ILL:
    switch (character)
    {
    case 'u':
      state = STATE_MTL_ILLU;
      return;
    }
    break;

  case STATE_MTL_ILLU:
    switch (character)
    {
    case 'm':
      state = STATE_MTL_ILLUM;
      return;
    }
    break;

  case STATE_MTL_M:
    switch (character)
    {
    case 'a':
      state = STATE_MTL_MA;
      return;
    }
    break;

  case STATE_MTL_MA:
    switch (character)
    {
    case 'p':
      state = STATE_MTL_MAP;
      return;
    }
    break;

  case STATE_MTL_MAP:
    switch (character)
    {
    case '_':
      state = STATE_MTL_MAP_UNDERSCORE;
      return;
    }
    break;

  case STATE_MTL_MAP_UNDERSCORE:
    switch (character)
    {
    case 'K':
      state = STATE_MTL_MAP_UNDERSCORE_K;
      return;
    }
    break;

  case STATE_MTL_MAP_UNDERSCORE_K:
    switch (character)
    {
    case 'd':
      state = STATE_MTL_MAP_UNDERSCORE_KD;
      return;
    }
    break;

  case STATE_MTL_MAP_UNDERSCORE_KD_SPACE:
    state = STATE_MTL_MAP_UNDERSCORE_KD_CHARACTERS;
    string_append(character);
    return;

  case STATE_MTL_MAP_UNDERSCORE_KD_CHARACTERS:
    string_append(character);
    return;

  case STATE_IGNORING_LINE:
    return;
  }

  fprintf(stderr, "Unexpected \"%c\" in MTL state %d.\n", character, state);
  exit(1);
}

static void obj_letter(char character)
{
  switch (state)
  {
  case STATE_INITIAL:
    switch (character)
    {
    case 'm':
      state = STATE_OBJ_M;
      return;

    case 'v':
      state = STATE_OBJ_V;
      return;

    case 'o':
      state = STATE_OBJ_O;
      return;

    case 's':
      state = STATE_IGNORING_LINE;
      return;

    case 'u':
      state = STATE_OBJ_U;
      return;

    case 'f':
      state = STATE_OBJ_F;
      return;
    }
    break;

  case STATE_IGNORING_LINE:
    return;

  case STATE_OBJ_M:
    switch (character)
    {
    case 't':
      state = STATE_OBJ_MT;
      return;
    }
    break;

  case STATE_OBJ_MT:
    switch (character)
    {
    case 'l':
      state = STATE_OBJ_MTL;
      return;
    }
    break;

  case STATE_OBJ_MTL:
    switch (character)
    {
    case 'l':
      state = STATE_OBJ_MTLL;
      return;
    }
    break;

  case STATE_OBJ_MTLL:
    switch (character)
    {
    case 'i':
      state = STATE_OBJ_MTLLI;
      return;
    }
    break;

  case STATE_OBJ_MTLLI:
    switch (character)
    {
    case 'b':
      state = STATE_OBJ_MTLLIB;
      return;
    }
    break;

  case STATE_OBJ_MTLLIB_SPACE:
    state = STATE_OBJ_MTLLIB_CHARACTERS;
    // Fall through.

  case STATE_OBJ_MTLLIB_CHARACTERS:
    string_append(character);
    return;

  case STATE_OBJ_O_SPACE:
    state = STATE_OBJ_O_CHARACTERS;
    // Fall through.

  case STATE_OBJ_O_CHARACTERS:
    string_append(character);
    return;

  case STATE_OBJ_V:
    switch (character)
    {
    case 'n':
      state = STATE_IGNORING_LINE;
      return;

    case 't':
      state = STATE_OBJ_VT;
      return;
    }
    break;

  case STATE_OBJ_U:
    switch (character)
    {
    case 's':
      state = STATE_OBJ_US;
      return;
    }
    break;

  case STATE_OBJ_US:
    switch (character)
    {
    case 'e':
      state = STATE_OBJ_USE;
      return;
    }
    break;

  case STATE_OBJ_USE:
    switch (character)
    {
    case 'm':
      state = STATE_OBJ_USEM;
      return;
    }
    break;

  case STATE_OBJ_USEM:
    switch (character)
    {
    case 't':
      state = STATE_OBJ_USEMT;
      return;
    }
    break;

  case STATE_OBJ_USEMT:
    switch (character)
    {
    case 'l':
      state = STATE_OBJ_USEMTL;
      return;
    }
    break;

  case STATE_OBJ_USEMTL_SPACE:
    string_append(character);
    state = STATE_OBJ_USEMTL_CHARACTERS;
    return;

  case STATE_OBJ_USEMTL_CHARACTERS:
    string_append(character);
    return;
  }

  fprintf(stderr, "Unexpected \"%c\" in OBJ state %d.\n", character, state);
  exit(1);
}

static void obj_end_f_vt()
{
  int index = get_and_reset_integer();

  if (index == 0)
  {
    fprintf(stderr, "Texture coordinate indices cannot be zero.\n");
    exit(1);
  }
  else if (index > 0 && index <= texture_coordinates_length)
  {
    if (!object_is_navigation_mesh)
    {
      face_texture_coordinate_indices[face_indices_length - 1] = index - 1;
    }
    return;
  }
  else if (index < 0 && index >= -texture_coordinates_length)
  {
    if (!object_is_navigation_mesh)
    {
      face_texture_coordinate_indices[face_indices_length - 1] = texture_coordinates_length + index;
    }
    return;
  }
  else
  {
    fprintf(stderr, "Out-of-range texture coordinate index.\n");
    exit(1);
  }
}

static void obj_forward_slash()
{
  switch (state)
  {
  case STATE_IGNORING_LINE:
    return;

  case STATE_OBJ_F_V:
  {
    int index = get_and_reset_integer();

    if (index == 0)
    {
      fprintf(stderr, "Vertex indices cannot be zero.\n");
      exit(1);
    }
    else if (index > 0 && index <= vertices_length)
    {
      if (!object_is_navigation_mesh)
      {
        face_vertex_indices[face_indices_length - 1] = index - 1;
      }
      state = STATE_OBJ_F_V_SEPARATOR;
      return;
    }
    else if (index < 0 && index >= -vertices_length)
    {
      if (!object_is_navigation_mesh)
      {
        face_vertex_indices[face_indices_length - 1] = vertices_length + index;
      }
      state = STATE_OBJ_F_V_SEPARATOR;
      return;
    }
    else
    {
      fprintf(stderr, "Out-of-range vertex index.\n");
      exit(1);
    }
  }

  case STATE_OBJ_F_VT:
    obj_end_f_vt();
    state = STATE_OBJ_F_VT_SEPARATOR;
    return;
  }

  fprintf(stderr, "Unexpected forward slash in OBJ state %d.\n", state);
  exit(1);
}

static void obj_period()
{
  switch (state)
  {
  case STATE_IGNORING_LINE:
    return;

  case STATE_OBJ_MTLLIB_SPACE:
    state = STATE_OBJ_MTLLIB_CHARACTERS;
    // Fall through.

  case STATE_OBJ_MTLLIB_CHARACTERS:
    string_append('.');
    return;

  case STATE_OBJ_V_X_DIGITS:
    state = STATE_OBJ_V_X_DECIMAL;
    string_append('.');
    return;

  case STATE_OBJ_V_Y_DIGITS:
    state = STATE_OBJ_V_Y_DECIMAL;
    string_append('.');
    return;

  case STATE_OBJ_V_Z_DIGITS:
    state = STATE_OBJ_V_Z_DECIMAL;
    string_append('.');
    return;

  case STATE_OBJ_V_R_DIGITS:
    state = STATE_OBJ_V_R_DECIMAL;
    string_append('.');
    return;

  case STATE_OBJ_V_G_DIGITS:
    state = STATE_OBJ_V_G_DECIMAL;
    string_append('.');
    return;

  case STATE_OBJ_V_B_DIGITS:
    state = STATE_OBJ_V_B_DECIMAL;
    string_append('.');
    return;

  case STATE_OBJ_V_O_DIGITS:
    state = STATE_OBJ_V_O_DECIMAL;
    string_append('.');
    return;

  case STATE_OBJ_VT_X_DIGITS:
    state = STATE_OBJ_VT_X_DECIMAL;
    string_append('.');
    return;

  case STATE_OBJ_VT_Y_DIGITS:
    state = STATE_OBJ_VT_Y_DECIMAL;
    string_append('.');
    return;
  }

  fprintf(stderr, "Unexpected period in state OBJ %d.\n", state);
  exit(1);
}

static void obj_underscore()
{
  switch (state)
  {
  case STATE_IGNORING_LINE:
    return;

  case STATE_OBJ_MTLLIB_SPACE:
    state = STATE_OBJ_MTLLIB_CHARACTERS;
    // Fall through.

  case STATE_OBJ_O_CHARACTERS:
  case STATE_OBJ_MTLLIB_CHARACTERS:
  case STATE_OBJ_USEMTL_CHARACTERS:
    string_append('_');
    return;
  }

  fprintf(stderr, "Unexpected underscore in OBJ state %d.\n", state);
  exit(1);
}

static void obj_minus()
{
  switch (state)
  {
  case STATE_IGNORING_LINE:
    return;

  case STATE_OBJ_V_SPACE:
    string_append('-');
    state = STATE_OBJ_V_X_DIGITS;
    return;

  case STATE_OBJ_V_X_SPACE:
    string_append('-');
    state = STATE_OBJ_V_Y_DIGITS;
    return;

  case STATE_OBJ_V_Y_SPACE:
    string_append('-');
    state = STATE_OBJ_V_Z_DIGITS;
    return;

  case STATE_OBJ_V_Z_SPACE:
    string_append('-');
    state = STATE_OBJ_V_R_DIGITS;
    return;

  case STATE_OBJ_V_R_SPACE:
    string_append('-');
    state = STATE_OBJ_V_G_DIGITS;
    return;

  case STATE_OBJ_V_G_SPACE:
    string_append('-');
    state = STATE_OBJ_V_B_DIGITS;
    return;

  case STATE_OBJ_V_B_SPACE:
    string_append('-');
    state = STATE_OBJ_V_O_DIGITS;
    return;

  case STATE_OBJ_VT_SPACE:
    string_append('-');
    state = STATE_OBJ_VT_X_DIGITS;
    return;

  case STATE_OBJ_VT_X_SPACE:
    string_append('-');
    state = STATE_OBJ_VT_Y_DIGITS;
    return;

  case STATE_OBJ_F:
  case STATE_OBJ_F_SPACE:
    string_append('-');
    state = STATE_OBJ_F_V_MINUS;
    return;

  case STATE_OBJ_F_V:
  case STATE_OBJ_F_V_SEPARATOR:
    string_append('-');
    state = STATE_OBJ_F_VT_MINUS;
    return;
  }

  fprintf(stderr, "Unexpected minus in state OBJ %d.\n", state);
  exit(1);
}

static void obj_digit(int digit)
{
  switch (state)
  {
  case STATE_IGNORING_LINE:
    return;

  case STATE_OBJ_V_X_DIGITS:
  case STATE_OBJ_V_X_DECIMAL:
  case STATE_OBJ_V_Y_DIGITS:
  case STATE_OBJ_V_Y_DECIMAL:
  case STATE_OBJ_V_Z_DIGITS:
  case STATE_OBJ_V_Z_DECIMAL:
  case STATE_OBJ_V_R_DIGITS:
  case STATE_OBJ_V_R_DECIMAL:
  case STATE_OBJ_V_G_DIGITS:
  case STATE_OBJ_V_G_DECIMAL:
  case STATE_OBJ_V_B_DIGITS:
  case STATE_OBJ_V_B_DECIMAL:
  case STATE_OBJ_V_O_DIGITS:
  case STATE_OBJ_V_O_DECIMAL:
  case STATE_OBJ_VT_X_DIGITS:
  case STATE_OBJ_VT_X_DECIMAL:
  case STATE_OBJ_VT_Y_DIGITS:
  case STATE_OBJ_VT_Y_DECIMAL:
    string_append(digit + '0');
    return;

  case STATE_OBJ_V_SPACE:
    state = STATE_OBJ_V_X_DIGITS;
    string_append(digit + '0');
    return;

  case STATE_OBJ_V_X_SPACE:
    state = STATE_OBJ_V_Y_DIGITS;
    string_append(digit + '0');
    return;

  case STATE_OBJ_V_Y_SPACE:
    state = STATE_OBJ_V_Z_DIGITS;
    string_append(digit + '0');
    return;

  case STATE_OBJ_V_Z_SPACE:
    state = STATE_OBJ_V_R_DIGITS;
    string_append(digit + '0');
    return;

  case STATE_OBJ_V_R_SPACE:
    state = STATE_OBJ_V_G_DIGITS;
    string_append(digit + '0');
    return;

  case STATE_OBJ_V_G_SPACE:
    state = STATE_OBJ_V_B_DIGITS;
    string_append(digit + '0');
    return;

  case STATE_OBJ_V_B_SPACE:
    state = STATE_OBJ_V_O_DIGITS;
    string_append(digit + '0');
    return;

  case STATE_OBJ_VT_SPACE:
    state = STATE_OBJ_VT_X_DIGITS;
    string_append(digit + '0');
    return;

  case STATE_OBJ_VT_X_SPACE:
    state = STATE_OBJ_VT_Y_DIGITS;
    string_append(digit + '0');
    return;

  case STATE_OBJ_F_SPACE:
  case STATE_OBJ_F_V_MINUS:
    if (!object_is_navigation_mesh && material_index == -1)
    {
      fprintf(stderr, "Cannot add faces before selecting materials.\n");
      exit(1);
    }
    else if (!object_is_navigation_mesh && material_diffuse_texture_indices[material_index] == -1)
    {
      fprintf(stderr, "Cannot add faces using materials lacking diffuse maps.\n");
      exit(1);
    }
    else
    {
      face_lengths[faces - 1]++;

      face_indices_length++;

      if (face_vertex_indices == NULL)
      {
        face_vertex_indices = malloc_or_exit("Failed to allocate memory for a list of face vertex indices.", sizeof(int));
        if (!object_is_navigation_mesh)
        {
          face_texture_coordinate_indices = malloc_or_exit("Failed to allocate memory for a list of face texture coordinate indices.", sizeof(int));
          face_material_indices = malloc_or_exit("Failed to allocate memory for a list of face material indices.", sizeof(char *));
        }
      }
      else
      {
        realloc_or_exit("Failed to allocate memory to extend a list of face vertex indices", (void **)&face_vertex_indices, sizeof(int) * face_indices_length);

        if (!object_is_navigation_mesh)
        {
          realloc_or_exit("Failed to allocate memory to extend a list of face texture coordinate indices", (void **)&face_texture_coordinate_indices, sizeof(int) * face_indices_length);
          realloc_or_exit("Failed to allocate memory to extend a list of face material indices", (void **)&face_material_indices, sizeof(char *) * faces);
        }
      }

      face_vertex_indices[face_indices_length - 1] = -1;

      if (!object_is_navigation_mesh)
      {
        face_texture_coordinate_indices[face_indices_length - 1] = -1;
        face_material_indices[faces - 1] = material_index;
      }

      state = STATE_OBJ_F_V;
      string_append(digit + '0');
    }
    return;

  case STATE_OBJ_F_V:
    string_append(digit + '0');
    return;

  case STATE_OBJ_F_V_SEPARATOR:
  case STATE_OBJ_F_VT_MINUS:
    state = STATE_OBJ_F_VT;
    string_append(digit + '0');
    return;

  case STATE_OBJ_F_VT:
  case STATE_OBJ_O_CHARACTERS:
    string_append(digit + '0');
    return;

  case STATE_OBJ_F_VT_SEPARATOR:
    return;

  case STATE_OBJ_USEMTL_CHARACTERS:
    string_append(digit + '0');
    return;
  }

  fprintf(stderr, "Unexpected digit %d in OBJ state %d.\n", digit, state);
  exit(1);
}

static void obj_space()
{
  switch (state)
  {
  case STATE_IGNORING_LINE:
    return;

  case STATE_OBJ_MTLLIB:
    state = STATE_OBJ_MTLLIB_SPACE;
    return;

  case STATE_OBJ_O:
    state = STATE_OBJ_O_SPACE;
    return;

  case STATE_OBJ_V:
    state = STATE_OBJ_V_SPACE;

    if (vertices == NULL)
    {
      vertices = malloc_or_exit("Failed to allocate memory for the list of vertices.", sizeof(float) * 7);
    }
    else
    {

      realloc_or_exit("Failed to allocate memory to extend the list of vertices.", (void **)&vertices, sizeof(float) * 7 * (vertices_length + 1));
    }

    vertices[vertices_length * 7] = 0.0f;
    vertices[vertices_length * 7 + 1] = 0.0f;
    vertices[vertices_length * 7 + 2] = 0.0f;
    vertices[vertices_length * 7 + 3] = 21.26f;
    vertices[vertices_length * 7 + 4] = 71.52f;
    vertices[vertices_length * 7 + 5] = 7.22f;
    vertices[vertices_length * 7 + 6] = 1.0f;

    vertices_length++;

    return;

  case STATE_OBJ_V_X_DIGITS:
  case STATE_OBJ_V_X_DECIMAL:
    vertices[(vertices_length - 1) * 7] = get_and_reset_float();
    state = STATE_OBJ_V_X_SPACE;
    return;

  case STATE_OBJ_V_Y_DIGITS:
  case STATE_OBJ_V_Y_DECIMAL:
    vertices[(vertices_length - 1) * 7 + 1] = get_and_reset_float();
    state = STATE_OBJ_V_Y_SPACE;
    return;

  case STATE_OBJ_V_Z_DIGITS:
  case STATE_OBJ_V_Z_DECIMAL:
    vertices[(vertices_length - 1) * 7 + 2] = get_and_reset_float();
    state = STATE_OBJ_V_Z_SPACE;
    return;

  case STATE_OBJ_V_R_DIGITS:
  case STATE_OBJ_V_R_DECIMAL:
    vertices[(vertices_length - 1) * 7 + 3] = pow(get_and_reset_float(), 2.2f) * 21.26f;
    state = STATE_OBJ_V_R_SPACE;
    return;

  case STATE_OBJ_V_G_DIGITS:
  case STATE_OBJ_V_G_DECIMAL:
    vertices[(vertices_length - 1) * 7 + 4] = pow(get_and_reset_float(), 2.2f) * 71.52f;
    state = STATE_OBJ_V_G_SPACE;
    return;

  case STATE_OBJ_V_B_DIGITS:
  case STATE_OBJ_V_B_DECIMAL:
    vertices[(vertices_length - 1) * 7 + 5] = pow(get_and_reset_float(), 2.2f) * 7.22f;
    state = STATE_OBJ_V_B_SPACE;
    return;

  case STATE_OBJ_V_O_DIGITS:
  case STATE_OBJ_V_O_DECIMAL:
    vertices[(vertices_length - 1) * 7 + 6] = get_and_reset_float();
    state = STATE_OBJ_V_O_AFTER;
    return;

  case STATE_OBJ_VT:
    state = STATE_OBJ_VT_SPACE;

    if (texture_coordinates == NULL)
    {
      texture_coordinates = malloc_or_exit("Failed to allocate memory for the list of texture coordinates.", sizeof(float) * 2);
      texture_coordinates_length++;
    }
    else
    {
      texture_coordinates_length++;
      realloc_or_exit("Failed to allocate memory to extend the list of texture coordinates.", (void **)&texture_coordinates, sizeof(float) * 2 * texture_coordinates_length);
    }
    return;

  case STATE_OBJ_VT_X_DIGITS:
  case STATE_OBJ_VT_X_DECIMAL:
    texture_coordinates[(texture_coordinates_length - 1) * 2] = get_and_reset_float();
    state = STATE_OBJ_VT_X_SPACE;
    return;

  case STATE_OBJ_VT_Y_DIGITS:
  case STATE_OBJ_VT_Y_DECIMAL:
    texture_coordinates[(texture_coordinates_length - 1) * 2 + 1] = get_and_reset_float();
    state = STATE_OBJ_VT_Y_AFTER;
    return;

  case STATE_OBJ_USEMTL:
    state = STATE_OBJ_USEMTL_SPACE;
    return;

  case STATE_OBJ_F:
    if (object_name == NULL)
    {
      fprintf(stderr, "Cannot create a face prior to setting an object name.\n");
      exit(1);
    }
    else
    {

      faces++;

      if (face_lengths == NULL)
      {
        face_lengths = malloc_or_exit("Failed to allocate memory for face lengths.", sizeof(int));
      }
      else
      {
        realloc_or_exit("Failed to allocate memory to extend face lengths.", (void **)&face_lengths, faces * sizeof(int));
      }

      face_lengths[faces - 1] = 0;

      state = STATE_OBJ_F_SPACE;
      return;
    }

  case STATE_OBJ_F_VT:
    obj_end_f_vt();
    state = STATE_OBJ_F_SPACE;
    return;

  case STATE_OBJ_F_VT_SEPARATOR:
    state = STATE_OBJ_F_SPACE;
    return;
  }

  fprintf(stderr, "Unexpected space in OBJ state %d.\n", state);
  exit(1);
}

static void obj_end_of_line()
{
  switch (state)
  {
  case STATE_INITIAL:
    return;

  case STATE_IGNORING_LINE:
  case STATE_OBJ_F_VT_SEPARATOR:
    state = STATE_INITIAL;
    return;

  case STATE_OBJ_F_VT:
    obj_end_f_vt();
    state = STATE_INITIAL;
    return;

  case STATE_OBJ_MTLLIB_CHARACTERS:
  {
    state = STATE_INITIAL;

    char *mtl_path = malloc_or_exit("Failed to allocate memory for the path to a MTL file.", sizeof(char) * (strlen(mtl_prefix) + strlen(string) + 1));
    strcpy(mtl_path, mtl_prefix);
    strcpy(&mtl_path[strlen(mtl_prefix)], string);
    free(string);
    string = NULL;

    FILE *mtl = fopen(mtl_path, "r");

    if (mtl == NULL)
    {
#ifdef errno
      fprintf(stderr, "Failed to open MTL file \"%s\" (errno %d).\n", mtl_path, errno);
#else
      fprintf(stderr, "Failed to open MTL file \"%s\".\n", mtl_path);
#endif
      exit(1);
    }

    bool run = true;

    while (run)
    {
      int character = fgetc(mtl);

      switch (character)
      {
      case EOF:
        mtl_end_of_line();
        run = false;
        break;

      case '#':
        mtl_hash();
        break;

      case 0x0a:
      case 0x0d:
        mtl_end_of_line();
        break;

      case ' ':
      case 0x09:
        mtl_space();
        break;

      default:
        mtl_other(character);
        break;
      }
    }

    if (fclose(mtl))
    {
      fprintf(stderr, "Failed to close MTL file \"%s\".\n", mtl_path);
      exit(1);
    }

    free(mtl_path);
    return;
  }

  case STATE_OBJ_O_CHARACTERS:
    obj_end_object();

    object_name = string;
    string = NULL;

    object_is_navigation_mesh = strlen(object_name) > 16 && strcmp(&object_name[strlen(object_name) - 16], "_navigation_mesh") == 0;

    state = STATE_INITIAL;
    return;

  case STATE_OBJ_USEMTL_CHARACTERS:
    material_index = get_material_index_by_name(string);

    if (material_index == -1)
    {
      fprintf(stderr, "Unknown material \"%s\".\n", string);
      exit(1);
    }
    else
    {
      free(string);
      string = NULL;
      state = STATE_INITIAL;
      return;
    }

  case STATE_OBJ_V_Z_DIGITS:
  case STATE_OBJ_V_Z_DECIMAL:
    vertices[(vertices_length - 1) * 7 + 2] = get_and_reset_float();
    state = STATE_INITIAL;
    return;

  case STATE_OBJ_V_B_DIGITS:
  case STATE_OBJ_V_B_DECIMAL:
    vertices[(vertices_length - 1) * 7 + 5] = get_and_reset_float();
    state = STATE_INITIAL;
    return;

  case STATE_OBJ_V_O_DIGITS:
  case STATE_OBJ_V_O_DECIMAL:
    vertices[(vertices_length - 1) * 7 + 6] = get_and_reset_float();
    state = STATE_INITIAL;
    return;

  case STATE_OBJ_VT_Y_DIGITS:
  case STATE_OBJ_VT_Y_DECIMAL:
    texture_coordinates[(texture_coordinates_length - 1) * 2 + 1] = get_and_reset_float();
    state = STATE_INITIAL;
    return;

  default:
    fprintf(stderr, "Unexpected end-of-line in OBJ state %d.\n", state);
    exit(1);
  }
}

static void obj_hash()
{
  obj_end_of_line();
  state = STATE_IGNORING_LINE;
}

int main(int argc, char **argv)
{
  if (argc != 11)
  {
    fprintf(stderr, "Expected 11 arguments, actual %d.\n", argc);
    exit(1);
  }

  name_prefix = argv[7];
  name = argv[8];
  mtl_prefix = argv[9];
  texture_prefix = argv[10];

  write("Failed to write the header.", "#include \"%s\"\n#include \"%s\"\n#include \"%s\"\n#include \"%s\"\n#include \"%s\"\n#include \"%s\"\n", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

  bool run = true;

  while (run)
  {
    int character = fgetc(stdin);

    switch (character)
    {
    case EOF:
      obj_end_of_line();
      run = false;
      break;

    case '0' ... '9':
      obj_digit(character - '0');
      break;

    case '-':
      obj_minus();
      break;

    case '/':
      obj_forward_slash();
      break;

    case '#':
      obj_hash();
      break;

    case 0x0a:
    case 0x0d:
      obj_end_of_line();
      break;

    case ' ':
    case 0x09:
      obj_space();
      break;

    case '.':
      obj_period();
      break;

    case '_':
      obj_underscore();
      break;

    case 'a' ... 'z':
      obj_letter(character);
      break;
    }
  }

  obj_end_object();

  return 0;
}
