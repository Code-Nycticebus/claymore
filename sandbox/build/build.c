
#define CEBUS_IMPLEMENTATION
#include "cebus.h"

#include "sandbox.h"

int main(void) {
  compile_claymore();
  compile_file(STR("src/sandbox.c"));
}
