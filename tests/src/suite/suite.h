#ifndef __SUITE_H__
#define __SUITE_H__

#include "claymore.h"

typedef enum {
  TESTS_FAILED,
  TESTS_ERROR,
} TestsExitCodes;

CmScene *test_init(CmScene *parent, u32 width, u32 height, Error *error, CmSceneInit scene);

CmScene *suite(CmScene *parent, u32 width, u32 height, u32 test_count, const CmSceneInit *tests);

#endif /* ifndef __SUITE_H__ */
