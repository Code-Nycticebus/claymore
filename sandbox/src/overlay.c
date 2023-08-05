#include "claymore.h"
#include "claymore/events/event.h"
#include "claymore/events/mouse.h"
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
  if (event->info.pos[0] < 100.F && event->info.pos[1] < app->window->height) {

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      if (event->action == CM_MOUSE_CLICK) {
        printf("OVERLAY CLICK!\n");
      }
      event->base.handled = true;
    }
  }
}
static bool overlay_init(CmLayer *layer) {
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

static bool overlay_update(CmLayer *layer, float dt) {
  glDisable(GL_DEPTH_TEST);

  static float time = 0;
  time += dt;
  if (time > 1) {
#define TITLE_MAX 128
    char title[TITLE_MAX];
    snprintf(title, TITLE_MAX, "%s FPS: %0.f", layer->app->window->title,
             1 / dt);
    glfwSetWindowTitle(layer->app->window->ctx, title);
    time = 0;
  }

  mat4 vp;
  mat4 mvp;
  mat4 model;

  glm_ortho(0.0F, (float)layer->app->window->width, 0.0F,
            (float)layer->app->window->height, near, 100.0F,
            layer->camera.projection);

  // Calculates camera perspective
  glm_mat4_mul(layer->camera.projection, layer->camera.view, vp);

  glm_mat4_identity(model);
  glm_mat4_mul(vp, model, mvp);

  glUseProgram(shader.id);
  glUniformMatrix4fv(shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer_begin();
  cm_renderer_push_quad_color(
      (vec2){0, 0}, 0, (vec2){100.F, (float)layer->app->window->height / 2},
      (vec4){0.F, 1.F, 1.F, 1.F});
  cm_renderer_end();

  glEnable(GL_DEPTH_TEST);
  return true;
}

static bool overlay_free(CmLayer *layer) {
  (void)layer;
  return true;
}

CmLayerInterface sandbox_overlay(void) {
  return (CmLayerInterface){
      .init = overlay_init,
      .free = overlay_free,
      .update = overlay_update,
  };
}
