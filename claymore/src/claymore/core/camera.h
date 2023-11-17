#ifndef __CM_CAMERA_H__
#define __CM_CAMERA_H__

#include "cm.h"

typedef struct {
  vec3s position;
  vec3s lookat;
  vec3s up;
  float fov;
  float aspect;
  float zoom;

  mat4s view;
  mat4s projection;
  mat4s vp;

  bool update;
} CmCamera;

CmCamera cm_camera_perspective_init(vec3s position, vec3s lookat, float fov,
                                    float aspect_ratio);
CmCamera cm_camera_ortho_init(vec3s position, float aspect, float zoom);
CmCamera cm_camera_screen_init(vec3s position, float right, float top);

void cm_camera_zoom(CmCamera *camera, float zoom);
void cm_camera_ortho_aspect(CmCamera *camera, float aspect);
void cm_camera_perspective_aspect(CmCamera *camera, float aspect);
void cm_camera_set_screen(CmCamera *camera, float right, float top);

void cm_camera_translate(CmCamera *camera, vec3s position);
void cm_camera_position(CmCamera *camera, vec3s position);
void cm_camera_lookat(CmCamera *camera, vec3s lookat);

void cm_camera_update(CmCamera *camera);

#endif /* !__CM_CAMERA_H__ */
