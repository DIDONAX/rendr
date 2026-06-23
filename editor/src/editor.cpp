#include "editor.h"
#include "rendr/camera.h"
#include "rendr/context.h"
#include "rendr/primitives.h"
#include "rendr/types.h"
#include "rendr/window.h"

#include "gui.h"

// example usage of the rendr library, OpenGl context lifecycle and windowing
// all managed application level, the library operates on the current bound
// context if provided

namespace rendr {

editor::editor() {
  window::settings s = {
      .title_ = "Editor", .vsync_ = false, .mode_ = rendr::Maximized};

  window_ = new window(s);
  ctx_ = new context();
  auto id =
      ctx_->create_program("../assets/vertex.glsl", "../assets/fragment.glsl");
  ctx_->use_program(id);

  gui::init(*window_);

  sync_frame_buffer();
  gui::cache_asset_files("assets");
}

editor::~editor() {
  gui::destroy();
  delete ctx_;
  delete window_;
}

void editor::sync_frame_buffer() {
  auto [width, height] = window_->frame_buffer_size();
  camera_ = {.aspect_ = static_cast<float>(width) / static_cast<float>(height)};

  ctx_->viewport(0, 0, width, height);
  ctx_->update_uniform(
      {.view_ = compute_view(camera_), .proj_ = compute_proj(camera_)});
}

camera &editor::default_camera() { return camera_; }

float editor::clock() {
  using namespace std::chrono;
  static auto start = steady_clock::now();
  auto now = steady_clock::now();
  return duration<float>(now - start).count();
}

context *editor::ctx() { return ctx_; }

bool editor::running() const {
  window_->poll_event();
  ctx_->update_uniform(
      {.view_ = compute_view(camera_), .proj_ = compute_proj(camera_)});
  return window_->is_open();
}

void editor::display() {
  ctx_->clear();
  ctx_->draw();

  gui::begin_frame();
  // gui::draw_assets(*ctx_, *this);
  // gui::draw_scene(*this);
  gui::draw_fps();
  // gui::draw_guizmo(*ctx_);
  // gui::draw_properties(*ctx_);
  gui::end_frame();

  window_->swap_buffers();
}

} // namespace rendr
