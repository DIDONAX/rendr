#pragma once
#include "rendr/mesh_storage.h"

namespace rendr {

constexpr mesh_id cube = 0;
constexpr mesh_id sphere = 1;
constexpr mesh_id cylinder = 2;

geometry load_cube();
geometry load_sphere(int lat = 8, int lon = 16); 
geometry load_cylinder(int segments = 16, float radius = 0.05f, float height = 0.1f); 
// geometry load_circle(int s = 32, float radius = 1); 
geometry load_triangle();
// geometry load_rectangle(float w = 0.3, float h = 0.3);


} // namespace rendr
