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

void cm_camera2D_ortho(CmCamera2D *camera, vec3 pos, float aspect, float zoom) {
  glm_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F,
            camera->base.projection);

  glm_mat4_identity(camera->base.view);
  glm_translate(camera->base.view, pos);

  glm_vec3_copy(pos, camera->position);

  camera->base.dirty = true;
  cm_camera_update(camera);
}

void cm_camera3D_perspective(CmCamera3D *camera, vec3 position, float fov,
                             vec3 lookat, float aspect) {
  glm_vec3_copy(position, camera->position);
  glm_vec3_copy((vec3){0, 1, 0}, camera->up);
  glm_vec3_copy(lookat, camera->lookat);

  const float near = 0.1f;
  const float far = 1000.f;
  glm_perspective(glm_rad(fov), aspect, near, far, camera->base.projection);
  glm_lookat(position, lookat, camera->up, camera->base.view);

  camera->fov = fov;

  camera->base.dirty = true;
  cm_camera_update(camera);
}

void cm_camera3D_position(CmCamera3D *camera, vec3 position) {
  glm_lookat(position, camera->lookat, camera->up, camera->base.view);
  glm_vec3_copy(position, camera->position);
  camera->base.dirty = true;
}
