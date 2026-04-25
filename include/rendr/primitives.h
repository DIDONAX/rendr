#pragma once
#include "rendr/mesh_storage.h"

namespace rendr {

constexpr mesh_id triangle = 0;
constexpr mesh_id cube = 1;
constexpr mesh_id quad = 2;

geometry load_triangle();
geometry load_quad();
geometry load_cube();

} // namespace rendr
