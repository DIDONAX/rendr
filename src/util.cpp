#include "rendr/utils.h"

#include <fstream>
#include <stdexcept>

namespace rendr {

std::string load_file(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw std::invalid_argument("invalid file path");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

}
