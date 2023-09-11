#ifndef __CM_MOUSE_H__
#define __CM_MOUSE_H__

#include <stdint.h>

#include "event_types.h"

CmMouseInfo cm_mouseinfo(void);

CmMouseAction cm_mouseinfo_button(CmMouseKeycode button);
vec2s cm_mouseinfo_pos(void);

// TODO set private
void cm_mouseinfo_set_pos(vec2s pos);
void cm_mouseinfo_set_button(CmMouseAction action, CmMouseKeycode button);
#endif /* !__CM_MOUSE_H__ */
