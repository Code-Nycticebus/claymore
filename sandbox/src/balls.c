#include "claymore/entrypoint.h"

#include "scenes/fps.h"
#include <stdio.h>

const float gravity = 981;

const float max_balls = 2500;

#define WINDOW_SIZE_Y 1000
#define WINDOW_SIZE_X 1500

#define BALL_SIZE 5
#define GRID_SIZE_X (WINDOW_SIZE_X / (BALL_SIZE * 2))
#define GRID_SIZE_Y (WINDOW_SIZE_Y / (BALL_SIZE * 2))
#define CELL_SIZE_X (WINDOW_SIZE_X / GRID_SIZE_X)
#define CELL_SIZE_Y (WINDOW_SIZE_Y / GRID_SIZE_Y)

static vec2 center;

static Str font = STR_STATIC("assets/fonts/Ubuntu.ttf");
static const float font_size = 20;

typedef struct {
  vec2 position;
  vec2 last_position;
  float radius;
  vec4 color;
} Ball;

typedef DA(Ball) Balls;
typedef DA(Ball *) Cell;

Cell grid[GRID_SIZE_X][GRID_SIZE_Y];

void grid_init(Arena *arena) {
  for (usize x = 0; x < GRID_SIZE_X; ++x) {
    for (usize y = 0; y < GRID_SIZE_Y; ++y) {
      da_init(&grid[x][y], arena);
    }
  }
}

void grid_clear(void) {
  for (usize x = 0; x < GRID_SIZE_X; ++x) {
    for (usize y = 0; y < GRID_SIZE_Y; ++y) {
      da_clear(&grid[x][y]);
    }
  }
}

typedef struct {
  CmCamera2D camera;
  CmFont *font;
  Balls balls;
  vec2 cursor;
} BallSimulation;

static void init(CmScene *scene) {
  BallSimulation *balls = cm_scene_set_data(scene, sizeof(BallSimulation));
  da_init(&balls->balls, &scene->arena);
  da_reserve(&balls->balls, max_balls);

  balls->font = cm_font_init(&scene->gpu, font, font_size, ErrPanic);

  const vec2 pos = {10, 0};
  fps(scene, pos, font, font_size);

  cm_camera2D_screen(&balls->camera);

  RGFW_window *window = cm_app_window();
  glm_vec2_divs((vec2){window->r.w, window->r.h}, 2, center);

  grid_init(&scene->arena);
}

static void bounds(Ball *ball) {
  float x1 = CELL_SIZE_X + BALL_SIZE * 2;
  float x2 = WINDOW_SIZE_X - CELL_SIZE_X - BALL_SIZE;

  float y1 = CELL_SIZE_Y + BALL_SIZE * 2;
  float y2 = WINDOW_SIZE_Y - CELL_SIZE_Y - BALL_SIZE;

  ball->position[0] = glm_clamp(ball->position[0], x1, x2);
  ball->position[1] = glm_clamp(ball->position[1], y1, y2);
}

static void verlet_integration(Ball *ball, double deltatime) {
  // Gravity
  vec2 velocity = {0, gravity};
  glm_vec2_scale(velocity, deltatime * deltatime, velocity);

  // Calculate ball movement
  vec2 displacement;
  glm_vec2_sub(ball->position, ball->last_position, displacement);
  // add displacement to gravity
  glm_vec2_add(velocity, displacement, velocity);
  // Save current position
  glm_vec2_copy(ball->position, ball->last_position);
  // add to postion
  glm_vec2_add(ball->position, velocity, ball->position);
}

static void collision(Ball *b1, Ball *b2) {
  // Get distance between balls
  const float distance = glm_vec2_distance(b1->position, b2->position);
  const float min_distance = b1->radius + b2->radius;
  if (distance < min_distance) {
    // Get Normalized direction
    vec2 dir;
    glm_vec2_sub(b1->position, b2->position, dir);
    glm_vec2_normalize(dir);

    // Get half of the overlapping distance
    glm_vec2_scale(dir, (distance - min_distance) / 2, dir);

    // Push balls into direction
    glm_vec2_sub(b1->position, dir, b1->position);
    glm_vec2_add(b2->position, dir, b2->position);
  }
}

void cell_resolve(Cell *c1, Cell *c2) {
  for (usize i = 0; i < da_len(c1); ++i) {
    Ball *b1 = da_get(c1, i);
    for (usize j = 0; j < da_len(c2); ++j) {
      Ball *b2 = da_get(c2, j);
      if (b1 != b2) {
        collision(b1, b2);
      }
    }
  }
}

void grid_resolve(void) {
  for (i32 x = 1; x < GRID_SIZE_X - 1; ++x) {
    for (i32 y = 1; y < GRID_SIZE_Y - 1; ++y) {
      Cell *current_cell = &grid[x][y];
      for (i32 x1 = -1; x1 <= 1; ++x1) {
        for (i32 y1 = -1; y1 <= 1; ++y1) {
          Cell *other_cell = &grid[x + x1][y + y1];
          cell_resolve(current_cell, other_cell);
        }
      }
    }
  }
}

static void physics(BallSimulation *balls, double dt) {
  grid_clear();

  for (size_t i = 0; i < da_len(&balls->balls); ++i) {
    Ball *ball = &da_get(&balls->balls, i);
    verlet_integration(ball, dt);
    bounds(ball);
    int x = ball->position[0] / CELL_SIZE_X;
    int y = ball->position[1] / CELL_SIZE_Y;
    da_push(&grid[x][y], ball);
  }

  grid_resolve();
}

static void fixed_update(CmScene *scene, double dt) {
  BallSimulation *balls = scene->data;

  const usize sub_steps = 8;
  for (usize i = 0; i < sub_steps; ++i) {
    physics(balls, dt / (double)sub_steps);
  }
}

static void frame_update(CmScene *scene, double dt) {
  BallSimulation *balls = scene->data;
  static bool generating = true;

  if (generating) {
    static float countdown = 0.0f;
    countdown -= dt;
    if (countdown < 0) {
      countdown = 0.1f;
      if (1 / 60.f < dt) {
        generating = false;
      }

      for (int i = -4; i <= 4; ++i) {
        vec2 pos = {100 + (BALL_SIZE * 2) * i, 100 - (BALL_SIZE * 2) * i};
        vec4 red = {.8, 0, 0.7, 1};
        vec4 green = {0, 0.8, 0.7, 1};
        vec4 color;
        const float t = (da_len(&balls->balls) % 200) / 200.f;
        glm_vec4_lerp(red, green, t, color);
        da_push(&balls->balls, (Ball){
                                   .position = {VEC2_ARG(pos)},
                                   .last_position = {pos[0] - 2.5, pos[1]},
                                   .radius = BALL_SIZE,
                                   .color = {VEC4_ARG(color)},
                               });
      }
    }
  }

  cm_2D_begin(&balls->camera);
  {
    for (size_t i = 0; i < da_len(&balls->balls); i++) {
      Ball *ball = &da_get(&balls->balls, i);
      cm_circle(ball->position, (vec2){ball->radius, ball->radius},
                ball->color);
    }

#define N 20
    char buffer[N];
    usize size =
        snprintf(buffer, N, "%" USIZE_FMT " Balls", da_len(&balls->balls));
    Str s = str_from_parts(size, buffer);
    cm_font(balls->font, (vec2){10, font_size}, s);
  }
  cm_2D_end();
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
      .frame_update = frame_update,
      .fixed_update = fixed_update,
      .event = event,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window =
          {
              .width = WINDOW_SIZE_X,
              .height = WINDOW_SIZE_Y,
              .title = "balls",
          },
      .main = balls,
  };
  return &config;
}
