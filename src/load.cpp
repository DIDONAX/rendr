#include "rendr/load.h"
#include "rendr/geometry.h"

#include <fstream>
#include <print>

namespace rendr {
namespace fs = std::filesystem;

geometry load_obj(const fs::path& path) {
    if (path.extension() != ".obj") throw std::invalid_argument("load_obj: invalid file format");
    const auto file = mmap<char>(path);
    const auto data = file.data();
    const auto size = file.size();

    geometry geom;
    const auto end = data + size;
    auto cursor = data;
    while (cursor < end) { 
        if (cursor[0] == 'v' && cursor[1] == ' ') {
            cursor+=2;
            geom.vertices_.push_back({
                char_to<float>(cursor),
                char_to<float>(cursor),
                char_to<float>(cursor),
            });
        } else if (*cursor == 'f') {
            cursor+=2;
            geom.indices_.insert(geom.indices_.end(), {
                char_to<index_t>(cursor)-1,
                char_to<index_t>(cursor)-1,
                char_to<index_t>(cursor)-1,
            });
        } else {
            cursor++;
        }
    }
    munmap(data, size);
    return geom;
}

std::string load_file(const fs::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw std::invalid_argument("load_file: invalid file path");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

}
