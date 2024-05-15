#include "camera.h"

#include "window.h"

void _cm_camera_update(CmCameraBase *camera) {
  if (camera->dirty == true) {
    camera->dirty = false;
    glm_mat4_mul(camera->projection, camera->view, camera->vp);
  }
}

void cm_camera2d_screen(CmCamera2D *camera) {
  vec2 window_size;
  cm_window_get_size(window_size);
  glm_ortho(0, window_size[0], window_size[1], 0, -1.F, 100.F,
            camera->base.projection);

  glm_mat4_identity(camera->base.view);
  glm_translate(camera->base.view, (vec3){0});

  camera->base.dirty = true;
  cm_camera_update(camera);
}
