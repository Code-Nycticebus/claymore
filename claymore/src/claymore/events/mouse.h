#ifndef __CM_MOUSE_H__
#define __CM_MOUSE_H__

#include <stdint.h>

#include "event_types.h"

CmMouseInfo cm_mouseinfo(void);

CmMouseAction cm_mouseinfo_button(CmMouseKeycode button);

void cm_mouseinfo_pos(vec2 pos);
void cm_mouseinfo_set_pos(vec2 pos);
void cm_mouseinfo_set_button(CmMouseAction action, CmMouseKeycode button);
#endif /* !__CM_MOUSE_H__ */
