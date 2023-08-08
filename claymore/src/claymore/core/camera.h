#ifndef __CM_CAMERA_H__
#define __CM_CAMERA_H__

#include "cm.h"

typedef struct {
  vec3 position;
  vec3 lookat;
  vec3 up;
  float fov;
  float aspect_ratio;

  mat4 view;
  mat4 projection;
  mat4 vp;

  bool update;
} CmCamera;

CmCamera cm_camera_init_perspective(vec3 position, vec3 lookat, float fov,
                                    float aspect_ratio);

void cm_camera_position(CmCamera *camera, vec3 position);
void cm_camera_lookat(CmCamera *camera, vec3 lookat);

void cm_camera_update(CmCamera *camera);

#endif /* !__CM_CAMERA_H__ */
