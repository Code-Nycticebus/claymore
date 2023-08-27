#ifndef __CM_SOUND_H__
#define __CM_SOUND_H__

#include "cm.h"

bool cm_sound_init(void);
void cm_sound_shutdown(void);

void cm_sound_play(const char *filename);

#endif /* !__CM_SOUND_H__ */
