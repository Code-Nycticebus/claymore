#include "claymore.h"
#include <stdio.h>

extern int claymore_main(void);

void cm_init(void) { printf("Init\n"); }
void cm_free(void) { printf("Free\n"); }
void cm_update(void) { printf("Update\n"); }

int main(void) {
  claymore_main();
  return 0;
}
