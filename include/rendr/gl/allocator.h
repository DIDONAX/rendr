#pragma once 

#include "rendr/gl/enums.h"

namespace rendr {

constexpr flag_t kBaseFlags = Persistent | Coherent;

template<typename T, Protection P>
struct mmblock {
    uint id_{0};
    T* data_{nullptr};
};

// creates namedbuffer, allocates memory
template<typename T, Protection P>
inline mmblock<T, P> allocate(const std::size_t n) {
    mmblock<T, P> alloc;
    auto& ptr = alloc.data_;
    auto& id = alloc.id_;

    glCreateBuffers(1, &id);
    assert(id!=0 && "glCreateBuffers failed");

    glNamedBufferStorage(id, n * sizeof(T), static_cast<const void*>(ptr) , kBaseFlags | Dynamic | P);
    ptr = static_cast<T*>(glMapNamedBufferRange(id, 0, n * sizeof(T), kBaseFlags | P));
    assert(ptr && "glMapNamedBufferRange failed");
    return alloc;
}

// destroys and deallocates
// TODO: use InvalidateBufferSubData()
template<typename T, Protection P>
inline void deallocate(const mmblock<T, P>& alloc) {
    if (alloc.id_ == 0) return;
    glUnmapNamedBuffer(alloc.id_);
    glDeleteBuffers(1, &alloc.id_);
}

}
