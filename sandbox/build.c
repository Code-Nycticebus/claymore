#define CM_DIR "../claymore/"
#include "../claymore/build/claymore.c"

int main(void) {
  create_directory(".dump");
  claymore_project_build_and_run(PATH(".dump/sandbox"), PATH("."), /* rebuild = */ true);
}
