#include "rendr/primitives.h"
#include <cmath>

// all follow ccw
namespace rendr {

geometry load_cube() {
    geometry g;
    g.vertices_ = {
        {-0.05,-0.05, 0.05}, { 0.05,-0.05, 0.05}, { 0.05, 0.05, 0.05}, {-0.05, 0.05, 0.05}, // Front (z+)
        {-0.05,-0.05,-0.05}, { 0.05,-0.05,-0.05}, { 0.05, 0.05,-0.05}, {-0.05, 0.05,-0.05}  // Back (z-)
    };
    g.indices_ = {
        0,1,2,0,2,3, // f
        1,5,6,1,6,2, // r
        5,4,7,5,7,6, // ba
        4,0,3,4,3,7, // l
        3,2,6,3,6,7, // t
        4,5,1,4,1,0  // bot
    };
    return g;
}

geometry load_triangle() {
    return {
        .vertices_ = {
            {0.0f,0.5f,0.0f},
            {-0.5f,-0.5f,0.0f},
            {0.5f,-0.5f,0.0f}
        },
        .indices_ = {0,1,2}
    };
}

geometry load_sphere(int lat, int lon) {
    geometry g;
    for(int i = 0; i <= lat; i++) {
        float theta = i * M_PI / float(lat);
        float sinTheta = sin(theta), cosTheta = cos(theta);
        for(int j = 0; j <= lon; j++) {
            float phi = j * 2.0f * M_PI / float(lon);
            float x = cos(phi) * sinTheta * 0.05f;
            float y = cosTheta * 0.05f;
            float z = sin(phi) * sinTheta * 0.05f;
            g.vertices_.push_back({x, y, z});
        }
    }

    for(int i = 0; i < lat; i++) {
        for(int j = 0; j < lon; j++) {
            int first = i * (lon + 1) + j;
            int second = first + lon + 1;

            g.indices_.push_back(first);
            g.indices_.push_back(second);
            g.indices_.push_back(first + 1);

            g.indices_.push_back(second);
            g.indices_.push_back(second + 1);
            g.indices_.push_back(first + 1);
        }
    }
    return g;
}

geometry load_cylinder(int segments, float radius, float height) {
    geometry g;
    float half_height = height / 2.0f;

    uint32_t top_center_idx = 0;
    uint32_t bottom_center_idx = 1;
    g.vertices_.push_back({0,  half_height, 0}); 
    g.vertices_.push_back({0, -half_height, 0}); 

    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * M_PI * i / segments;
        float x = radius * std::cos(theta);
        float z = radius * std::sin(theta);
        g.vertices_.push_back({x,  half_height, z});
        g.vertices_.push_back({x, -half_height, z});
    }

    for (int i = 0; i < segments; ++i) {
        int next = (i + 1) % segments;
        uint32_t t1 = 2 + (i * 2);
        uint32_t t2 = 2 + (next * 2);
        uint32_t b1 = 3 + (i * 2);
        uint32_t b2 = 3 + (next * 2);

        g.indices_.insert(g.indices_.end(), {t1, b1, b2});
        g.indices_.insert(g.indices_.end(), {t1, b2, t2});
        g.indices_.insert(g.indices_.end(), {top_center_idx, t2, t1});
        g.indices_.insert(g.indices_.end(), {bottom_center_idx, b2, b1});
    }
    return g;
}

} // namespace rendr
