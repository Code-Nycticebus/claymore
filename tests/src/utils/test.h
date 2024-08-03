#ifndef __TEST_H__
#define __TEST_H__

#include "claymore.h"

typedef enum {
  TESTS_OK = 1,
  TESTS_ERR,
} TestsExitCodes;

CmScene *test_init(CmScene *parent, CmSceneInit scene);

#endif /* __TEST_H__ */
