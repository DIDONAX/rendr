#include "rendr/system.h"
#include <cmath>
#include <cstring>
#include <print>
#include <vector>
#include <random>

struct particle_system {
    std::vector<float> px, py, pz;
    std::vector<float> vx, vy, vz;
    size_t count;
    particle_system(size_t n) : count(n) {
        px.resize(n); py.resize(n); pz.resize(n);
        vx.resize(n); vy.resize(n); vz.resize(n);
        std::mt19937 rng(42);
        std::uniform_real_distribution<float> angle(0, 2 * M_PI);
        std::uniform_real_distribution<float> radius(0.1f, 1.8f);
        std::uniform_real_distribution<float> spread(-0.1f, 0.1f);

        for (size_t i = 0; i < n; ++i) {
            float a = angle(rng);
            float r = radius(rng);
            // spiral arms: offset angle by radius
            a += r * 2.5f;

            px[i] = r * cosf(a);
            py[i] = spread(rng) * 0.3f;
            pz[i] = r * sinf(a);

            // orbital velocity perpendicular to radius, scaled by 1/sqrt(r)
            float speed = 0.4f / sqrtf(r);
            vx[i] = -sinf(a) * speed;
            vy[i] = spread(rng) * 0.001f;
            vz[i] =  cosf(a) * speed;
        }
    }

    void update(float dt) {
        constexpr float G  = 0.08f;   // gravity strength
        constexpr float softening = 0.01f; // avoids singularity at origin

        // update velocity — pull toward origin (central mass)
        for (size_t i = 0; i < count; ++i) {
            float r2 = px[i]*px[i] + py[i]*py[i] + pz[i]*pz[i] + softening;
            float inv_r3 = G / (r2 * sqrtf(r2));
            vx[i] -= px[i] * inv_r3 * dt;
            vy[i] -= py[i] * inv_r3 * dt;
            vz[i] -= pz[i] * inv_r3 * dt;
        }

        // update position
        for (size_t i = 0; i < count; ++i) {
            px[i] += vx[i] * dt;
            py[i] += vy[i] * dt;
            pz[i] += vz[i] * dt;
        }
    }
};

int main() {
   double lastTime = glfwGetTime();
   double timer = 0.0;
   int frames = 0;

   rendr::system r;

   constexpr size_t N = 100000;

   particle_system particles(N);

   // register all instances at initial positions
   std::vector<rendr::instance_id> ids;
   ids.reserve(N);
   for (size_t i = 0; i < N; ++i) {
     ids.push_back(r.add_instance(0, {particles.px[i], particles.py[i], particles.pz[i], 0}));
   }

   r.upload_batch();

   auto t = 0.0f;
   particles.update(t);
   for (size_t i = 0; i < N; ++i) {
      r.update_instance(ids[i], {particles.px[i], particles.py[i], particles.pz[i], 0});
   }
  while (r.window_->is_open()) {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    timer += deltaTime;
    frames++;

    if (timer >= 1.0) {
        double fps = frames / timer;
        std::println("fps: {}", fps);
        frames = 0;
        timer = 0.0;
    }

    r.window_->poll_event();
    r.window_->clear();
    r.update_cam();
    r.draw();
    r.window_->display();
} 
}
