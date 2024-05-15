#include "claymore/entrypoint.h"

typedef struct {
  vec2 start;
  vec2 end;
  double duration;
} Keyframe;

typedef struct {
  double timer;
  usize idx;
  usize max;
  Keyframe *keyframes;
} Animation;

static void init(CmScene *scene) {
  Animation *animation = cm_scene_alloc_data(scene, sizeof(*animation));

  static Keyframe KEYFRAMES[] = {
      {{-.5, .5}, {.5, .5}, 3},
      {{.5, .5}, {.5, -.5}, 3},
      {{.5, -.5}, {-.5, -.5}, 3},
      {{-.5, -.5}, {-.5, .5}, 3},
  };
  animation->max = ARRAY_LEN(KEYFRAMES);
  animation->keyframes = KEYFRAMES;
}

static void update(CmScene *scene, double dt) {
  Animation *animation = scene->data;

  Keyframe *keyframe = &animation->keyframes[animation->idx];
  vec2 pos;
  glm_vec2_lerp(keyframe->start, keyframe->end,
                animation->timer / keyframe->duration, pos);

  // cm_quad_begin(GLM_MAT4_IDENTITY);
  // cm_quad_push(pos, (vec2){.25, .25}, 0, (vec4){1, 0, 0, 1});
  // cm_quad_end();

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
