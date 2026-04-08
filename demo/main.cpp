#include "rendr/primitives.h"
#include "rendr/system.h"
#include "rendr/camera.h"
#include "rendr/types.h"
#include "rendr/window.h"
#include <glm/gtc/random.hpp>
#include <print>
#include <random>

using namespace rendr;

struct particle_system {
    std::vector<glm::vec4> pos;
    std::vector<glm::vec3> vel;
    std::vector<glm::vec4> col;

    void init(size_t count) {
        pos.resize(count);
        vel.resize(count);
        col.resize(count);

        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * 3.14159f);
        std::uniform_real_distribution<float> distRadius(50.0f, 70.0f);

        for (size_t i = 0; i < count; ++i) {
            float angle = distAngle(rng);
            float radius = distRadius(rng);

            pos[i] = glm::vec4(
                radius * cos(angle),
                radius * sin(angle),
                0.0f,
                1.0f
            );

            glm::vec3 tangent = glm::vec3(-pos[i].y, pos[i].x, 0.0f);
            tangent = glm::normalize(tangent);

            float speed = sqrt(20000.0f / (radius + 1.0f)); 
            vel[i] = tangent * speed;

            col[i] = glm::vec4(1.0f);
        }
    }

    void update(float dt) {
        const size_t n = pos.size();

        const float G = 30000.0f;
        const float softening = 300.0f;
        const float drag = 0.999f;

        for (size_t i = 0; i < n; ++i) {
            glm::vec2 p = glm::vec2(pos[i]);

            float distSq = glm::dot(p, p) + softening;

            float invDist = 1.0f / sqrt(distSq);
            float gravityMagnitude = G * invDist * invDist;
            glm::vec2 gravityAccel = glm::normalize(-p) * gravityMagnitude;

            glm::vec2 v = glm::vec2(vel[i]);
            v += gravityAccel * dt;
            v *= drag;

            p += v * dt;

            pos[i].x = p.x;
            pos[i].y = p.y;
            pos[i].z = 0.0f;

            vel[i].x = v.x;
            vel[i].y = v.y;
            vel[i].z = 0.0f;
        }
    }
};


int main() {
    camera cam;
    cam.speed_ = 30;
    cam.position_.z = -40;
    window win;
    rendr::system s;

    float last_frame{0};
    float curr_frame{0};
    int num{0};

    particle_system ps;
    ps.init(10000);

    for (auto& p : ps.pos) {
        s.add_instance(0, p);
    }

    float delta{0}; 
    float time{0.f};

    while (win.is_open()) {
        std::println("fps: {}", 1/delta);
        win.poll_event();
        curr_frame = glfwGetTime();
        delta = curr_frame-last_frame;
        time+=delta;
        last_frame = curr_frame;
        win.clear();

        s.update_camera(cam);

        ps.update(delta);
        s.update_offsets(0, ps.pos);
        s.update_colors(0, ps.col);

        s.draw();
        win.display();
    }
    return 0;
}


