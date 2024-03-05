#ifndef __CLAYMORE_WINDOW_H__
#define __CLAYMORE_WINDOW_H__

#include "claymore/defines.h" // IWYU pragma: export

void *cm_window_context(void);
double cm_window_time(void);
void cm_window_set_bg(const vec3 color);
void cm_window_get_size(vec2 out);
void cm_window_close(bool close);

bool cm_window_internal_create(usize width, usize heigth, const char *title);
void cm_window_internal_close(void);

bool cm_window_internal_should_close(void);

void cm_window_internal_swap_buffers(void);
void cm_window_internal_poll_events(void);

#endif /* !__CLAYMORE_WINDOW_H__ */
