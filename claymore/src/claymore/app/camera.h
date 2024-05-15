#ifndef __CLAYMORE_CAMERA_H__
#define __CLAYMORE_CAMERA_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef struct {
  bool dirty;
  mat4 view;
  mat4 projection;
  mat4 vp;
} CmCameraBase;

void _cm_camera_update(CmCameraBase *camera);
#define cm_camera_update(camera) _cm_camera_update(&(camera)->base)
#define cm_camera_vp(camera) ((camera)->base.vp)

typedef struct {
  CmCameraBase base;
  vec2 pos;
  float zoom;
} CmCamera2D;

void cm_camera2d_screen(CmCamera2D *camera);
void cm_camera2d_ortho(CmCamera2D *camera);

typedef struct {
  bool dirty;
  mat4 view;
  mat4 projection;
  mat4 vp;
} CmCamera3D;

#endif /* __CLAYMORE_CAMERA_H__ */
