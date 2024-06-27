#ifndef __SANDBOX_GUI_H__
#define __SANDBOX_GUI_H__

#include "claymore.h"

typedef void (*Update)(CmScene *gui);

CmScene *gui_init(CmScene *parent, vec2 anchor, float width, Update update);

bool button(CmScene *scene, Str label);

#endif /* !__SANDBOX_GUI_H__ */
