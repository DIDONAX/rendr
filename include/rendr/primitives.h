#pragma once
#include "rendr/mesh_storage.h"

namespace rendr {

geometry load_triangle();
geometry load_rectangle();
geometry load_circle(int s = 32, float radius = 1); 
geometry load_sphere(int lat = 8, int lon = 16); 
geometry load_cube();
geometry load_cylinder(int segments = 16, float radius = 0.05f, float height = 0.1f); 


} // namespace rendr
