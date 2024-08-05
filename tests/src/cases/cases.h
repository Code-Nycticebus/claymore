#ifndef __TESTS_H__
#define __TESTS_H__

#include "claymore.h"

#define WIDHT 80
#define HEIGHT 80

CmSceneInterface *framebuffer(void);
CmSceneInterface *renderer2D(void);

static const CmSceneInit test_cases[] = {
    framebuffer,
    renderer2D,
};

#endif /* __TESTS_H__ */
