#ifndef __CLAYMORE_CAMERA_H__
#define __CLAYMORE_CAMERA_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef struct {
  bool dirty;
  vec3 position;
  mat4 view;
  mat4 projection;
  mat4 vp;
} CmCameraBase;

void _cm_camera_update(CmCameraBase *camera);
#define cm_camera_update(camera) _cm_camera_update(&(camera)->base)
#define cm_camera_vp(camera) ((camera)->base.vp)

typedef struct {
  CmCameraBase base;
  float zoom;
} CmCamera2D;

void cm_camera2D_screen(CmCamera2D *camera);
void cm_camera2D_ortho(CmCamera2D *camera, vec2 pos, float aspect, float zoom);

typedef struct {
  CmCameraBase base;
  vec3 lookat;
  vec3 up;
  float fov;
  float aspect;
} CmCamera3D;

void cm_camera3D_perspective(CmCamera3D *camera, vec3 position, float fov,
                             vec3 lookat, float aspect);
void cm_camera3D_position(CmCamera3D *camera, vec3 position);

#endif /* __CLAYMORE_CAMERA_H__ */
