#include "claymore.h"

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } uniform_loc;
};
static struct ShaderData cube_shader;

static struct {
  GLuint vbo;
  GLuint vao;
  GLuint ibo;
} RenderData;

static GLenum draw_mode = GL_FILL;

const float rotation = 90.F;
static mat4 model = GLM_MAT4_IDENTITY_INIT;

struct Vertex {
  vec3 pos;
  vec4 color;
};

static const uint32_t indices_count = 36;

static const float fov = 45.F;

static void camera_controll(CmMouseEvent *event, CmCamera *camera) {
  if (event->action == CM_MOUSE_MOVE) {
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

static void camera_scroll(CmScrollEvent *event, CmCamera *camera) {
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

static void camera_resize(CmWindowEvent *event, CmCamera *camera) {
  (void)event;
  glm_perspective(glm_rad(fov),
                  (float)event->window->width / (float)event->window->height,
                  1 / 100.F, 100.F, camera->projection);
  camera->update = true;
}

static void cube_key_callback(CmKeyEvent *event, CmLayer *layer) {
  if (event->action == CM_KEY_PRESS) {
    event->base.handled = true;
    switch (event->code) {
    case CM_KEY_F5: {
      cm_camera_position(&layer->camera, (vec3) { 0, 0, 4 });
      break;
    }
    case CM_KEY_ESCAPE: {
      // TODO cm_app_close()
      cm_event_dispatch((CmEvent) {
        .type = CM_EVENT_WINDOW_CLOSE,
        .event.window.window = layer->app->window,
      });
      break;
    }
    case CM_KEY_F1: {
      draw_mode = draw_mode == GL_LINE ? GL_FILL : GL_LINE;
      break;
    }
    default:
      event->base.handled = false;
      break;
    }
  }
}

static void cube_init(CmLayer *layer) {
  glfwSwapInterval(1); // Set vsync
  cm_event_set_callback(CM_EVENT_KEYBOARD, (cm_event_callback)cube_key_callback,
                        layer);

  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)camera_controll,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_SCROLL, (cm_event_callback)camera_scroll,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)camera_resize, &layer->camera);

  cube_shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                            "res/shader/basic.fs.glsl");
  cube_shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(cube_shader.id, "u_mvp");

  layer->camera = cm_camera_init_perspective(
      (vec3){0, 0, 4}, (vec3){0}, fov,
      (float)layer->app->window->width / (float)layer->app->window->height);

  struct Vertex cube_vertecies[] = {
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

  glGenBuffers(1, &RenderData.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, RenderData.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertecies), cube_vertecies,
               GL_STATIC_DRAW);

  glGenVertexArrays(1, &RenderData.vao);
  glBindVertexArray(RenderData.vao);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
                        (void *)offsetof(struct Vertex, pos));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
                        (void *)offsetof(struct Vertex, color)); // NOLINT

  const uint32_t cube_indices[] = {
      0, 1, 2, 0, 2, 3, // 1
      4, 5, 6, 4, 6, 7, // 2
      0, 4, 5, 0, 5, 1, // 3
      0, 4, 7, 0, 3, 7, // 4
      1, 2, 5, 5, 6, 2, // 5
      2, 3, 6, 3, 6, 7, // 6
  };

  glGenBuffers(1, &RenderData.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderData.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices,
               GL_STATIC_DRAW);
}

static void cube_update(CmLayer *layer, float dt) {
  glPolygonMode(GL_FRONT_AND_BACK, draw_mode);
  static mat4 mvp;
  (void)dt;

  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(cube_shader.id);
  glUniformMatrix4fv(cube_shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  glBindVertexArray(RenderData.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderData.ibo);
  glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram(0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to normal mode
}

static void cube_free(CmLayer *layer) { (void)layer; }

CmLayerInterface sandbox_cube(void) {
  return (const CmLayerInterface){
      .init = cube_init,
      .free = cube_free,
      .update = cube_update,
  };
}
