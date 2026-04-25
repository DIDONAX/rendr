#pragma once
#include <cstring>
#include <filesystem>
#include <vector>

namespace rendr {

template<typename T>
void host_to_device(void* dest, const std::vector<T>& src) {
    std::memcpy(dest, src.data(), src.size() * sizeof(T));
}

std::string load_file(const std::filesystem::path& path);

} // namespace rendr
