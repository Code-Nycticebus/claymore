#include "claymore.h"

struct ShaderData {
  uint32_t id;

  struct {
    GLint mvp;
  } uniform_loc;
};

static struct ShaderData shader;

static void gui_mouse_callback(CmMouseEvent *event, CmScene *scene) {
  if (event->info.pos[0] < 100.F &&
      event->info.pos[1] < (float)scene->app->window->height / 2) {
    if (event->action == CM_MOUSE_CLICK) {
      if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
        printf("OVERLAY CLICK!\n");
      }
    }
    event->base.handled = true;
  }
}

static void gui_resize_callback(CmWindowEvent *event, CmCamera *camera) {
  glm_ortho(0.0F, (float)event->window->width, 0.0F,
            (float)event->window->height, -1.F, 1.F, camera->projection);
  camera->update = true;
}

static bool gui_init(CmScene *scene, CmLayer *layer) {
  shader.id = cm_load_shader_from_file("res/shader/basic.vs.glsl",
                                       "res/shader/basic.fs.glsl");
  shader.uniform_loc.mvp = cm_shader_get_uniform_location(shader.id, "u_mvp");

  glm_ortho(0.0F, (float)scene->app->window->width, 0.0F,
            (float)scene->app->window->height, -100.F, 100.F,
            layer->camera.projection);

  glm_mat4_identity(layer->camera.view);
  glm_vec3_copy((vec3){0}, layer->camera.position);
  glm_translate(layer->camera.view, layer->camera.position);
  layer->camera.update = true;

  cm_event_set_callback(CM_EVENT_MOUSE, (cm_event_callback)gui_mouse_callback,
                        scene);
  cm_event_set_callback(CM_EVENT_WINDOW_RESIZE,
                        (cm_event_callback)gui_resize_callback, &layer->camera);
  return true;
}

static void gui_update(CmScene *scene, CmLayer *layer, float dt) {
  glDisable(GL_DEPTH_TEST);
  (void)dt;
  mat4 mvp;
  mat4 model;
  glm_mat4_identity(model);
  glm_mat4_mul(layer->camera.vp, model, mvp);

  glUseProgram(shader.id);
  glUniformMatrix4fv(shader.uniform_loc.mvp, 1, GL_FALSE, (float *)mvp);

  cm_renderer2d_begin();
  cm_renderer2d_push_quad_color(
      (vec2){0, 0}, 1.F, (vec2){100.F, (float)scene->app->window->height / 2},
      (vec4){0.F, 1.F, 1.F, 1.F});
  cm_renderer2d_end();
  glUseProgram(0);
  glEnable(GL_DEPTH_TEST);
}

static void gui_free(CmScene *scene, CmLayer *layer) {
  (void)layer, (void)scene;
}

CmLayerInterface sandbox_gui(void) {
  return (CmLayerInterface){
      .init = gui_init,
      .free = gui_free,
      .update = gui_update,
  };
}
