#ifndef __CLAYMORE_RESSOURCE_H__
#define __CLAYMORE_RESSOURCE_H__

#include "claymore/defines.h" // IWYU pragma: export

typedef struct {
  HashMap *hm;
} CmRessource;

CmRessource cm_ressource_internal_init(Arena *arena);

void cm_ressource_add(CmRessource *r, Str key, void *data);
void cm_ressource_get(CmRessource *r, Str key, void *data);

#endif /* !__CLAYMORE_RESSOURCE_H__ */
