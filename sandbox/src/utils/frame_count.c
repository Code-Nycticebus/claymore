#include "frame_count.h"
#include <stdio.h>

typedef struct {
  FILE *outfile;
} FrameCount;

static void init(CmScene *scene) {
  FrameCount *fc = cm_scene_set_data(scene, sizeof(FrameCount));
  fc->outfile = stdout;
}

static void fixed_update(CmScene *scene, double dt) {
  FrameCount *fc = scene->data;
  (void)fc, (void)dt;
  io_write_fmt(fc->outfile, "%ld,,,50\n", cm_app_time());
  fflush(fc->outfile);
}

static void frame_update(CmScene *scene, double dt) {
  FrameCount *fc = scene->data;
  io_write_fmt(fc->outfile, "%ld, %g, %g, \n", cm_app_time(), 1 / dt, dt);
  fflush(fc->outfile);
}

static CmSceneInterface *interface(void) {
  static CmSceneInterface interface = {
      .init = init,
      .fixed_update = fixed_update,
      .frame_update = frame_update,
  };
  return &interface;
}

CmScene *frame_count(CmScene *parent, Str filename) {
  CmScene *scene = cm_scene_push(parent, interface);

  if (!str_eq(filename, STR("stdout"))) {
    FrameCount *fc = scene->data;
    fc->outfile = file_open(filename, "wb", ErrPanic);
    io_write_str(fc->outfile, STR("time,fps,ms,fixed\n"), ErrPanic);
  }

  return scene;
}