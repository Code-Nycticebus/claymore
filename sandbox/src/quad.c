#include "cglm/vec2.h"
#include "claymore.h"

#include "miniaudio.h"

struct Vertex {
  vec3 pos;
  vec2 uv;
};

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
    GLint texture;
  } uniform_loc;
};

static struct ShaderData grid_shader;

static mat4 model = GLM_MAT4_IDENTITY_INIT;

static float zoom = 100.F;
static float aspect;

static void quad_scroll_callback(CmScrollEvent *event, CmLayer *layer) {
  const float min_zoom = 1.F;
  const float scroll_speed = 100.F;
  zoom = glm_max(zoom - event->yoffset * scroll_speed, min_zoom);
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F,
            layer->camera.projection);
  layer->camera.update = true;
}

static void quad_window_resize_callback(CmWindowEvent *event,
                                        CmCamera *camera) {
  aspect = (float)event->window->width / (float)event->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 1.F,
            camera->projection);
  camera->update = true;
}

static void quad_mouse_callback(CmMouseEvent *event, CmLayer *layer) {
  if (event->action == CM_MOUSE_MOVE) {
    static vec3 last_position = {0};
    vec2 pos;
    vec2 direction;
    cm_mouseinfo_pos(pos);
    glm_vec2_sub(pos, last_position, direction);
    glm_vec2_copy(pos, last_position);

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      glm_vec2_scale(direction, zoom / 100.F, direction);
      glm_vec2_add(layer->camera.position, direction, layer->camera.position);

      glm_mat4_identity(layer->camera.view);
      glm_translate(layer->camera.view, layer->camera.position);
      layer->camera.update = true;
    }
  }
}

#define KEY_QUEUE_MAX 2

struct {
  size_t bottom;
  size_t count;
  CmKeyCodes keys[KEY_QUEUE_MAX];
} queue;

void queue_push(CmKeyCodes key) {
  if (queue.count < KEY_QUEUE_MAX) {
    size_t index = queue.bottom + queue.count;
    if (index < KEY_QUEUE_MAX) {
      queue.keys[index] = key;
      queue.count++;
    } else {
      queue.count = 0;
      queue.bottom = 0;
      queue_push(key);
    }
  }
}

CmKeyCodes queue_poll(void) {
  if (queue.count > 0) {
    CmKeyCodes key = queue.keys[queue.bottom];
    queue.bottom++;
    queue.count--;
    return key;
  }
  queue.bottom = 0;
  return -1;
}

typedef struct {
  bool moving;
  float timer;
  vec2 end_pos;
  vec2 start_pos;
} QuadMover;

typedef struct {
  vec2 pos;
  vec2 size;
  vec4 color;
  QuadMover move;
} Quad;

static void quad_key_callback(CmKeyEvent *event, CmLayer *layer) {
  (void)layer;
  if (event->action == CM_KEY_PRESS) {
    event->base.handled = true;
    queue_push(event->code);
  }
}

static void draw_quad(Quad *quad) {
  cm_renderer2d_begin();
  cm_renderer2d_push_quad_color(quad->pos, 0.F, quad->size, quad->color);
  cm_renderer2d_end();
}

static bool move_quad(Quad *quad) {
  const float duration = .25F;
  const float progress = glm_min(quad->move.timer / duration, 1.F);
  glm_vec2_lerp(quad->move.start_pos, quad->move.end_pos, progress, quad->pos);
  if (progress >= 1.F) {
    quad->move.timer = 0.F;
    return true;
  }
  return false;
}

static void render_quad(float dt, Quad *quad) {
  if (quad->move.moving) {
    if (quad->move.timer == 0) {
      glm_vec2_copy(quad->pos, quad->move.start_pos);
      cm_sound_play("res/sound/efx/foom.wav");
    }
    quad->move.timer += dt;
    if (move_quad(quad)) {
      quad->move.moving = false;
      quad->move.timer = 0;
    }
  }
  draw_quad(quad);
}

static void quad_init(CmLayer *layer) {
  grid_shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                            "res/shader/basic.fs.glsl");
  grid_shader.uniform_loc.mvp =
      cm_shader_get_uniform_location(grid_shader.id, "u_mvp");

  aspect = (float)layer->app->window->width / (float)layer->app->window->height;
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)quad_window_resize_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)quad_mouse_callback,
                        &layer->camera);
  cm_event_set_callback(CM_EVENT_SCROLL,
                        (cm_event_callback)quad_scroll_callback, layer);

  cm_event_set_callback(CM_EVENT_KEYBOARD, (cm_event_callback)quad_key_callback,
                        layer);
}

static void quad_update(CmLayer *layer, float dt) {
  (void)dt;

  static mat4 mvp;
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(grid_shader.id);
  glUniformMatrix4fv(grid_shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);
#define PLAYER_COLOR                                                           \
  { .2F, .7F, .8F, 1.F }

  static Quad player = {
      .pos = {0}, .color = PLAYER_COLOR, .size = {100.F, 100.F}};

  if (!player.move.moving) {
    CmKeyCodes key = queue_poll();
    if (key == CM_KEY_A) {
      player.move.moving = true;
      player.move.end_pos[0] = player.pos[0] - 100.F;
    } else if (key == CM_KEY_D) {
      player.move.moving = true;
      player.move.end_pos[0] = player.pos[0] + 100.F;
    } else if (key == CM_KEY_W) {
      player.move.moving = true;
      player.move.end_pos[1] = player.pos[1] + 100.F;
    } else if (key == CM_KEY_S) {
      player.move.moving = true;
      player.move.end_pos[1] = player.pos[1] - 100.F;
    }
  }

  render_quad(dt, &player);

  glUseProgram(0);
}

static void quad_free(CmLayer *layer) { (void)layer; }

CmLayerInterface sandbox_quad(void) {
  return (CmLayerInterface){
      .init = quad_init,
      .free = quad_free,
      .update = quad_update,
  };
}
