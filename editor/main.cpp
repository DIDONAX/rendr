#include "editor.h"
#include "rendr/context.h"
#include "rendr/primitives.h"

constexpr auto N = 100000U;
constexpr auto p = 3.0f;
constexpr auto q = 5.0f;

static_assert(N < rendr::kGlobalInstanceCapacity);

void run_animation(float t, std::span<rendr::offset_t> offs,
                   std::span<rendr::color_t> cols,
                   const std::vector<glm::vec3> &static_noise) {
  auto i = 0U;

  const auto t_phase = t * 0.3F;
  const auto t_pulse = t * 1.5f;

  const auto t_color_r = t * 2.0f;
  const auto t_color_g = t * 1.5f + 2.0f;
  const auto t_color_b = t * 1.0f + 4.0f;

  for (auto &off : offs) {
    auto f = static_cast<float>(i);
    auto u = f * 0.02f + t_phase;
    auto pulse = 1.0f + 0.2f * std::sin(f * 0.05f - t_pulse);
    auto base_r = (std::cos(q * u) + 2.0f) * 0.25f * pulse;

    off.x = base_r * std::cos(p * u) + static_noise[i].x;
    off.y = base_r * std::sin(p * u) + static_noise[i].y;
    off.z = -std::sin(q * u) * 0.25f * pulse + static_noise[i].z;

    cols[i].r = 0.5f + 0.5f * std::sin(t_color_r + f * 0.01f);
    cols[i].g = 0.5f + 0.5f * std::sin(t_color_g + f * 0.02f);
    cols[i].b = 0.5f + 0.5f * std::sin(t_color_b + f * 0.03f);

    i++;
  }
}

int main() {
  rendr::editor e;
  auto& cam = e.default_camera();
  cam.position_.z = -2;
  auto ctx = e.ctx();
  auto quad = ctx->create_mesh(rendr::load_quad());

  for (auto i = 0U; i < N; ++i) {
    rendr::model_matrix m;
    m.scale_ /= 100;
    ctx->create_instance(quad, m, {});
  }

  auto offs = ctx->get_offset_range(quad);
  auto cols = ctx->get_color_range(quad);

  std::vector<glm::vec3> static_noise(offs.size());

  for (auto i = 0U; i < offs.size(); ++i) {
    auto f = static_cast<float>(i);
    static_noise[i].x = std::sin(f * 134.0f) * 0.05f;
    static_noise[i].y = std::cos(f * 173.0f) * 0.05f;
    static_noise[i].z = std::sin(f * 197.0f) * 0.05f;
  }

  while (e.running()) {
    auto t = e.clock();
    run_animation(t, offs, cols, static_noise);
    e.display();
  }
}
