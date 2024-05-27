#include "claymore/entrypoint.h"

const vec2 position = {500, 200};
const vec2 size = {200, 400};
static const Str atlas = STR_STATIC("assets/textures/atlas.png");

typedef struct {
  vec2 position;
  vec2 size;
} Model;

Model model(const vec2 pos, const vec2 size) {
  return (Model){
      .position = {pos[0], pos[1]},
      .size = {size[0], size[1]},
  };
}

typedef struct {
  CmTexture2D texture;
  vec2 size;
} TextureAtlas;

TextureAtlas texture_atlas(CmGpu *gpu, Str filename, vec2 sprite_size) {
  TextureAtlas atlas = {0};
  atlas.texture = cm_texture_from_file(gpu, filename, ErrPanic);

  vec3 dimension = {atlas.texture.width, atlas.texture.height};
  glm_vec2_div((vec2){1, 1}, dimension, atlas.size);
  glm_vec2_mul(atlas.size, sprite_size, atlas.size);

  return atlas;
}

void texure_atlas_draw(TextureAtlas *atlas, vec2 pos, vec2 size, vec2 idx) {
  vec2 uv;
  glm_vec2_mul(atlas->size, idx, uv);
  cm_sprite(&atlas->texture, pos, size, 0, uv, atlas->size);
}

typedef struct {
  float timer;
  float framerate;
  int frame;
  int first;
  int last;
} Animation;

Animation animation(int first, int last) {
  return (Animation){
      .framerate = 1.f / (float)(last - first),
      .frame = first,
      .first = first,
      .last = last,
  };
}

void animation_update(Animation *a, double dt) {
  a->timer += dt;
  if (a->framerate <= a->timer) {
    a->frame++;
    if (a->last <= a->frame) {
      a->frame = a->first;
    }
    a->timer = 0;
  }
}

typedef struct {
  CmCamera2D camera;
  Model model;
  TextureAtlas atlas;
  Animation animation;
} SceneData;

static void init(CmScene *scene) {
  SceneData *data = cm_scene_set_data(scene, sizeof(SceneData));
  cm_camera2D_screen(&data->camera);

  data->model = model(position, size);
  data->atlas = texture_atlas(&scene->gpu, atlas, (vec2){32, 64}); // NOLINT
  data->animation = animation(1, 6);                               // NOLINT
}

static void frame_update(CmScene *scene, double dt) {
  (void)dt;
  SceneData *data = scene->data;
  animation_update(&data->animation, dt);

  cm_2D_begin(&data->camera);
  texure_atlas_draw(&data->atlas, data->model.position, data->model.size,
                    (vec2){data->animation.frame, 0});
  cm_2D_end();
}

static CmSceneInterface *texture(void) {
  static CmSceneInterface interface = {
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 1280, .height = 720, .title = "texture"},
      .main = texture,
  };
  return &config;
}
