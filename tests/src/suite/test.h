#ifndef __TEST_H__
#define __TEST_H__

#include "claymore.h"

typedef enum {
  TESTS_FAILED,
  TESTS_ERROR,
} TestsExitCodes;

CmScene *test_init(CmScene *parent, Error *error, CmSceneInit scene);

#endif /* __TEST_H__ */
