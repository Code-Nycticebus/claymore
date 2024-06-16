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
  glm_ortho(0, window->r.w, window->r.h, 0, 1.f / 100.f, 100.f,
            camera->base.projection);

  glm_mat4_identity(camera->base.view);
  glm_translate(camera->base.view, (vec3){0, 0, -1.f});

  camera->base.dirty = true;
  cm_camera_update(camera);
}

void cm_camera2D_ortho(CmCamera2D *camera, vec2 pos, float aspect, float zoom) {
  glm_ortho(-aspect * zoom, aspect * zoom, zoom, -zoom, 1.f / 100.f, 100.f,
            camera->base.projection);

  vec3 position = {pos[0], pos[1], -1.f};

  glm_mat4_identity(camera->base.view);
  glm_translate(camera->base.view, position);

  glm_vec3_copy(position, camera->base.position);

  camera->base.dirty = true;
  cm_camera_update(camera);
}

void cm_camera3D_perspective(CmCamera3D *camera, vec3 position, float fov,
                             vec3 lookat, float aspect) {
  glm_vec3_copy(position, camera->base.position);
  glm_vec3_copy((vec3){0, 1, 0}, camera->up);
  glm_vec3_copy(lookat, camera->lookat);

  glm_perspective(glm_rad(fov), aspect, 1.f / 100.f, 100.f, camera->base.projection);
  glm_lookat(position, lookat, camera->up, camera->base.view);

  camera->fov = fov;

  camera->base.dirty = true;
  cm_camera_update(camera);
}

void cm_camera3D_position(CmCamera3D *camera, vec3 position) {
  glm_lookat(position, camera->lookat, camera->up, camera->base.view);
  glm_vec3_copy(position, camera->base.position);
  camera->base.dirty = true;
}
