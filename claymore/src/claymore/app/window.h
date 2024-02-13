#ifndef __CLAYMORE_WINDOW_H__
#define __CLAYMORE_WINDOW_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef struct {
  void *context;
} CmWindow;

CmWindow cm_window_create(usize width, usize heigth, const char *title);

#endif /* !__CLAYMORE_WINDOW_H__ */
