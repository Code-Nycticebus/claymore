#include "claymore.h"
#include "claymore/events/event.h"
#include <GL/gl.h>
#include <stdio.h>

static const float near = 0.1F;

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } uniform_loc;
};

static struct ShaderData shader;

static void overlay_mouse_callback(CmApp *app, CmMouseEvent *event) {
  (void)app;
  if (event->action == CM_MOUSE_CLICK) {
    if (event->info.pos[0] < app->window->width / 2 &&
        event->info.pos[1] < app->window->height / 2) {
      printf("OVERLAY CLICK!\n");
      event->base.handled = true;
    }
  }
}

static bool overlay_init(CmLayerData *layer) {

  shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                       "res/shader/basic.fs.glsl");
  shader.uniform_loc.mvp = cm_shader_get_uniform_location(shader.id, "u_mvp");

  glm_ortho(0.0F, (float)layer->app->window->width, 0.0F,
            (float)layer->app->window->height, near, 100.0F,
            layer->camera.projection);

  vec3 up = {0, 1, 0};
  glm_lookat((vec3){0, 0, 3}, (vec3){0, 0, 0}, (float *)up, layer->camera.view);

  cm_event_set_callback(CM_EVENT_MOUSE,
                        (cm_event_callback)overlay_mouse_callback);

  return true;
}

static bool overlay_update(CmLayerData *layer) {
  glDisable(GL_DEPTH_TEST);

  mat4 vp;
  mat4 mvp;
  mat4 model;
  glm_mat4_identity(model);

  glm_ortho(0.0F, (float)layer->app->window->width, 0.0F,
            (float)layer->app->window->height, near, 100.0F,
            layer->camera.projection);

  // Calculates camera perspective
  glm_mat4_mul(layer->camera.projection, layer->camera.view, vp);

  glm_mat4_mul(vp, model, mvp);
  glUseProgram(shader.id);
  glUniformMatrix4fv(shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer_begin();

  cm_renderer_push_quad_color(
      (vec2){0, 0}, 0,
      (vec2){layer->app->window->width / 2, layer->app->window->height / 2},
      (vec4){1.F, 1.F, 1.F, 1.F});

  cm_renderer_end();
  glEnable(GL_DEPTH_TEST);
  return true;
}

static bool overlay_free(CmLayerData *layer) {
  (void)layer;
  return true;
}

CmLayer sandbox_overlay(void) {
  return (CmLayer){
      .init = overlay_init, .free = overlay_free, .update = overlay_update};
}
