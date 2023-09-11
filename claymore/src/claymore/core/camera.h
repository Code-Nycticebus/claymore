#ifndef __CM_CAMERA_H__
#define __CM_CAMERA_H__

#include "cm.h"

typedef struct {
  vec3s position;
  vec3s lookat;
  vec3s up;
  float fov;
  float aspect_ratio;

  mat4s view;
  mat4s projection;
  mat4s vp;

  bool update;
} CmCamera;

CmCamera cm_camera_init_perspective(vec3s position, vec3s lookat, float fov,
                                    float aspect_ratio);

void cm_camera_position(CmCamera *camera, vec3s position);
void cm_camera_lookat(CmCamera *camera, vec3s lookat);

void cm_camera_update(CmCamera *camera);

#endif /* !__CM_CAMERA_H__ */
