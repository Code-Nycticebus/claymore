#include "camera.h"

CmCamera cm_camera_init_perspective(vec3s position, vec3s lookat, float fov,
                                    float aspect_ratio) {
  CmCamera camera = (CmCamera){
      .update = true,
      .up = {{0, 1, 0}},
  };

  camera.position = position;
  camera.projection =
      glms_perspective(glm_rad(fov), aspect_ratio, 1 / 100.F, 100.F);
  camera.view = glms_lookat(camera.position, lookat, camera.up);
  cm_camera_update(&camera);
  return camera;
}

void cm_camera_position(CmCamera *camera, vec3s position) {
  camera->position = position;
  camera->view = glms_lookat(position, camera->lookat, camera->up);
  camera->update = true;
}

void cm_camera_lookat(CmCamera *camera, vec3s lookat) {
  camera->lookat = lookat;
  camera->view = glms_lookat(camera->position, lookat, camera->up);
  camera->update = true;
}

void cm_camera_update(CmCamera *camera) {
  if (camera->update) {
    camera->vp = glms_mul(camera->projection, camera->view);
    camera->update = false;
  }
}
