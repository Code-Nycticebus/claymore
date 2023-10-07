#include "cglm/types-struct.h"
#include "claymore.h"
#include <stdlib.h>

extern CmLayerInterface sandbox_fps(void);
static CmShader shader;

typedef struct {
  vec2s pos;
  vec2s size;
  vec4s color;
  float rotation;
  vec2s vel;
  vec4s color_start;
  vec4s color_end;
  float lifetime;
  bool active;
} Particle;

const float particle_lifetime = 2.F;

#define PARTICLES_MAX 10000

struct {
  size_t index;
  Particle pool[PARTICLES_MAX];
} particle_pool = {.index = PARTICLES_MAX};

float rand_float(void) { return rand() / (float)RAND_MAX; }

void particle_emit(vec2s pos) {
  const float size = rand_float() * 6.F + 3.F;
  const Particle particle = {
      .pos = pos,
      .size = {{size, size}},
      .rotation = rand_float() * 360.F,
      .active = true,
      .color = {0},
      .color_end = {{1, .5F, 0, 1}},
      .color_start = {{0, .5F, 1, 1}},
      .lifetime = 0,
      .vel = {{rand_float() * 300.F - 150.F, rand_float() * 300.F - 150.F}},
  };

  particle_pool.pool[particle_pool.index] = particle;
  --particle_pool.index;
  if (particle_pool.index == 0) {
    particle_pool.index = PARTICLES_MAX;
  }
}

void particle_render(mat4s mvp, float dt) {
  cm_shader_bind(&shader);
  cm_shader_set_mat4(&shader, "u_mvp", mvp);
  cm_renderer2d_begin();
  for (size_t i = 0; i < PARTICLES_MAX; i++) {
    Particle *particle = &particle_pool.pool[i];
    if (particle->active) {
      vec2s vel = glms_vec2_scale(particle->vel, dt);
      particle->pos = glms_vec2_add(particle->pos, vel);

      float progress = particle->lifetime / particle_lifetime;
      particle->color =
          glms_vec4_lerp(particle->color_end, particle->color_start, progress);
      particle->color.a = 1.F - particle->color.a * progress;

      particle->lifetime += dt;
      if (particle_lifetime <= particle->lifetime) {
        particle->active = false;
      }
      cm_renderer2d_push_quad_color_rotated(particle->pos, 0, particle->size,
                                            particle->color,
                                            particle->rotation);
    }
  }
  cm_renderer2d_end();
  cm_shader_unbind();
}

bool particle_scene_init(CmScene *scene) {
  (void)scene;
  shader = cm_shader_load_from_file("res/shader/basic.vs.glsl",
                                    "res/shader/basic.fs.glsl");
  float aspect =
      (float)scene->app->window->width / (float)scene->app->window->height;
  scene->camera = cm_camera_init_ortho((vec3s){0}, aspect, 100.F);

  glfwSwapInterval(1);
  return true;
}

void particle_scene_update(CmScene *scene, float dt) {
  if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
    vec2s mouse_pos = cm_mouseinfo_pos();
    vec2s screen_point = {{
        (2 * mouse_pos.x / (float)scene->app->window->width) - 1.F,
        (2 * mouse_pos.y / (float)scene->app->window->height) - 1.F,
    }};

    vec4s screenspace = {{screen_point.x, screen_point.y, 0, 1}};
    mat4s inverse_projection = glms_mat4_inv(scene->camera.projection);
    vec4s view_coords = glms_mat4_mulv(inverse_projection, screenspace);
    mat4s inverse_view = glms_mat4_inv(scene->camera.view);
    vec4s world_space = glms_mat4_mulv(inverse_view, view_coords);

    const size_t num_particles = 10;
    for (size_t i = 0; i < num_particles; i++) {
      particle_emit((vec2s){{world_space.x, world_space.y}});
    }
  }

  mat4s mvp = glms_mat4_mul(glms_mat4_identity(), scene->camera.vp);
  particle_render(mvp, dt);
}

void particle_scene_free(CmScene *scene) { (void)scene; }

CmSceneInterface scene_particle(void) {
  return (CmSceneInterface){
      .free = particle_scene_free,
      .update = particle_scene_update,
      .init = particle_scene_init,
      .layers =
          {
              sandbox_fps,
              NULL,
          },
  };
}
