#include "sound.h"
#include "miniaudio.h"

static ma_engine engine;

// TODO: efx, music, ambient sound group
// TODO: play loops
// TODO: volume controll
// TOOD: 3d sound

bool cm_sound_init(void) {
  ma_result result;
  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    return false;
  }

  return true;
}

void cm_sound_play(const char *filename) {
  ma_engine_play_sound(&engine, filename, NULL);
}

void cm_sound_shutdown(void) { ma_engine_uninit(&engine); }
