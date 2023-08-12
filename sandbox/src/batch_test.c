#include "claymore.h"

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } uniform_loc;
};

static struct ShaderData shader;

static void batch_resize_callback(CmWindowEvent *event, CmCamera *camera) {
  glm_ortho(0.0F, (float)event->window->width, 0.0F,
            (float)event->window->height, -100.F, 100.F, camera->projection);
  camera->update = true;
}

static void batch_init(CmLayer *layer) {

  shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                       "res/shader/basic.fs.glsl");
  shader.uniform_loc.mvp = cm_shader_get_uniform_location(shader.id, "u_mvp");

  glm_ortho(0.0F, (float)layer->app->window->width, 0.0F,
            (float)layer->app->window->height, -100.F, 100.F,
            layer->camera.projection);
  glm_mat4_identity(layer->camera.view);
  layer->camera.update = true;

  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)batch_resize_callback,
                        &layer->camera);
}

static void batch_update(CmLayer *layer, float dt) {
  mat4 mvp;
  mat4 model;

  glm_mat4_identity(model);
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(shader.id);
  glUniformMatrix4fv(shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer2d_begin();
  static float past_time = 0;
  static float y = 0;
  y += (layer->app->window->height / 4) * sinf(past_time) * dt * 2;
  past_time += dt;
  y = glm_clamp(y, 0.F, layer->app->window->height - 100.F);

  const uint32_t num_quads = 50000;
  const float alpha_factor = (1.F / (float)num_quads);
  const float xs = (float)layer->app->window->width / num_quads;

  for (size_t i = 0; i < num_quads; i++) {
    float x = xs * i;
    float a = alpha_factor * i + alpha_factor;
    cm_renderer2d_push_quad_color((vec2){x, y}, 1.F, (vec2){xs, 100.F},
                                  (vec4){1.F, 0.F, 0.F, a});
  }
  cm_renderer2d_end();
}

static void batch_free(CmLayer *layer) { (void)layer; }

CmLayerInterface sandbox_batch_test(void) {
  return (CmLayerInterface){
      .init = batch_init,
      .free = batch_free,
      .update = batch_update,
  };
}
