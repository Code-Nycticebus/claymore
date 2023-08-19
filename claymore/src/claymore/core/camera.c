#include "camera.h"

CmCamera cm_camera_init_perspective(vec3 position, vec3 lookat, float fov,
                                    float aspect_ratio) {
  CmCamera camera = (CmCamera){
      .update = true,
      .up = {0, 1, 0},
  };

  glm_vec3_copy(position, camera.position);
  glm_perspective(glm_rad(fov), aspect_ratio, 1 / 100.F, 100.F,
                  camera.projection);
  glm_lookat(camera.position, lookat, camera.up, camera.view);
  cm_camera_update(&camera);
  return camera;
}

void cm_camera_position(CmCamera *camera, vec3 position) {
  glm_vec3_copy(position, camera->position);
  glm_lookat(position, camera->lookat, camera->up, camera->view);
  camera->update = true;
}

void cm_camera_lookat(CmCamera *camera, vec3 lookat) {
  glm_vec3_copy(lookat, camera->lookat);
  glm_lookat(camera->position, lookat, camera->up, camera->view);
  camera->update = true;
}

void cm_camera_update(CmCamera *camera) {
  if (camera->update) {
    glm_mul(camera->projection, camera->view, camera->vp);
    camera->update = false;
  }
}
