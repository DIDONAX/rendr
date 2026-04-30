#pragma once

#include "rendr/mesh_storage.h"

namespace rendr {

std::string load_file(const std::filesystem::path& path);
geometry load_obj(const std::filesystem::path& path); 
geometry load_ply(const std::filesystem::path& path); 

}
