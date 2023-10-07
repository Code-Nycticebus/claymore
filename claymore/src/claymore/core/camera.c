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

CmCamera cm_camera_init_ortho(vec3s position, float aspect, float zoom) {
  CmCamera camera = {0};
  camera.projection =
      glms_ortho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1.F, 100.F);
  camera.position = position;
  camera.view = glms_translate(glms_mat4_identity(),
                               glms_vec3_scale(camera.position, -1));
  camera.aspect = aspect;
  camera.zoom = zoom;
  camera.update = true;
  cm_camera_update(&camera);
  return camera;
}

CmCamera cm_camera_init_screen(vec3s position, float right, float top) {
  CmCamera camera = {0};
  camera.projection = glms_ortho(0.F, right, 0.F, top, -1.F, 100.F);
  camera.position = position;
  camera.view = glms_translate(glms_mat4_identity(),
                               glms_vec3_scale(camera.position, -1));
  camera.update = true;
  cm_camera_update(&camera);
  return camera;
}

void cm_camera_translate(CmCamera *camera, vec3s position) {
  camera->position = glms_vec3_add(camera->position, position);
  camera->view = glms_translate(glms_mat4_identity(),
                                glms_vec3_scale(camera->position, -1));
  camera->update = true;
}

void cm_camera_zoom(CmCamera *camera, float zoom) {
  assert(camera->aspect);
  camera->zoom = zoom;
  camera->projection = glms_ortho(-camera->aspect * zoom, camera->aspect * zoom,
                                  -zoom, zoom, -1.F, 1.F);
  camera->update = true;
}

void cm_camera_aspect(CmCamera *camera, float aspect) {
  assert(camera->aspect && 0 < camera->zoom);
  camera->aspect = aspect;
  camera->projection = glms_ortho(-aspect * camera->zoom, aspect * camera->zoom,
                                  -camera->zoom, camera->zoom, -1.F, 1.F);
  camera->update = true;
}
void cm_camera_set_screen(CmCamera *camera, float right, float top) {
  camera->projection = glms_ortho(0.F, right, 0.F, top, -1.F, 100.F);
  camera->update = true;
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
