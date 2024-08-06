/* Ok so i don't know whats happening. clangd has some problems with CEBUS_IMPLEMENTATION.
 * So i came up with this so it shuts up.
 * */
#include "cebus.h" // IWYU pragma: keep

#if defined(CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#endif

#define CEBUS_IMPLEMENTATION
#include "cebus.h" // IWYU pragma: keep

#if defined(CLANG)
#pragma clang diagnostic ignored "-Wall"
#endif
