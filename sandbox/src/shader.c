#include "claymore/app/scene.h"
#include "claymore/entrypoint.h"

#include "glad.h"

static void init(CmScene *scene) {
  (void)cm_scene_alloc_data(scene, sizeof(CmScene));
}

static void update(CmScene *scene, double dt) {
  (void)dt;
  CmShader *shader = scene->data;
  cm_shader_bind(shader);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
}

static CmSceneInterface *shader(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
  };
  return &interface;
}

static CmScene *shader_init(CmScene *parent, Str filename) {
  CmScene *scene = cm_scene_push(parent, shader);

  Arena arena = {0};
  Str content = file_read_str(filename, &arena, ErrPanic);
  *(CmShader *)scene->data =
      cm_shader_from_memory(&scene->gpu,
                            STR("#version 430 core\n"
                                "void main() {\n"
                                "  gl_Position = vec4(\n"
                                "    (gl_VertexID >> 0 & 1) * 2 - 1,\n"
                                "    (gl_VertexID >> 1 & 1) * 2 - 1,\n"
                                "    0.0,\n"
                                "    1.0\n"
                                "  );\n"
                                "}\n"),
                            content, ErrPanic);
  arena_free(&arena);
  return scene;
}

typedef struct {
  Error error;
  CmScene *child;
  Str filename;
} Toy;

static void toy_init(CmScene *scene) {
  Toy *toy = cm_scene_alloc_data(scene, sizeof(Toy));

  toy->filename = STR("res/shader/toy.fs.glsl");
  toy->child = shader_init(scene, toy->filename);
}

static void event(CmScene *scene, CmEvent *event) {
  Toy *toy = scene->data;
  cm_event_key(event, {
    if (key->action == CM_KEY_RELEASE) {
      if (key->code == CM_KEY_ESCAPE) {
        cm_window_close();
        return;
      }
      if (key->code == CM_KEY_F5) {
        // Releoad
        cm_scene_delete(scene, toy->child);
        toy->child = shader_init(scene, toy->filename);
      }
    }
  });
}

static CmSceneInterface *toy(void) {
  static CmSceneInterface interface = {
      .init = toy_init,
      .event = event,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Shader Toy"},
      .main = toy,
  };
  return &config;
}
