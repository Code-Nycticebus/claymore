#ifndef __TESTS_H__
#define __TESTS_H__

#include "claymore.h"

#define WIDHT 80
#define HEIGHT 80

CmSceneInterface *renderer2D(void);
CmSceneInterface *framebuffer(void);

static const CmSceneInit test_suite[] = {
    renderer2D,
    framebuffer,
};

#endif /* __TESTS_H__ */
