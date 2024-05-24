#include "camera.h"

#include "app.h"

void _cm_camera_update(CmCameraBase *camera) {
  if (camera->dirty == true) {
    camera->dirty = false;
    glm_mat4_mul(camera->projection, camera->view, camera->vp);
  }
}

void cm_camera2D_screen(CmCamera2D *camera) {
  RGFW_window *window = cm_app_window();
  glm_ortho(0, window->r.w, window->r.h, 0, -1.F, 100.F,
            camera->base.projection);

  glm_mat4_identity(camera->base.view);
  glm_translate(camera->base.view, (vec3){0});

  camera->base.dirty = true;
  cm_camera_update(camera);
}
