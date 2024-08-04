#ifndef __TESTS_H__
#define __TESTS_H__

#include "claymore.h"

#define WIDHT 80
#define HEIGHT 80

CmSceneInterface *circles(void);
CmSceneInterface *quads(void);

static const CmSceneInit test_suite[] = {
    circles,
    quads,
};

#endif /* __TESTS_H__ */
