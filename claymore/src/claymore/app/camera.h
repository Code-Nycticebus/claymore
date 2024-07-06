#ifndef __CLAYMORE_CAMERA_H__
#define __CLAYMORE_CAMERA_H__

#include "claymore/defines.h" // IWYU pragma: export

/* ========= camera base ========= */

/* Base camera (CmCameraBase):
 *  - Base camera struct that every camera needs.
 * */
typedef struct {
  bool dirty;      // if view projection matrix needs to be updated
  vec3 position;   // camera position
  mat4 view;       // view matrix
  mat4 projection; // projection matrix
  mat4 vp;         // projection * view
} CmCameraBase;

// base camera methods
void _cm_camera_update(CmCameraBase *camera);
#define cm_camera_update(camera) _cm_camera_update(&(camera)->base)

// get camera view projection matrix
#define cm_camera_vp(camera) ((camera)->base.vp)

/* ========= camera 2D ========= */

typedef struct {
  CmCameraBase base; // inherits the base camera properties
  float aspect;      // aspect ratio
  float zoom;        // current zoom
} CmCamera2D;

// initalizes camera bound to screen dimensions (UI, ...)
void cm_camera2D_screen(CmCamera2D *camera);
// initalizes an orthogonal camera view
void cm_camera2D_ortho(CmCamera2D *camera, vec2 pos, float aspect, float zoom);

// set position
void cm_camera2D_set_position(CmCamera2D *camera, vec2 position);
// move camera into the direction
void cm_camera2D_move(CmCamera2D *camera, vec2 direction);
// set zoom of orthogonal camera
void cm_camera2D_set_zoom(CmCamera2D *camera, float zoom);
// set aspect of orthogonal camera
void cm_camera2D_set_aspect(CmCamera2D *camera, float aspect);

/* ========= camera 3D ========= */

typedef struct {
  CmCameraBase base; // inherits the base camera properties
  vec3 lookat;       // point where camera is looking at
  vec3 up;           // up direction of the camera
  float fov;         // field of view
  float aspect;      // aspect ratio
} CmCamera3D;

// initalizes 3D perspective camera
void cm_camera3D_perspective(CmCamera3D *camera, vec3 position, float fov,
                             vec3 lookat, float aspect);
// changes camera position
void cm_camera3D_position(CmCamera3D *camera, vec3 position);

#endif /* __CLAYMORE_CAMERA_H__ */
