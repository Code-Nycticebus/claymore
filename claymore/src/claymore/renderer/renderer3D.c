#include "renderer3D.h"

#include "render_buffer.h"
#include "render_command.h"

#define CM_RENDERER3D_MAX_CUBES 200
#define CM_RENDERER3D_VERTICES_PER_CUBE 24

#define CM_RENDERER3D_MAX_VERTICES                                             \
  (CM_RENDERER3D_MAX_CUBES * CM_RENDERER3D_VERTICES_PER_CUBE)

#define CM_RENDERER3D_INDICES_PER_CUBE 36
#define CM_RENDERER3D_MAX_INDICES                                              \
  (CM_RENDERER3D_MAX_CUBES * CM_RENDERER3D_INDICES_PER_CUBE)

struct Render3dData {
  CmRenderBuffer renderer;

  size_t vertices_count;
  CmVertex3D data[CM_RENDERER3D_MAX_VERTICES];

  size_t indices_count;
  uint32_t indices[CM_RENDERER3D_MAX_INDICES];
};

static struct Render3dData *render_data = NULL;

void cm_renderer3d_init(void) {
  assert(render_data == NULL && "Renderer3d initialized twice");
  render_data = (struct Render3dData *)calloc(1, sizeof(struct Render3dData));

  // Initialize Cube primitive buffer
  render_data->renderer.vertex_buffer = cm_vertex_buffer_create(
      CM_RENDERER3D_MAX_VERTICES, sizeof(CmVertex3D), NULL, GL_DYNAMIC_DRAW);
  render_data->vertices_count = 0;

  render_data->renderer.vertex_attribute =
      cm_vertex_attribute_create(&render_data->renderer.vertex_buffer);
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 3, GL_FLOAT,
                           offsetof(CmVertex3D, pos));
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 4, GL_FLOAT,
                           offsetof(CmVertex3D, color));
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 3, GL_FLOAT,
                           offsetof(CmVertex3D, normal));
  cm_vertex_attribute_push(&render_data->renderer.vertex_attribute, 2, GL_FLOAT,
                           offsetof(CmVertex3D, uv));

  // Generates indices in advance
  const uint32_t base_indices[CM_RENDERER3D_INDICES_PER_CUBE] = {
      0,  1,  2,  0,  2,  3,  // Front
      4,  5,  6,  4,  6,  7,  // Right
      8,  9,  10, 8,  10, 11, // Left
      12, 13, 14, 12, 14, 15, // Back
      16, 17, 18, 16, 18, 19, // Top
      20, 21, 22, 20, 22, 23, // Bottom
  };
  for (size_t i = 0; i < CM_RENDERER3D_MAX_INDICES; ++i) {
    render_data->indices[i] = base_indices[i % CM_RENDERER3D_INDICES_PER_CUBE] +
                              (CM_RENDERER3D_VERTICES_PER_CUBE *
                               (i / CM_RENDERER3D_INDICES_PER_CUBE));
  }
  render_data->indices_count = 0;

  render_data->renderer.index_buffer = cm_index_buffer_create(
      &render_data->renderer.vertex_attribute, CM_RENDERER3D_MAX_INDICES,
      render_data->indices, GL_STATIC_DRAW);

  const float bytes_in_kilboytes = 1000.F;
  cm_log_info("Renderer: %.0f kb\n",
              (uint32_t)sizeof(struct Render3dData) / bytes_in_kilboytes);
}

void cm_renderer3d_shutdown(void) {
  glDeleteBuffers(1, &render_data->renderer.vertex_buffer.id);
  glDeleteVertexArrays(1, &render_data->renderer.vertex_attribute.id);
  glDeleteBuffers(1, &render_data->renderer.index_buffer.id);
  free(render_data);
}

static void cm_renderer3d_flush(void) {
  glBindBuffer(GL_ARRAY_BUFFER, render_data->renderer.vertex_buffer.id);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  sizeof(CmVertex3D) * render_data->vertices_count,
                  render_data->data);

  cm_renderer_draw_indexed(&render_data->renderer, render_data->indices_count);

  render_data->vertices_count = 0;
  render_data->indices_count = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cm_renderer3d_begin(void) {}

void cm_renderer3d_end(void) { cm_renderer3d_flush(); }

static inline vec3s _rotate_around_axis(float angle, vec3s axis,
                                        const vec3s vertex,
                                        const vec3s origin) {
  vec3s direction = glms_vec3_sub(vertex, origin);
  vec3s rotation = glms_vec3_rotate(direction, angle, axis);
  vec3s new_pos = glms_vec3_add(rotation, origin);
  return new_pos;
}

static inline void _cm_renderer3d_push_cube(vec3s pos, vec3s size, vec4s color,
                                            float rotation, vec3s axis) {
  assert(render_data && "Renderer 3D was not initialized!");
  if (!(render_data->vertices_count < CM_RENDERER3D_MAX_VERTICES)) {
    cm_renderer3d_flush();
  }

  // For safety i still assert
  assert(render_data->vertices_count < CM_RENDERER3D_MAX_VERTICES);
  assert(render_data->indices_count < CM_RENDERER3D_MAX_INDICES);

  const struct {
    vec3s position;
    vec3s normal;
  } vertex_data[CM_RENDERER3D_VERTICES_PER_CUBE] = {
      // Front
      {{{pos.x, pos.y, pos.z}}, {{0, 0, 1}}},
      {{{pos.x + size.x, pos.y, pos.z}}, {{0, 0, 1}}},
      {{{pos.x + size.x, pos.y + size.y, pos.z}}, {{0, 0, 1}}},
      {{{pos.x, pos.y + size.y, pos.z}}, {{0, 0, 1}}},

      // Right
      {{{pos.x + size.x, pos.y, pos.z}}, {{1, 0, 0}}},
      {{{pos.x + size.x, pos.y, pos.z - size.z}}, {{1, 0, 0}}},
      {{{pos.x + size.x, pos.y + size.y, pos.z - size.z}}, {{1, 0, 0}}},
      {{{pos.x + size.x, pos.y + size.y, pos.z}}, {{1, 0, 0}}},

      // Left
      {{{pos.x, pos.y, pos.z}}, {{-1, 0, 0}}},
      {{{pos.x, pos.y, pos.z - size.z}}, {{-1, 0, 0}}},
      {{{pos.x, pos.y + size.y, pos.z - size.z}}, {{-1, 0, 0}}},
      {{{pos.x, pos.y + size.y, pos.z}}, {{-1, 0, 0}}},

      // Back
      {{{pos.x, pos.y, pos.z - size.z}}, {{0, 0, -1}}},
      {{{pos.x + size.x, pos.y, pos.z - size.z}}, {{0, 0, -1}}},
      {{{pos.x + size.x, pos.y + size.y, pos.z - size.z}}, {{0, 0, -1}}},
      {{{pos.x, pos.y + size.y, pos.z - size.z}}, {{0, 0, -1}}},

      // Top
      {{{pos.x + size.x, pos.y + size.y, pos.z}}, {{0, 1, 0}}},
      {{{pos.x, pos.y + size.y, pos.z}}, {{0, 1, 0}}},
      {{{pos.x, pos.y + size.y, pos.z - size.z}}, {{0, 1, 0}}},
      {{{pos.x + size.x, pos.y + size.y, pos.z - size.z}}, {{0, 1, 0}}},

      // Bottom
      {{{pos.x + size.x, pos.y, pos.z}}, {{0, -1, 0}}},
      {{{pos.x, pos.y, pos.z}}, {{0, -1, 0}}},
      {{{pos.x, pos.y, pos.z - size.z}}, {{0, -1, 0}}},
      {{{pos.x + size.x, pos.y, pos.z - size.z}}, {{0, -1, 0}}},

  };

  CmVertex3D *vertices = &render_data->data[render_data->vertices_count];
  for (int i = 0; i < CM_RENDERER3D_VERTICES_PER_CUBE; ++i) {
    vertices[i].pos =
        rotation == 0
            ? vertex_data[i].position
            : _rotate_around_axis(rotation, axis, vertex_data[i].position, pos);
    vertices[i].normal =
        rotation == 0
            ? vertex_data[i].normal
            : _rotate_around_axis(rotation, axis, vertex_data[i].normal, pos);
    ;
    vertices[i].color = color;
    // vertices[i].uv.u = text_coord.u + (i == 1 || i == 2 ? text_size.x : 0);
    // vertices[i].uv.v = text_coord.v + (i == 2 || i == 3 ? text_size.y : 0);
  }

  render_data->vertices_count += CM_RENDERER3D_VERTICES_PER_CUBE;
  render_data->indices_count += CM_RENDERER3D_INDICES_PER_CUBE;
}

void cm_renderer3d_push_cube(vec3s position, vec3s size) {
  _cm_renderer3d_push_cube(position, size, (vec4s){{1, 1, 1, 1}}, 0,
                           (vec3s){0});
}
void cm_renderer3d_push_cube_rotated(vec3s position, vec3s size, float rotation,
                                     vec3s axis) {
  _cm_renderer3d_push_cube(position, size, (vec4s){{1, 1, 1, 1}}, rotation,
                           axis);
}

void cm_renderer3d_push_cube_color(vec3s position, vec3s size, vec4s color) {
  _cm_renderer3d_push_cube(position, size, color, 0, (vec3s){0});
}

void cm_renderer3d_push_cube_color_rotated(vec3s position, vec3s size,
                                           vec4s color, float rotation,
                                           vec3s axis) {
  _cm_renderer3d_push_cube(position, size, color, rotation, axis);
}
