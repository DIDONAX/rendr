#pragma once

#include "rendr/mesh_storage.h"

namespace rendr {

inline geometry load_obj(const std::filesystem::path& path) {
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
        } else if (*cursor == '/') {
            throw std::runtime_error("/ format not supported yet");
        } else {
            cursor++;
        }
    }
    munmap(data, size);
    return geom;
}

}
