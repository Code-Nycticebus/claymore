#include "claymore/entrypoint.h"

const vec4 color = {.2, .2, .2, 1};
const vec2 size = {100, 100};

typedef struct {
  vec2 start;
  vec2 end;
  double duration;
} Keyframe;

typedef struct {
  CmCamera2D camera;
  double timer;
  usize idx;
  usize max;
  Keyframe *keyframes;
} Animation;

static void init(CmScene *scene) {
  Animation *animation = cm_scene_alloc_data(scene, sizeof(*animation));

  static Keyframe KEYFRAMES[] = {
      {{25, 300}, {500, 300}, 2}, // NOLINT
      {{500, 300}, {500, 25}, 1}, // NOLINT
      {{500, 25}, {25, 25}, 2},   // NOLINT
      {{25, 25}, {25, 300}, 1},   // NOLINT
  };
  animation->max = ARRAY_LEN(KEYFRAMES);
  animation->keyframes = KEYFRAMES;

  cm_camera2d_screen(&animation->camera);
}

static void update(CmScene *scene, double dt) {
  Animation *animation = scene->data;

  Keyframe *keyframe = &animation->keyframes[animation->idx];
  vec2 pos;
  glm_vec2_lerp(keyframe->start, keyframe->end,
                animation->timer / keyframe->duration, pos);

  cm_renderer2d_begin(&animation->camera);
  { cm_quad(pos, size, 0, color); }
  cm_renderer2d_end();

  animation->timer += dt;
  if (keyframe->duration <= animation->timer) {
    animation->idx++;
    animation->timer = 0;
  }
  animation->idx = animation->idx % animation->max;
}

static CmSceneInterface *animation(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Animation"},
      .main = animation,
  };
  return &config;
}
