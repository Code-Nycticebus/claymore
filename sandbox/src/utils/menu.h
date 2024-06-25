#ifndef __SANDBOX_MENU_H__
#define __SANDBOX_MENU_H__

#include "claymore.h"

typedef void (*Update)(CmScene *menu);

CmScene *menu_init(CmScene *parent, vec2 anchor, float width, Update update);

bool button(CmScene *scene, Str label);

#endif /* !__SANDBOX_MENU_H__ */
