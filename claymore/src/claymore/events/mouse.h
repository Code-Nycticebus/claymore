#ifndef __CM_MOUSE_H__
#define __CM_MOUSE_H__

#include <stdint.h>

#include "cm.h"
#include "key.h"

typedef enum {
  CM_MOUSE_BUTTON_1,
  CM_MOUSE_BUTTON_2,
  CM_MOUSE_BUTTON_3,
  CM_MOUSE_BUTTON_4,
  CM_MOUSE_BUTTON_5,
  CM_MOUSE_BUTTON_6,
  CM_MOUSE_BUTTON_7,
  CM_MOUSE_BUTTON_8,
  CM_MOUSE_BUTTON_COUNT,
  CM_MOUSE_BUTTON_LEFT = CM_MOUSE_BUTTON_1,
  CM_MOUSE_BUTTON_RIGHT = CM_MOUSE_BUTTON_2,
  CM_MOUSE_BUTTON_MIDDLE = CM_MOUSE_BUTTON_3,
} CmMouseKeycode;

typedef struct {
  float x;
  float y;
  CmKeyAction action;
  CmMouseKeycode button;
} CmMouseInfo;

typedef enum {
  CM_MOUSE_CLICK,
  CM_MOUSE_MOVE,
  CM_MOUSE_SCROLL,
} CmMouseAction;

typedef struct {
  CmMouseAction action;
  CmMouseInfo info;
} CmMouseEvent;

CmMouseInfo cm_mouseinfo(void);
void cm_mouseinfo_pos(vec2 pos);
void cm_mouseinfo_set_pos(float x, float y);
void cm_mouseinfo_set_button(CmKeyAction action, CmMouseKeycode button);
#endif /* !__CM_MOUSE_H__ */
