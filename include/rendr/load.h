#pragma once
#include <fstream>

#include "rendr/mesh_storage.h"
#include "rendr/utils.h"
#include <cassert>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <print>

namespace rendr {

inline geometry load_obj(const std::filesystem::path& path) {
    if (path.extension() != ".obj") throw std::invalid_argument("load_obj: invalid file format");
    auto file = mmap<char>(path);
    auto data = file.data();
    auto size = file.size();

    geometry geom;
    const auto end = file.data() + size;
    auto cursor = file.data();
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
            throw std::runtime_error("/ format not supported");
        } else {
            cursor++;
        }
    }

    munmap(data, size);

    return geom;
}

}
