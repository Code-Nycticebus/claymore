#include "models.h"

static void camera_controll(CmMouseEvent *event, CmCamera *camera) {
  if (event->action == CM_MOUSE_MOVE) {
    vec2s dir = cm_mouseinfo_direction();
    glms_vec2_normalize(dir);
    dir = glms_vec2_scale(dir, (float)3);

    if (cm_mouseinfo_button(CM_MOUSE_BUTTON_LEFT)) {
      float camera_distance =
          glms_vec3_distance(camera->position, camera->lookat);

      static float rotation_horizontal = 0.F;
      static float rotation_vertical = 0.F;

      rotation_horizontal += glm_rad(-dir.x);
      rotation_vertical += glm_rad(-dir.y);

      const float limit = glm_rad(89.F);
      rotation_vertical = glm_clamp(rotation_vertical, -limit, limit);

      vec3s new_camera_pos;
      new_camera_pos.x = camera->lookat.x + camera_distance *
                                                sinf(rotation_horizontal) *
                                                cosf(rotation_vertical);
      new_camera_pos.y =
          camera->lookat.y + camera_distance * sinf(rotation_vertical);
      new_camera_pos.z = camera->lookat.z + camera_distance *
                                                cosf(rotation_horizontal) *
                                                cosf(rotation_vertical);

      cm_camera_position(camera, new_camera_pos);
    }
  }
}

static void camera_scroll(CmScrollEvent *event, CmCamera *camera) {
  const float min_distance = 1.F;
  const float max_distance = 1000.F;

  vec3s direction;
  vec3s center = {{0, 0, 0}};
  float distance = glms_vec3_distance(center, camera->position);
  direction = glms_vec3_sub(center, camera->position);
  direction = glms_vec3_normalize(direction);
  direction = glms_vec3_scale(direction, (float)event->yoffset * distance / 4);

  vec3s new_camera_pos = glms_vec3_add(camera->position, direction);
  float new_distance = glms_vec3_distance(new_camera_pos, center);
  if (min_distance <= new_distance && new_distance <= max_distance) {
    cm_camera_position(camera, new_camera_pos);
  }
}

static void camera_resize(CmWindowEvent *event, CmCamera *camera) {
  camera->projection = glms_perspective(glm_rad(camera->fov),
                                        (float)event->window->width /
                                            (float)event->window->height,
                                        1 / 100.F, 100.F);
  camera->update = true;
}

void camera_register_callbacks(CmCamera *camera) {
  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE, (cm_event_callback)camera_resize,
                     camera);

  cm_event_subscribe(CM_EVENT_MOUSE, (cm_event_callback)camera_controll,
                     camera);
  cm_event_subscribe(CM_EVENT_SCROLL, (cm_event_callback)camera_scroll, camera);
  cm_event_subscribe(CM_EVENT_WINDOW_RESIZE, (cm_event_callback)camera_resize,
                     camera);
}
