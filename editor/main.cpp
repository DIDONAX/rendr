#include "editor.h"
#include "rendr/context.h"
#include "rendr/primitives.h"

#include <cmath>

constexpr auto N = 150000U;

static_assert(N <= rendr::kGlobalInstanceCapacity);

void run_animation(float t, std::span<rendr::offset_t> offs,
                   std::span<rendr::color_t> cols,
                   const std::vector<glm::vec3> &seed) {
  // Cosmic vortex: spiral arms + chaotic radius/height weave.
  // Hot loop: ~1 div, 2 sin, 1 cos per particle.
  const auto spin = t;
  const auto storm = t * 0.85f;

  for (auto i = 0U; i < offs.size(); ++i) {
    const auto r0 = seed[i].x; // base radius
    const auto p = seed[i].y;  // phase
    const auto h = seed[i].z;  // height bias [-1,1]

    // Radius never settles — each particle breathes on its own phase
    const auto breathe = std::sin(storm + p * 2.0f);
    const auto r = r0 * (0.5f + 0.5f * breathe) + 0.06f * std::sin(spin + p);

    // Spiral: wind with radius, spin faster near the core (cheap 1/r)
    const auto inv = 1.0f / (r + 0.1f);
    const auto theta = p + spin * inv + r * 6.0f + h * 0.8f;

    const auto ct = std::cos(theta);
    const auto st = std::sin(theta);

    // Counter-swirl shear so it doesn't read as a rigid helix
    const auto shear = 0.12f * h * std::sin(theta + storm);

    offs[i].x = r * ct + shear * st;
    offs[i].y = r * st - shear * ct;
    offs[i].z = h * (0.15f + 0.55f * r) + 0.1f * st;

    // Hot core -> cool rim
    const auto heat = inv * 0.1f;
    cols[i].r = 0.25f + heat * 1.4f;
    cols[i].g = 0.35f + heat * 0.6f + 0.15f * breathe;
    cols[i].b = 0.55f + 0.35f * r0 - heat * 0.3f;
  }
}

void update_camera(float t, rendr::camera &cam) {
  const auto yaw = t * 0.3f;
  const auto pitch = 0.25f * std::sin(t * 0.14f);
  constexpr auto radius = 1.5;
  const auto cp = std::cos(pitch);

  cam.target_ = {0.0f, 0.0f, 0.0f};
  cam.position_ = {radius * cp * std::cos(yaw), radius * std::sin(pitch),
                   radius * cp * std::sin(yaw)};
  cam.direction_ = glm::normalize(cam.target_ - cam.position_);
}

int main() {
  rendr::editor editor;
  auto &cam = editor.default_camera();
  auto ctx = editor.ctx();
  auto cube = ctx->create_mesh(rendr::load_cube(), N);

  for (auto i = 0U; i < N; ++i) {
    rendr::model_matrix m;
    m.scale_ /= 130;
    ctx->create_instance(cube, m, {});
  }

  auto offs = ctx->get_offset_range(cube);
  auto cols = ctx->get_color_range(cube);

  // seed: radius, phase, height bias
  std::vector<glm::vec3> seed(offs.size());
  for (auto i = 0U; i < offs.size(); ++i) {
    const auto f = static_cast<float>(i);
    const auto u = std::sin(f * 12.9898f) * 43758.5453f;
    const auto v = std::sin(f * 78.233f) * 23421.6312f;
    const auto w = std::sin(f * 45.164f) * 19234.1284f;
    // fractal-ish radius bias toward the core
    const auto ru = u - std::floor(u);
    seed[i].x = 0.05f + ru * ru * 0.55f;
    seed[i].y = v;           // raw phase (large)
    seed[i].z = std::sin(w); // height bias [-1,1]
  }

  while (true) {
    auto t = editor.clock();
    update_camera(t, cam);
    run_animation(t, offs, cols, seed);
    if (!editor.running()) {
      break;
    }
    editor.display();
  }
}
