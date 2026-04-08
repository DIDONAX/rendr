#pragma once
#include <cstddef>
#include <sys/types.h>

namespace rendr {

template<typename T, typename F>
class mapped_vector {
    public:
        ~mapped_vector<T, F>();
        mapped_vector<T, F>(size_t capacity) : capacity_(capacity) {
            // glNamedBufferStorage(id_, );
        }

    private:
        size_t capacity_;
        size_t size_;
        T* data_;
        uint id_;
}; 

} // namespace rendr
