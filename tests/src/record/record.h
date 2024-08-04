#ifndef __RECORD_H__
#define __RECORD_H__

#include "claymore.h"

CmScene *writer_push(CmScene *parent, u32 width, u32 height, CmSceneInit scene);
CmScene *display(CmScene *parent, u32 width, u32 height, u32 test_count, const CmSceneInit *tests);
CmScene *player_push(CmScene *parent, u32 width, u32 height, CmSceneInit test_scene);

#endif /* !__RECORD_H__ */
