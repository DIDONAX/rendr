#pragma once

#include <cstring>
#include <unistd.h>
#include <vector>
#include <cassert>
#include <filesystem>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace rendr {

template<typename T>
inline void host_to_device(T* dest, const std::vector<T>& src) {
    std::memcpy(dest, src.data(), src.size() * sizeof(T));
}

std::string load_file(const std::filesystem::path& path);


template<typename T>
inline T char_to(char*& cursor) {
    T n;
    auto start = cursor;
    while (*cursor != ' ' && *cursor != '\n' && *cursor != '\0') ++cursor;
    std::from_chars(start, cursor, n);
    ++cursor;
    return n;
}

template<typename T>
inline std::span<T> mmap(const std::filesystem::path& path) {
    auto fd = open(path.string().c_str(), O_RDONLY);
    assert(fd != -1 && "failed to open file");

    struct stat sb;
    fstat(fd, &sb);
    const auto size = sb.st_size;

    auto data = static_cast<T*>(::mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0));
    assert(data != MAP_FAILED && "file mapping failed");

    close(fd);
    return std::span<T>(data, size); 
}

} // namespace rendr
