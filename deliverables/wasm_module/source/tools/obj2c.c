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

static char **material_names = NULL;
static int *material_diffuse_texture_indices = NULL;
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
      int *written_texture_indices = NULL;
      int written_texture_indices_length = 0;
      int number_of_triangulated_indices = 0;

      for (int face_index = 0; face_index < faces; face_index++)
      {
        number_of_triangulated_indices += (face_lengths[face_index] - 2) * 3;

        const int face_material_index = face_material_indices[face_index];
        const int face_texture_index = material_diffuse_texture_indices[face_material_index];

        bool found = false;

        for (int written_texture_indices_index = 0; written_texture_indices_index < written_texture_indices_length; written_texture_indices_index++)
        {
          if (written_texture_indices[written_texture_indices_index] == face_texture_index)
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          written_texture_indices_length++;

          if (written_texture_indices == NULL)
          {
            written_texture_indices = malloc_or_exit("Failed to allocate a list of written texture indices.", sizeof(int));
          }
          else
          {
            realloc_or_exit("Failed to allocate a list of written texture indices.", (void **)&written_texture_indices, sizeof(int) * written_texture_indices_length);
          }

          written_texture_indices[written_texture_indices_length - 1] = face_texture_index;
        }
      }

      int *remapped_vertex_indices = malloc_or_exit("Failed to allocate memory for a list of remapped vertex indices.", sizeof(int) * vertices_length);
      float *remapped_reds = malloc_or_exit("Failed to allocate memory for a list of remapped reds.", sizeof(float) * number_of_triangulated_indices);
      float *remapped_greens = malloc_or_exit("Failed to allocate memory for a list of remapped greens.", sizeof(float) * number_of_triangulated_indices);
      float *remapped_blues = malloc_or_exit("Failed to allocate memory for a list of remapped blues.", sizeof(float) * number_of_triangulated_indices);
      float *remapped_opacities = malloc_or_exit("Failed to allocate memory for a list of remapped opacities.", sizeof(float) * number_of_triangulated_indices);

      for (int remapped_vertex_indices_index = 0; remapped_vertex_indices_index < vertices_length; remapped_vertex_indices_index++)
      {
        remapped_vertex_indices[remapped_vertex_indices_index] = -1;
      }

      float *remapped_vertices = NULL;
      int remapped_vertices_length = 0;

      for (int written_texture_indices_index = 0; written_texture_indices_index < written_texture_indices_length; written_texture_indices_index++)
      {
        const int written_texture_index = written_texture_indices[written_texture_indices_index];

        int index = 0;

        for (int face_index = 0; face_index < faces; face_index++)
        {
          int vertex_count = face_lengths[face_index];

          if (vertex_count < 3)
          {
            fprintf(stderr, "Faces must have at least 3 vertices.\n");
            exit(1);
          }
          else
          {
            const int face_material_index = face_material_indices[face_index];
            const int face_texture_index = material_diffuse_texture_indices[face_material_index];

            if (face_texture_index == written_texture_index)
            {
              for (int vertex = 0; vertex < vertex_count; vertex++)
              {
                int face_vertex_index = face_vertex_indices[index];

                if (remapped_vertex_indices[face_vertex_index] == -1)
                {
                  float x = vertices[face_vertex_index * 7];
                  float y = vertices[face_vertex_index * 7 + 1];
                  float z = vertices[face_vertex_index * 7 + 2];

                  for (int remapped_vertex_index = 0; remapped_vertex_index < remapped_vertices_length; remapped_vertex_index++)
                  {
                    if (remapped_vertices[remapped_vertex_index * 3] == x && remapped_vertices[remapped_vertex_index * 3 + 1] == y && remapped_vertices[remapped_vertex_index * 3 + 2] == z)
                    {
                      remapped_vertex_indices[face_vertex_index] = remapped_vertex_index;
                      break;
                    }
                  }

                  if (remapped_vertex_indices[face_vertex_index] == -1)
                  {
                    remapped_vertices_length++;

                    if (remapped_vertices == NULL)
                    {
                      remapped_vertices = malloc_or_exit("Failed to allocate a list of remapped vertices.", sizeof(float) * 3);
                    }
                    else
                    {
                      realloc_or_exit("Failed to allocate a list of remapped vertices.", (void **)&remapped_vertices, sizeof(float) * 3 * remapped_vertices_length);
                    }

                    remapped_vertices[(remapped_vertices_length - 1) * 3] = x;
                    remapped_vertices[(remapped_vertices_length - 1) * 3 + 1] = y;
                    remapped_vertices[(remapped_vertices_length - 1) * 3 + 2] = z;

                    remapped_vertex_indices[face_vertex_index] = remapped_vertices_length - 1;
                  }
                }

                index++;
              }
            }
            else
            {
              index += vertex_count;
            }
          }
        }
      }

      write("Failed to write the header of an object's vertices.", "\nstatic const vector %s_%s_%s_vertices[] = {", name_prefix, name, object_name);

      for (int remapped_vertex_index = 0; remapped_vertex_index < remapped_vertices_length; remapped_vertex_index++)
      {
        write("Failed to write an object's vertices.", "%s{%ff, %ff, %ff}", remapped_vertex_index ? ", " : "", remapped_vertices[remapped_vertex_index * 3], remapped_vertices[remapped_vertex_index * 3 + 1], remapped_vertices[remapped_vertex_index * 3 + 2]);
      }

      write("Failed to write the footer of an object's vertices and the header of its vertex indices.", "};\n\nstatic const index %s_%s_%s_indices[] = {", name_prefix, name, object_name);

      bool first = true;
      int remapped_colors = 0;

      for (int written_texture_indices_index = 0; written_texture_indices_index < written_texture_indices_length; written_texture_indices_index++)
      {
        const int written_texture_index = written_texture_indices[written_texture_indices_index];

        int index = 0;

        for (int face_index = 0; face_index < faces; face_index++)
        {
          const int vertex_count = face_lengths[face_index];

          const int face_material_index = face_material_indices[face_index];
          const int face_texture_index = material_diffuse_texture_indices[face_material_index];

          if (face_texture_index == written_texture_index)
          {
            int first_index = face_vertex_indices[index++];
            int first_remapped_index = remapped_vertex_indices[first_index];

            int previous_index = face_vertex_indices[index++];
            int previous_remapped_index = remapped_vertex_indices[previous_index];

            for (int triangle = 0; triangle < vertex_count - 2; triangle++)
            {
              int next_index = face_vertex_indices[index++];

              remapped_reds[remapped_colors] = vertices[first_index * 7 + 3];
              remapped_greens[remapped_colors] = vertices[first_index * 7 + 4];
              remapped_blues[remapped_colors] = vertices[first_index * 7 + 5];
              remapped_opacities[remapped_colors] = vertices[first_index * 7 + 6];
              remapped_colors++;

              remapped_reds[remapped_colors] = vertices[previous_index * 7 + 3];
              remapped_greens[remapped_colors] = vertices[previous_index * 7 + 4];
              remapped_blues[remapped_colors] = vertices[previous_index * 7 + 5];
              remapped_opacities[remapped_colors] = vertices[previous_index * 7 + 6];
              remapped_colors++;

              int next_remapped_index = remapped_vertex_indices[next_index];

              remapped_reds[remapped_colors] = vertices[next_index * 7 + 3];
              remapped_greens[remapped_colors] = vertices[next_index * 7 + 4];
              remapped_blues[remapped_colors] = vertices[next_index * 7 + 5];
              remapped_opacities[remapped_colors] = vertices[next_index * 7 + 6];
              remapped_colors++;

              write("Failed to write an object's vertex indices.", "%s%d, %d, %d", first ? "" : ", ", first_remapped_index, previous_remapped_index, next_remapped_index);
              first = false;

              previous_index = next_index;
              previous_remapped_index = next_remapped_index;
            }
          }
          else
          {
            index += vertex_count;
          }
        }
      }

      write("Failed to write the footer of an object's vertex indices and the header of its texture coordinates.", "};\n\nstatic const f32 %s_%s_%s_texture_coordinates[] = {", name_prefix, name, object_name);

      first = true;

      for (int written_texture_indices_index = 0; written_texture_indices_index < written_texture_indices_length; written_texture_indices_index++)
      {
        const int written_texture_index = written_texture_indices[written_texture_indices_index];

        int index = 0;

        for (int face_index = 0; face_index < faces; face_index++)
        {
          int vertices = face_lengths[face_index];

          const int face_material_index = face_material_indices[face_index];
          const int face_texture_index = material_diffuse_texture_indices[face_material_index];

          if (face_texture_index == written_texture_index)
          {
            int first_texture_coordinate_index = face_texture_coordinate_indices[index++] * 2;
            float first_u = texture_coordinates[first_texture_coordinate_index];
            float first_v = 1.0f - texture_coordinates[first_texture_coordinate_index + 1];

            int previous_texture_coordinate_index = face_texture_coordinate_indices[index++] * 2;
            float previous_u = texture_coordinates[previous_texture_coordinate_index];
            float previous_v = 1.0f - texture_coordinates[previous_texture_coordinate_index + 1];

            for (int triangle = 0; triangle < vertices - 2; triangle++)
            {
              int next_texture_coordinate_index = face_texture_coordinate_indices[index++] * 2;
              float next_u = texture_coordinates[next_texture_coordinate_index];
              float next_v = 1.0 - texture_coordinates[next_texture_coordinate_index + 1];

              write("Failed to write an object's texture coordinates.", "%s%f, %f, %f, %f, %f, %f", first ? "" : ", ", first_v, first_u, previous_v, previous_u, next_v, next_u);
              first = false;

              previous_u = next_u;
              previous_v = next_v;
            }
          }
          else
          {
            index += vertices;
          }
        }
      }

      write("Failed to write the footer of an object's texture coordinates.", "};\n");

      first = true;

      for (int written_texture_indices_index = 0; written_texture_indices_index < written_texture_indices_length; written_texture_indices_index++)
      {
        const int written_texture_index = written_texture_indices[written_texture_indices_index];

        if (!textures_imported[written_texture_index])
        {
          write("Failed to write a texture import.", "%sextern const texture * %s_%s();\n", first ? "\n" : "", texture_prefix, texture_variable_names[written_texture_index]);
          textures_imported[written_texture_index] = true;
          first = false;
        }
      }

      write("Failed to write the header of an object's texture images.", "\nstatic texture_factory * const %s_%s_%s_textures[] = {", name_prefix, name, object_name);

      for (int written_texture_indices_index = 0; written_texture_indices_index < written_texture_indices_length; written_texture_indices_index++)
      {
        write("Failed to write an object's texture images.", "%s%s_%s", written_texture_indices_index ? ", " : "", texture_prefix, texture_variable_names[written_texture_indices[written_texture_indices_index]]);
      }

      write("Failed to write the footer of an object's textures and the header of its triangle counts per textures.", "};\n\nconst quantity %s_%s_%s_triangles[] = {", name_prefix, name, object_name);

      for (int written_texture_indices_index = 0; written_texture_indices_index < written_texture_indices_length; written_texture_indices_index++)
      {
        const int written_texture_index = written_texture_indices[written_texture_indices_index];

        int total = 0;

        for (int face_index = 0; face_index < faces; face_index++)
        {
          int vertices = face_lengths[face_index];

          const int face_material_index = face_material_indices[face_index];
          const int face_texture_index = material_diffuse_texture_indices[face_material_index];

          if (face_texture_index == written_texture_index)
          {
            total += vertices - 2;
          }
        }

        write("Failed to write the number of triangles using a texture.", "%s%d", written_texture_indices_index ? ", " : "", total);
      }

      write("Failed to write the footer of an object's triangle counts per textures and the header of its reds.", "};\n\nconst f32 %s_%s_%s_reds[] = {", name_prefix, name, object_name);

      for (int index = 0; index < number_of_triangulated_indices; index++)
      {
        write("Failed to write a red.", "%s%f", index ? ", " : "", remapped_reds[index]);
      }

      write("Failed to write the footer of an object's reds and the header of its greens.", "};\n\nconst f32 %s_%s_%s_greens[] = {", name_prefix, name, object_name);

      for (int index = 0; index < number_of_triangulated_indices; index++)
      {
        write("Failed to write a green.", "%s%f", index ? ", " : "", remapped_greens[index]);
      }

      write("Failed to write the footer of an object's greens and the header of its blues.", "};\n\nconst f32 %s_%s_%s_blues[] = {", name_prefix, name, object_name);

      for (int index = 0; index < number_of_triangulated_indices; index++)
      {
        write("Failed to write a blue.", "%s%f", index ? ", " : "", remapped_blues[index]);
      }

      write("Failed to write the footer of an object's blues and the header of its opacities.", "};\n\nconst f32 %s_%s_%s_opacities[] = {", name_prefix, name, object_name);

      for (int index = 0; index < number_of_triangulated_indices; index++)
      {
        write("Failed to write an opacity.", "%s%f", index ? ", " : "", remapped_opacities[index]);
      }

      write("Failed to write the footer.", "};\n\nstatic const mesh %s_%s_%s_mesh = {%d, %s_%s_%s_vertices, %d, %s_%s_%s_textures, %s_%s_%s_triangles, %s_%s_%s_indices, %s_%s_%s_texture_coordinates, %s_%s_%s_reds, %s_%s_%s_greens, %s_%s_%s_blues, %s_%s_%s_opacities};\n\nconst mesh * %s_%s_%s()\n{\n  return &%s_%s_%s_mesh;}\n", name_prefix, name, object_name, remapped_vertices_length, name_prefix, name, object_name, written_texture_indices_length, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name, name_prefix, name, object_name);

      // TODO: synchronize names with headers

      free(remapped_reds);
      free(remapped_greens);
      free(remapped_blues);
      free(remapped_opacities);
      free(remapped_vertices);
      free(remapped_vertex_indices);
      free(written_texture_indices);

      free(face_texture_coordinate_indices);
      face_texture_coordinate_indices = NULL;

      free(face_material_indices);
      face_material_indices = NULL;
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
      }
      else
      {
        realloc_or_exit("Failed to allocate a list of material names.", (void **)&material_names, sizeof(char *) * materials);
        realloc_or_exit("Failed to allocate a list of material diffuse texture indices.", (void **)&material_diffuse_texture_indices, sizeof(int) * materials);
      }

      material_names[materials - 1] = string;
      material_diffuse_texture_indices[materials - 1] = -1;

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

// TODO: check triangle sizes - not just edge lengths

// TODO: Add vertex count check
