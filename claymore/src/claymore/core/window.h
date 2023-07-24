#ifndef __CM_WINDOW_H__
#define __CM_WINDOW_H__

#include "cm.h"

typedef struct {
  void *ctx;
  uint32_t height, width;
  const char *title;
} CMwindow;

CMwindow *cm_window_init(uint32_t width, uint32_t height, const char *name);
void cm_window_update(const CMwindow *window);

void cm_window_close(CMwindow *window);
void cm_window_shutdown(void);

float cm_window_time(void);

#endif /* !__CM_WINDOW_H__ */
