#pragma once

#include "rendr/camera.h"
#include "rendr/context.h"
#include "rendr/types.h"
#include "rendr/window.h"

namespace rendr {

class editor {
public:
  editor();
  ~editor();

  bool running() const;
  void display();
  context *ctx();
  camera &default_camera();
  float clock();

private:
  window *window_;
  context *ctx_;
  rendr::camera camera_;
  void sync_frame_buffer();
};

} // namespace rendr
