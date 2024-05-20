#include "claymore/entrypoint.h"

#include "scenes/fps.h"
#include <stdio.h>

const float damping = 0.999;
const float gravity = 1000;

const float max_speed = 0.5f;

const float r = 220;
static vec2 center;

typedef struct {
  vec2 pos;
  vec2 last_pos;
  vec2 vel;
  float radius;
  vec4 color;
} Ball;

typedef struct {
  CmCamera2D camera;
  CmFont *font;
  DA(Ball) balls;
  vec2 cursor;
} Balls;

static void init(CmScene *scene) {
  Balls *balls = cm_scene_alloc_data(scene, sizeof(Balls));
  da_init(&balls->balls, &scene->arena);

  balls->font =
      cm_font_init(&scene->gpu, STR("res/fonts/Ubuntu.ttf"), 20, ErrPanic);

  cm_scene_push(scene, fps_scene_init);
  cm_camera2d_screen(&balls->camera);

  cm_window_get_size(center);
  glm_vec2_divs(center, 2, center);
}

static void physics(Balls *balls, double dt) {
  for (size_t i = 0; i < da_len(&balls->balls); ++i) {
    Ball *ball = &da_get(&balls->balls, i);

    // Gravity
    {
      vec2 grav = {0, gravity};
      glm_vec2_add(ball->vel, grav, ball->vel);
    }

    // Verlet
    {
      // Calculate velocity
      vec2 displacement;
      glm_vec2_sub(ball->pos, ball->last_pos, displacement);
	  glm_vec2_clamp(displacement, -max_speed, +max_speed);

      // Save current position
      glm_vec2_copy(ball->pos, ball->last_pos);

      // Verlet integration
      glm_vec2_scale(ball->vel, dt * dt, ball->vel);
      glm_vec2_add(ball->vel, displacement, displacement);
      glm_vec2_add(ball->pos, displacement, ball->pos);

      // Reset vel
      glm_vec2_zero(ball->vel);
    }

    // Bounds
    {
      vec2 diff;
      glm_vec2_sub(ball->pos, center, diff);
      const float distance = glm_vec2_distance(ball->pos, center);
      if (r - ball->radius < distance) {
        vec2 normal;
        glm_vec2_divs(diff, distance, normal);
        glm_vec2_scale(normal, r - ball->radius, normal);
        glm_vec2_add(center, normal, ball->pos);
      }
    }

    // Collision
    {
      for (size_t j = i; j < da_len(&balls->balls); ++j) {
        Ball *ball2 = &da_get(&balls->balls, j);
        const float distance = glm_vec2_distance(ball->pos, ball2->pos);
        const float min_distance = ball->radius + ball2->radius;
        if (distance < min_distance) {
          vec2 dir;
          glm_vec2_sub(ball->pos, ball2->pos, dir);
          glm_vec2_normalize(dir);
          glm_vec2_scale(dir, (distance - min_distance) / 2, dir);
          glm_vec2_sub(ball->pos, dir, ball->pos);
          glm_vec2_add(ball2->pos, dir, ball2->pos);
        }
      }
    }
  }
}

static void update(CmScene *scene, double dt) {
  Balls *balls = scene->data;

  const float max_balls = 1100.f;
  if (da_len(&balls->balls) < max_balls) {
    static float timer = 0.0f;
    static float countdown = 0.0f;
    countdown -= dt;
    timer += dt;
    if (countdown < 0) {
      countdown = 0.07;

      vec2 pos = {center[0], center[1] - r * 0.9f};
      vec4 red = {.8, 0, 0.7, 1};
      vec4 green = {0, 0.8, 0.7, 1};
      vec4 color;
      glm_vec4_lerp(red, green, da_len(&balls->balls) / max_balls, color);
      da_push(&balls->balls, (Ball){
                                 .pos = {VEC2_ARG(pos)},
                                 .last_pos = {pos[0] + sinf(timer) * 200 * dt,
                                              pos[1] - 100 * dt},
                                 .vel = {0},
                                 .radius = 5,
                                 .color = {VEC4_ARG(color)},
                             });
    }
  }

  const usize sub_steps = 4;
  for (usize i = 0; i < sub_steps; ++i) {
    physics(balls, dt / (double)sub_steps);
  }

  cm_renderer2d_begin(&balls->camera);
  cm_circle(center, (vec2){r, r}, (vec4){0.1, 0.1, 0.1, 1.0});
  cm_renderer2d_end();

  cm_renderer2d_begin(&balls->camera);
  for (size_t i = 0; i < da_len(&balls->balls); i++) {
    Ball *ball = &da_get(&balls->balls, i);
    cm_circle(ball->pos, (vec2){ball->radius, ball->radius}, ball->color);
  }

#define N 20
  char buffer[N];
  usize size = snprintf(buffer, N, "%"USIZE_FMT" Balls", da_len(&balls->balls));
  Str s = str_from_parts(size, buffer);
  cm_font(balls->font, (vec2){10, 50}, s);

  cm_renderer2d_end();
}

static void event(CmScene *scene, CmEvent *event) {
  Balls *balls = scene->data;
  cm_event_cursor(event, {
    balls->cursor[0] = cursor->pos[0];
    balls->cursor[1] = cursor->pos[1];
  });
}

static CmSceneInterface *balls(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
      .event = event,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 480, .title = "balls"},
      .main = balls,
  };
  return &config;
}
