
#include "claymore.h"
#include "claymore/events/event.h"

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } uniform_loc;
};

static struct {
  GLuint vbo;
  GLuint vba;
  GLuint ibo;
} RenderData;
static struct ShaderData shader;

const float rotation = 90.F;
mat4 model = GLM_MAT4_IDENTITY_INIT;

const struct Vertex {
  vec3 pos;
  vec4 color;
} vertecies[] = {
    // 1
    {{-1.F, -1.F, 1.F}, {1.F, 1.F, 0.F, 1.F}},
    {{1.F, -1.F, 1.F}, {1.F, 0.F, 0.F, 1.F}},
    {{1.F, 1.F, 1.F}, {0.F, 1.F, 1.F, 1.F}},
    {{-1.F, 1.F, 1.F}, {1.F, 0.F, 0.F, 1.F}},

    // 2
    {{-1.F, -1.F, -1.F}, {0.F, 0.F, 1.F, 1.F}},
    {{1.F, -1.F, -1.F}, {0.F, 1.F, 1.F, 1.F}},
    {{1.F, 1.F, -1.F}, {1.F, 0.F, 1.F, 1.F}},
    {{-1.F, 1.F, -1.F}, {1.F, 1.F, 0.F, 1.F}},
};

const uint32_t indices[] = {
    0, 1, 2, 0, 2, 3, // 1
    4, 5, 6, 4, 6, 7, // 2
    0, 4, 5, 0, 5, 1, // 3
    0, 4, 7, 0, 3, 7, // 4
    1, 2, 5, 5, 6, 2, // 5
    2, 3, 6, 3, 6, 7, // 6
};

uint32_t indices_count = sizeof(indices) / sizeof(indices[0]);

const float fov = 45.F;

static void layer_camera_controll(CmMouseEvent *event, CmCamera *camera) {
  if (event->action == CM_MOUSE_CLICK) {
    printf("LAYER CLICK!\n");
    event->base.handled = true;
  } else if (event->action == CM_MOUSE_MOVE) {
    vec2 mouse_pos;
    cm_mouseinfo_pos(mouse_pos);
    static vec2 last_pos = {0, 0};

    vec2 dir;
    glm_vec2_sub(mouse_pos, last_pos, dir);
    glm_vec2_normalize(dir);
    glm_vec2_scale(dir, (float)3, dir);

    glm_vec2_copy(mouse_pos, last_pos);

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      float camera_distance =
          glm_vec3_distance(camera->position, camera->lookat);

      static float rotation_horizontal = 0.F;
      static float rotation_vertical = 0.F;

      rotation_horizontal += glm_rad(-dir[0]);
      rotation_vertical += glm_rad(-dir[1]);

      const float limit = glm_rad(89.F);
      rotation_vertical = glm_clamp(rotation_vertical, -limit, limit);

      vec3 new_camera_pos;
      new_camera_pos[0] = camera->lookat[0] + camera_distance *
                                                  sinf(rotation_horizontal) *
                                                  cosf(rotation_vertical);
      new_camera_pos[1] =
          camera->lookat[1] + camera_distance * sinf(rotation_vertical);
      new_camera_pos[2] = camera->lookat[2] + camera_distance *
                                                  cosf(rotation_horizontal) *
                                                  cosf(rotation_vertical);

      cm_camera_position(camera, new_camera_pos);
    }
  }
}

static void layer_camera_scroll(CmScrollEvent *event, CmCamera *camera) {
  vec3 direction;
  vec3 center = {0, 0, 0};
  const float max_distance = 0.1F;

  glm_vec3_sub(center, camera->position, direction);
  glm_vec3_normalize(direction);
  glm_vec3_scale(direction, (float)event->yoffset, direction);

  vec3 new_camera_pos;
  glm_vec3_add(camera->position, direction, new_camera_pos);
  float new_distance = glm_vec3_distance(new_camera_pos, center);
  if (max_distance <= new_distance) {
    cm_camera_position(camera, new_camera_pos);
  }
}

static void layer_camera_resize(CmWindowEvent *event, CmCamera *camera) {
  (void)event;
  glm_perspective(glm_rad(fov),
                  (float)event->window->width / (float)event->window->height,
                  1 / 100.F, 100.F, camera->projection);
  camera->update = true;
}

static void sandbox_key_callback(CmKeyEvent *event, CmLayer *layer) {
  if (event->action == CM_KEY_PRESS) {
    if (event->code == CM_KEY_F5) {
      cm_camera_position(&layer->camera, (vec3){0, 0, 4});
      event->base.handled = true;
    } else if (event->code == CM_KEY_ESCAPE) {
      cm_event_dispatch((CmEvent){
          .type = CM_EVENT_WINDOW_CLOSE,
          .event.window.window = layer->app->window,
      });
      event->base.handled = true;
    }
  }
}

static void sandbox_init(CmLayer *layer) {
  glfwSwapInterval(0); // Set vsync
  cm_event_set_callback(CM_EVENT_KEYBOARD,
                        (cm_event_callback)sandbox_key_callback, layer);

  cm_event_set_callback(
      CM_EVENT_MOUSE, (cm_event_callback)layer_camera_controll, &layer->camera);
  cm_event_set_callback(CM_EVENT_SCROLL, (cm_event_callback)layer_camera_scroll,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)layer_camera_resize, &layer->camera);

  shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                       "res/shader/basic.fs.glsl");
  shader.uniform_loc.mvp = cm_shader_get_uniform_location(shader.id, "u_mvp");

  layer->camera = cm_camera_init_perspective(
      (vec3){0, 0, 4}, (vec3){0}, fov,
      (float)layer->app->window->width / (float)layer->app->window->height);

  glGenBuffers(1, &RenderData.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, RenderData.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

  glGenVertexArrays(1, &RenderData.vba);
  glBindVertexArray(RenderData.vba);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
                        (void *)offsetof(struct Vertex, pos));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
                        (void *)offsetof(struct Vertex, color)); // NOLINT

  glGenBuffers(1, &RenderData.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderData.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
}

static void sandbox_update(CmLayer *layer, float dt) {
  mat4 mvp;
  (void)dt;

  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(shader.id);
  glUniformMatrix4fv(shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderData.ibo);
  glBindVertexArray(RenderData.vba);

  glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, NULL);
}

static void sandbox_free(CmLayer *layer) { (void)layer; }

CmLayerInterface sandbox_layer(void) {
  return (const CmLayerInterface){
      .init = sandbox_init,
      .free = sandbox_free,
      .update = sandbox_update,
  };
}
