#pragma once
#include <cstring>
#include <vector>

namespace rendr {

template<typename T>
void host_to_device(void* dest, const std::vector<T>& src) {
    std::memcpy(dest, src.data(), src.size() * sizeof(T));
}

} // namespace rendr
