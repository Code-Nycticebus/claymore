#ifndef __CLAYMORE_CAMERA_H__
#define __CLAYMORE_CAMERA_H__

#include "claymore/defines.h" // IWYU pragma: export

// Base camera struct
typedef struct {
  bool dirty;      // If view projection matrix needs to be updated
  vec3 position;   // Camera position
  mat4 view;       // View matrix
  mat4 projection; // Projection matrix
  mat4 vp;         // Projection * View
} CmCameraBase;

// Base camera methods
void _cm_camera_update(CmCameraBase *camera);
#define cm_camera_update(camera) _cm_camera_update(&(camera)->base)

// Get camera view projection matrix
#define cm_camera_vp(camera) ((camera)->base.vp)

// 2D camera struct
typedef struct {
  CmCameraBase base; // Camera base
  float zoom;        // Current zoom
} CmCamera2D;

// Initalizes camera bound to screen dimensions (UI, ...)
void cm_camera2D_screen(CmCamera2D *camera);
// Initalizes an orthogonal camera view
void cm_camera2D_ortho(CmCamera2D *camera, vec2 pos, float aspect, float zoom);

// 3D camera struct
typedef struct {
  CmCameraBase base; // Inherits the base camera properties
  vec3 lookat;       // Point where camera is looking at
  vec3 up;           // Up direction of the camera
  float fov;         // Field of view
  float aspect;      // Aspect ratio
} CmCamera3D;

// Initalizes 3D perspective camera
void cm_camera3D_perspective(CmCamera3D *camera, vec3 position, float fov,
                             vec3 lookat, float aspect);
// Changes camera position
void cm_camera3D_position(CmCamera3D *camera, vec3 position);

#endif /* __CLAYMORE_CAMERA_H__ */
