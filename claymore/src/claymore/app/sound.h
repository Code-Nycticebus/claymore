#ifndef __CLAYMORE_SOUND_H__
#define __CLAYMORE_SOUND_H__

#include "claymore/defines.h" // IWYU pragma: export

void cm_sound_play(const char *filename);

bool cm_sound_internal_init(void);
void cm_sound_interal_shutdown(void);

#endif /* !__CLAYMORE_SOUND_H__ */
