#include "texture.h"

static CmShader texture_shader;
static CmFont *text_font;
static const float font_size = 32.F;

static CmTexture *texture = NULL;

static void resize_callback(CmWindowEvent *event, CmCamera *camera) {
  cm_camera_set_screen(camera, event->window->width, event->window->height);
}

static void drop_callback(CmDropEvent *event, CmScene *scene) {

  if (texture) {
    cm_texture_delete(texture);
  }

  if (!texture) {
    texture = malloc(sizeof(CmTexture));
  }
  *texture = cm_texture_create(event->files[0]);

  cm_window_set_size(scene->app->window, texture->width, texture->height);

  cm_camera_set_screen(&scene->camera, texture->width, texture->height);
}

static bool texture_scene_init(CmScene *scene) {
  (void)scene;
  scene->camera = cm_camera_screen_init((vec3s){0}, scene->app->window->width,
                                        scene->app->window->height);

  texture_shader = cm_shader_load_from_file("res/shader/texture.vs.glsl",
                                            "res/shader/texture.fs.glsl");

  text_font = cm_font_init("res/fonts/Silkscreen.ttf", font_size);

  cm_event_subscribe(CM_EVENT_DROP, (cm_event_callback)drop_callback, scene);
  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE, (cm_event_callback)resize_callback,
                     &scene->camera);
  return true;
}

static void texture_scene_update(CmScene *scene, float dt) {
  (void)scene, (void)dt;

  mat4s mvp = glms_mat4_mul(scene->camera.vp, glms_mat4_identity());

  if (texture) {
    cm_shader_bind(&texture_shader);
    cm_shader_set_mat4(&texture_shader, "u_mvp", mvp);
    cm_texture_bind(texture, 0);
    cm_renderer2d_begin();
    cm_renderer2d_push_quad_textured(
        (vec2s){{0, 0}}, 0,
        (vec2s){{scene->app->window->width, scene->app->window->height}},
        (vec2s){{0, 0}}, (vec2s){{1, 1}});
    cm_renderer2d_end();
    cm_shader_unbind();
    cm_texture_unbind(0);
  } else {
    const char text[] = "Drag a texture inside the window";
    size_t str_len = (sizeof(text) - 1);
    float y_offset = ((float)scene->app->window->height / 2) - (font_size / 2);
    float x_offset = ((float)scene->app->window->width / 2) -
                     (((float)str_len / 2) * (font_size / 2));
    cm_font_draw_cstr(text_font, mvp, x_offset, y_offset, 0, text);
  }
}

static void texture_scene_free(CmScene *scene) {
  (void)scene;
  cm_shader_delete(&texture_shader);
  cm_font_free(text_font);
  cm_texture_delete(texture);
  free(texture);
  texture = NULL;
}

CmSceneInterface scene_texture(void) {
  return (CmSceneInterface){
      .init = texture_scene_init,
      .update = texture_scene_update,
      .free = texture_scene_free,
      .layers = {0},
  };
}
