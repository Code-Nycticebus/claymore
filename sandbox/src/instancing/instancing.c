#include "instancing.h"
#include <stdlib.h>

CmLayerInterface sandbox_fps(void);

static CmShader cube_shader;

static CmFont *font;
static const float font_size = 24.F;

static const float fov = 90.F;

size_t transform_count;
typedef struct Transform {
  vec4s color;
  mat4s transform;
} Transform;

#define INSTANCED_DYNAMIC_CUBES 100000

typedef struct {
  vec3 pos;
  vec3 normal;
} Vertex;

typedef struct {
  bool init_stage_over;
  CmVertexBuffer vbo;
  size_t cap;
  size_t static_count;
  size_t count;
  Transform *transforms;
} Transforms;

typedef struct {
  size_t id;
  CmRenderBuffer buffer;
  Transforms transforms;
} Mesh;

#define MESHMANAGER_MAX 10
typedef struct {
  size_t count;
  Mesh meshes[MESHMANAGER_MAX];
} MeshManager;

static MeshManager mesh_manager;

void mesh_manager_finish_init(void) {
  for (size_t i = 0; i < mesh_manager.count; i++) {
    mesh_manager.meshes[i].transforms.init_stage_over = true;
    glBindBuffer(GL_ARRAY_BUFFER, mesh_manager.meshes[i].transforms.vbo.id);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(Transform) * mesh_manager.meshes[i].transforms.count,
                    mesh_manager.meshes[i].transforms.transforms);
    mesh_manager.meshes[i].transforms.static_count =
        mesh_manager.meshes[i].transforms.count;
  }
}

void mesh_manager_free(void) {
  for (size_t i = 0; i < MESHMANAGER_MAX; i++) {
    if (mesh_manager.meshes[i].transforms.transforms != NULL) {
      free(mesh_manager.meshes[i].transforms.transforms);
    }
  }
}

Mesh *mesh_create(const Vertex *vertices, size_t vertices_count,
                  const uint32_t *indices, size_t indices_count,
                  size_t transforms) {
  Mesh mesh = {0};
  mesh.buffer.vertex_buffer = cm_vertex_buffer_create(
      vertices_count, sizeof(Vertex), vertices, GL_STATIC_DRAW);
  mesh.buffer.vertex_attribute =
      cm_vertex_attribute_create(&mesh.buffer.vertex_buffer);
  cm_vertex_attribute_push(&mesh.buffer.vertex_attribute, 3, GL_FLOAT,
                           offsetof(Vertex, pos));
  cm_vertex_attribute_push(&mesh.buffer.vertex_attribute, 3, GL_FLOAT,
                           offsetof(Vertex, normal));
  mesh.buffer.index_buffer = cm_index_buffer_create(
      &mesh.buffer.vertex_attribute, indices_count, indices, GL_STATIC_DRAW);

  if (transforms != 0) {
    mesh.transforms.transforms = malloc(sizeof(Transform) * transforms);
    mesh.transforms.cap = transforms;
    mesh.transforms.vbo = cm_vertex_buffer_create(
        1, sizeof(Transform) * transforms, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.transforms.vbo.id);
    glBindVertexArray(mesh.buffer.vertex_attribute.id);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Transform),
                          (void *)offsetof(Transform, color));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3, 4, GL_FLOAT, GL_TRUE, sizeof(Transform),
        (void *)(offsetof(Transform, transform) + sizeof(vec4s) * 0));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
        4, 4, GL_FLOAT, GL_FALSE, sizeof(Transform),
        (void *)(offsetof(Transform, transform) + sizeof(vec4s) * 1));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(
        5, 4, GL_FLOAT, GL_FALSE, sizeof(Transform),
        (void *)(offsetof(Transform, transform) + sizeof(vec4s) * 2));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(
        6, 4, GL_FLOAT, GL_FALSE, sizeof(Transform),
        (void *)(offsetof(Transform, transform) + sizeof(vec4s) * 3));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
  }
  mesh.id = mesh_manager.count;
  mesh_manager.meshes[mesh_manager.count] = mesh;
  mesh_manager.count++;
  return &mesh_manager.meshes[mesh.id];
}

void mesh_push_static_transform(Mesh *mesh, Transform transform) {
  assert(mesh->transforms.init_stage_over == false);
  assert(mesh->transforms.count < mesh->transforms.cap);
  mesh->transforms.transforms[mesh->transforms.count] = transform;
  mesh->transforms.count++;
}

void mesh_push_dynamic_transform(Mesh *mesh, Transform transform) {
  assert(mesh->transforms.count < mesh->transforms.cap);
  mesh->transforms.transforms[mesh->transforms.count] = transform;
  mesh->transforms.count++;
}

Mesh *cube_mesh;

static void camera_controll(CmMouseEvent *event, CmCamera *camera) {
  if (event->action == CM_MOUSE_MOVE) {
    vec2s dir = cm_mouseinfo_direction();
    glms_vec2_normalize(dir);
    dir = glms_vec2_scale(dir, (float)3);

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      float camera_distance =
          glms_vec3_distance(camera->position, camera->lookat);

      static float rotation_horizontal = 0.F;
      static float rotation_vertical = 0.F;

      rotation_horizontal += glm_rad(-dir.x);
      rotation_vertical += glm_rad(-dir.y);

      const float limit = glm_rad(89.F);
      rotation_vertical = glm_clamp(rotation_vertical, -limit, limit);

      vec3s new_camera_pos;
      new_camera_pos.x = camera->lookat.x + camera_distance *
                                                sinf(rotation_horizontal) *
                                                cosf(rotation_vertical);
      new_camera_pos.y =
          camera->lookat.y + camera_distance * sinf(rotation_vertical);
      new_camera_pos.z = camera->lookat.z + camera_distance *
                                                cosf(rotation_horizontal) *
                                                cosf(rotation_vertical);

      cm_camera_position(camera, new_camera_pos);
    }
  }
}

static void camera_scroll(CmScrollEvent *event, CmCamera *camera) {
  const float min_distance = 1.F;
  const float max_distance = 1000.F;

  vec3s direction;
  vec3s center = {{0, 0, 0}};
  float distance = glms_vec3_distance(center, camera->position);
  direction = glms_vec3_sub(center, camera->position);
  direction = glms_vec3_normalize(direction);
  direction = glms_vec3_scale(direction, (float)event->yoffset * distance / 4);

  vec3s new_camera_pos = glms_vec3_add(camera->position, direction);
  float new_distance = glms_vec3_distance(new_camera_pos, center);
  if (min_distance <= new_distance && new_distance <= max_distance) {
    cm_camera_position(camera, new_camera_pos);
  }
}

static void camera_resize(CmWindowEvent *event, CmCamera *camera) {
  camera->projection = glms_perspective(
      glm_rad(fov), (float)event->window->width / (float)event->window->height,
      1 / 100.F, 100.F);
  camera->update = true;
}

static void cube_key_callback(CmKeyEvent *event, CmLayer *layer) {
  if (event->action == CM_KEY_PRESS) {
    event->base.handled = true;
    switch (event->code) {
    case CM_KEY_F5: {
      cm_camera_position(&layer->camera, (vec3s){{0, 0, 4}});
      break;
    }

    case CM_KEY_F2: {
      static bool vsync = true;
      vsync = !vsync;
      glfwSwapInterval(vsync);
      cm_log_info("vsync turned %s\n", vsync ? "on" : "off");
      break;
    }
    default:
      event->base.handled = false;
      break;
    }
  }
}

static bool instancing_scene_init(CmScene *scene) {
  (void)scene;
  scene->camera = cm_camera_init_perspective(
      (vec3s){{0, 0, 4}}, (vec3s){0}, fov,
      (float)scene->app->window->width / (float)scene->app->window->height);
  cube_shader = cm_shader_load_from_file("res/shader/cube.vs.glsl",
                                         "res/shader/cube.fs.glsl");

  Vertex cube_vertices[] = {
      // Front
      {{0.F, 0.F, 0.F}, {0, 0, 1}},
      {{0.F, 1.F, 0.F}, {0, 0, 1}},
      {{1.F, 1.F, 0.F}, {0, 0, 1}},
      {{1.F, 0.F, 0.F}, {0, 0, 1}},

      // Right
      {{1.F, 0.F, 0.F}, {1, 0, 0}},
      {{1.F, 1.F, 0.F}, {1, 0, 0}},
      {{1.F, 1.F, -1.F}, {1, 0, 0}},
      {{1.F, 0.F, -1.F}, {1, 0, 0}},

      // Left
      {{0.F, 0.F, 0.F}, {-1, 0, 0}},
      {{0.F, 0.F, -1.F}, {-1, 0, 0}},
      {{0.F, 1.F, -1.F}, {-1, 0, 0}},
      {{0.F, 1.F, 0.F}, {-1, 0, 0}},

      // Back
      {{0.F, 0.F, -1.F}, {0, 0, -1}},
      {{1.F, 0.F, -1.F}, {0, 0, -1}},
      {{1.F, 1.F, -1.F}, {0, 0, -1}},
      {{0.F, 1.F, -1.F}, {0, 0, -1}},

      // Top
      {{1.F, 1.F, 0.F}, {0, 1, 0}},
      {{0.F, 1.F, 0.F}, {0, 1, 0}},
      {{0.F, 1.F, -1.F}, {0, 1, 0}},
      {{1.F, 1.F, -1.F}, {0, 1, 0}},

      // Bottom
      {{1.F, 0.F, 0.F}, {0, -1, 0}},
      {{1.F, 0.F, -1.F}, {0, -1, 0}},
      {{0.F, 0.F, -1.F}, {0, -1, 0}},
      {{0.F, 0.F, 0.F}, {0, -1, 0}},
  };
  const size_t vertices_count =
      sizeof(cube_vertices) / sizeof(cube_vertices[0]);

  const uint32_t cube_indices[] = {
      0,  1,  2,  0,  2,  3,  // Front
      4,  5,  6,  4,  6,  7,  // Right
      8,  9,  10, 8,  10, 11, // Left
      12, 13, 14, 12, 14, 15, // Back
      16, 17, 18, 16, 18, 19, // Top
      20, 21, 22, 20, 22, 23, // Bottom
  };
  const size_t indices_count = sizeof(cube_indices) / sizeof(cube_indices[0]);

  cube_mesh = mesh_create(cube_vertices, vertices_count, cube_indices,
                          indices_count, INSTANCED_DYNAMIC_CUBES);

  mesh_manager_finish_init();

  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE, (cm_event_callback)camera_resize,
                     &scene->camera);

  cm_event_subscribe(CM_EVENT_KEYBOARD, (cm_event_callback)cube_key_callback,
                     scene);

  cm_event_subscribe(CM_EVENT_MOUSE, (cm_event_callback)camera_controll,
                     &scene->camera);
  cm_event_subscribe(CM_EVENT_SCROLL, (cm_event_callback)camera_scroll,
                     &scene->camera);
  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE, (cm_event_callback)camera_resize,
                     &scene->camera);

  font = cm_font_init("res/fonts/Ubuntu.ttf", font_size);
  glfwSwapInterval(0);
  return true;
}

static void instancing_scene_update(CmScene *scene, float dt) {
  (void)scene, (void)dt;
  mat4s model = glms_mat4_identity();

  cm_shader_bind(&cube_shader);
  cm_shader_set_mat4(&cube_shader, "u_vp", scene->camera.vp);
  cm_shader_set_mat4(&cube_shader, "u_model", model);

  cm_shader_set_vec3(&cube_shader, "u_light_pos", (vec3s){0});
  cm_shader_set_vec3(&cube_shader, "u_view_pos", scene->camera.position);

  static size_t grid_size = 3;
  assert(grid_size < 1000 && "grid to big");
  const float dt_max = 1 / 65.F;
  const float dt_min = 1 / 57.F;
  static float rotation = 0;
  const vec3s size = (vec3s){{.5F, .5F, .5F}};
  rotation += 4 * dt;
  grid_size += dt < dt_max ? +1 : dt > dt_min ? -1 : 0;
  for (size_t x = 0; x < grid_size; x++) {
    for (size_t y = 0; y < grid_size; y++) {
      for (size_t z = 0; z < grid_size; z++) {
        mat4s trans = glms_translate_make((vec3s){{x, y, z}});
        mat4s rot = glms_rotate_make(glm_rad(rotation), (vec3s){{1, 1, 1}});
        mat4s scale = glms_scale_make(size);
        Transform t;
        t.transform = glms_mat4_mul(trans, rot);
        t.transform = glms_mat4_mul(t.transform, scale);
        t.color = (vec4s){{x / (float)grid_size, y / (float)grid_size,
                           z / (float)grid_size, 1}};
        mesh_push_dynamic_transform(cube_mesh, t);
      }
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, cube_mesh->transforms.vbo.id);
  glBufferSubData(
      GL_ARRAY_BUFFER, sizeof(Transform) * cube_mesh->transforms.static_count,
      sizeof(Transform) *
          (cube_mesh->transforms.count - cube_mesh->transforms.static_count),
      &cube_mesh->transforms.transforms[cube_mesh->transforms.static_count]);

  glBindVertexArray(cube_mesh->buffer.vertex_attribute.id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_mesh->buffer.index_buffer.id);
  glDrawElementsInstanced(GL_TRIANGLES, cube_mesh->buffer.index_buffer.count,
                          GL_UNSIGNED_INT, NULL, cube_mesh->transforms.count);

  cube_mesh->transforms.count = cube_mesh->transforms.static_count;
  cm_shader_unbind();

#define LABEL_SIZE 128
  char label_buffer[LABEL_SIZE];
  const size_t len = snprintf(label_buffer, LABEL_SIZE - 1, "%lu cubes",
                              grid_size * grid_size * (uint64_t)grid_size);
  mat4s mvp = glms_mat4_mul(model, scene->camera.vp);
  cm_font_draw(font, mvp, 0.F, -font_size, -100.F, len, label_buffer);
}

static void instancing_scene_free(CmScene *scene) {
  (void)scene;
  mesh_manager_free();
}

CmSceneInterface scene_instancing(void) {
  return (CmSceneInterface){
      .init = instancing_scene_init,
      .update = instancing_scene_update,
      .free = instancing_scene_free,
      .layers = {sandbox_fps, NULL},
  };
}
