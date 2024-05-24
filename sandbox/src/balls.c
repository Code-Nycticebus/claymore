#include "claymore/entrypoint.h"

#include "scenes/fps.h"
#include <stdio.h>

const float damping = 0.999;
const float gravity = 1000;

const float max_balls = 2000.f;
const float max_speed = 1.f;

const float r = 350;
static vec2 center;

typedef struct {
  vec2 position;
  vec2 last_position;
  float radius;
  vec4 color;
} Ball;

typedef DA(Ball) Balls;

typedef struct {
  CmCamera2D camera;
  CmFont *font;
  Balls balls;
  vec2 cursor;
} BallSimulation;

static void init(CmScene *scene) {
  BallSimulation *balls = cm_scene_alloc_data(scene, sizeof(BallSimulation));
  da_init(&balls->balls, &scene->arena);

  balls->font =
      cm_font_init(&scene->gpu, STR("assets/fonts/Ubuntu.ttf"), 20, ErrPanic);

  cm_scene_push(scene, fps);
  cm_camera2D_screen(&balls->camera);

  RGFW_window *window = cm_app_window();
  glm_vec2_divs((vec2){window->r.w, window->r.h}, 2, center);
}

static void verlet_integration(Ball *ball, double deltatime) {
  // Gravity
  vec2 velocity = {0, gravity};
  glm_vec2_scale(velocity, deltatime * deltatime, velocity);

  // Calculate ball movement
  vec2 displacement;
  glm_vec2_sub(ball->position, ball->last_position, displacement);
  // Optional min/max:
  glm_vec2_clamp(displacement, -max_speed, +max_speed);
  // add displacement to gravity
  glm_vec2_add(velocity, displacement, velocity);
  // Save current position
  glm_vec2_copy(ball->position, ball->last_position);
  // add to postion
  glm_vec2_add(ball->position, velocity, ball->position);
}

static void bounds(Ball *ball) {
  vec2 diff;
  glm_vec2_sub(ball->position, center, diff);
  const float distance = glm_vec2_distance(ball->position, center);
  if (r - ball->radius < distance) {
    vec2 normal;
    glm_vec2_divs(diff, distance, normal);
    glm_vec2_scale(normal, r - ball->radius, normal);
    glm_vec2_add(center, normal, ball->position);
  }
}

static void collision(Ball *ball, Balls *balls) {
  for (size_t i = 0; i < da_len(balls); ++i) {
    Ball *ball2 = &da_get(balls, i);

    // Get distance between balls
    const float distance = glm_vec2_distance(ball->position, ball2->position);
    const float min_distance = ball->radius + ball2->radius;
    if (distance < min_distance) {
      // Get Normalized direction
      vec2 dir;
      glm_vec2_sub(ball->position, ball2->position, dir);
      glm_vec2_normalize(dir);

      // Get half of the overlapping distance
      glm_vec2_scale(dir, (distance - min_distance) / 2, dir);

      // Push balls into direction
      glm_vec2_sub(ball->position, dir, ball->position);
      glm_vec2_add(ball2->position, dir, ball2->position);
    }
  }
}

static void physics(BallSimulation *balls, double dt) {
  for (size_t i = 0; i < da_len(&balls->balls); ++i) {
    Ball *ball = &da_get(&balls->balls, i);

    verlet_integration(ball, dt);
    bounds(ball);
    collision(ball, &balls->balls);
  }
}

static void update(CmScene *scene, double dt) {
  BallSimulation *balls = scene->data;

  if (da_len(&balls->balls) < max_balls) {
    static float timer = 0.0f;
    static float countdown = 0.0f;
    countdown -= dt;
    timer += dt;
    if (countdown < 0) {
      countdown = 0.3f;

      vec2 pos = {center[0], center[1] - r * 0.9f};
      vec4 red = {.8, 0, 0.7, 1};
      vec4 green = {0, 0.8, 0.7, 1};
      vec4 color;
      glm_vec4_lerp(red, green, da_len(&balls->balls) / max_balls, color);
      da_push(&balls->balls,
              (Ball){
                  .position = {VEC2_ARG(pos)},
                  .last_position = {pos[0] + sinf(timer) * 0.5, pos[1]},
                  .radius = 10,
                  .color = {VEC4_ARG(color)},
              });
    }
  }

  const usize sub_steps = 4;
  for (usize i = 0; i < sub_steps; ++i) {
    physics(balls, dt / (double)sub_steps);
  }

  cm_renderer2D_begin(&balls->camera);
  cm_circle(center, (vec2){r, r}, (vec4){0.1, 0.1, 0.1, 1.0});
  cm_renderer2D_end();

  cm_renderer2D_begin(&balls->camera);
  for (size_t i = 0; i < da_len(&balls->balls); i++) {
    Ball *ball = &da_get(&balls->balls, i);
    cm_circle(ball->position, (vec2){ball->radius, ball->radius}, ball->color);
  }

#define N 20
  char buffer[N];
  usize size =
      snprintf(buffer, N, "%" USIZE_FMT " Balls", da_len(&balls->balls));
  Str s = str_from_parts(size, buffer);
  cm_font(balls->font, (vec2){10, 50}, s);

  cm_renderer2D_end();
}

static void event(CmScene *scene, CmEvent *event) {
  BallSimulation *balls = scene->data;
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
      .window = {.width = 1280, .height = 720, .title = "balls"},
      .main = balls,
  };
  return &config;
}
