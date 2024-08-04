#ifndef __TEST_H__
#define __TEST_H__

#include "claymore.h"

typedef enum {
  TESTS_FAILED,
  TESTS_ERROR,
} TestsExitCodes;

CmScene *test_init(CmScene *parent, u32 width, u32 height, Error *error, CmSceneInit scene);

#endif /* __TEST_H__ */
