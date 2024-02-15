#ifndef __CLAYMORE_WINDOW_H__
#define __CLAYMORE_WINDOW_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef struct {
  void *context;
} CmWindow;

bool cm_window_create(usize width, usize heigth, const char *title);
void cm_window_close(void);

void *cm_window_context(void);
double cm_window_time(void);

void cm_window_set_bg_color(vec3 color);

bool cm_window_should_close(void);

void cm_window_swap_buffers(void);
void cm_window_poll_events(void);

#endif /* !__CLAYMORE_WINDOW_H__ */
