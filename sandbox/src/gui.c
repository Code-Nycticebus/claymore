#include "claymore.h"

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } uniform_loc;
};

static struct ShaderData shader;

static void gui_mouse_callback(CmMouseEvent *event, CmLayer *layer) {
  if (event->info.pos[0] < 100.F &&
      event->info.pos[1] < layer->app->window->height / 2) {
    if (event->action == CM_MOUSE_CLICK) {
      if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
        printf("OVERLAY CLICK!\n");
      }
    }
    event->base.handled = true;
  }
}

static void gui_init(CmLayer *layer) {
  shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                       "res/shader/basic.fs.glsl");
  shader.uniform_loc.mvp = cm_shader_get_uniform_location(shader.id, "u_mvp");

  glm_ortho(0.0F, (float)layer->app->window->width, 0.0F,
            (float)layer->app->window->height, -100.F, 100.F,
            layer->camera.projection);

  glm_mat4_identity(layer->camera.view);
  layer->camera.update = true;

  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)gui_mouse_callback,
                        layer);
}

static void gui_update(CmLayer *layer, float dt) {
  glDisable(GL_DEPTH_TEST);

  static float time = 0;
  time += dt;
  if (time > 1) {
#define TITLE_MAX 128
    char title[TITLE_MAX] = {0};
    snprintf(title, TITLE_MAX - 1, "%s FPS: %0.f", layer->app->window->title,
             1 / dt);
    glfwSetWindowTitle(layer->app->window->ctx, title);
    time = 0;
  }

  mat4 mvp;
  mat4 model;

  glm_mat4_identity(model);
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(shader.id);
  glUniformMatrix4fv(shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer2d_begin();
  cm_renderer2d_push_quad_color(
      (vec2){0, 0}, 1.F, (vec2){100.F, (float)layer->app->window->height / 2},
      (vec4){0.F, 1.F, 1.F, 1.F});
  cm_renderer2d_end();

  cm_renderer2d_begin();
  static float past_time = 0;
  static float y = 0;
  y += 100.F * sinf(past_time) * dt * 2;
  past_time += dt;
  y = glm_clamp(y, 0.F, layer->app->window->height - 100.F);

  const uint32_t num_quads = 8000;
  const float alpha_factor = (1.F / (float)num_quads);
  const float xs = (float)layer->app->window->width / num_quads;
  for (size_t i = 0; i < num_quads; i++) {
    float x = xs * i;
    float a = alpha_factor * i + alpha_factor;
    cm_renderer2d_push_quad_color((vec2){x, y}, 1.F, (vec2){xs, 100.F},
                                  (vec4){1.F, 0.F, 0.F, a});
  }
  cm_renderer2d_end();

  glEnable(GL_DEPTH_TEST);
}

static void gui_free(CmLayer *layer) { (void)layer; }

CmLayerInterface sandbox_gui(void) {
  return (CmLayerInterface){
      .init = gui_init,
      .free = gui_free,
      .update = gui_update,
  };
}
