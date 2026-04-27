#pragma once 

#include <cstddef>
#include <iomanip>
#include <stdexcept>
#include <vector>

#include "glad/gl.h"

#include "rendr/gl/enums.h"
#include "rendr/types.h"
#include "rendr/utils.h"

namespace rendr {

//fixed size for now
template<std::size_t C, typename T>
class mapped_buffer {
    public :
        // investigate usage flag bug
        mapped_buffer(T* data = nullptr, uint usage = Dynamic) {
            glCreateBuffers(1, &id_);
            if (id_ == 0) throw std::runtime_error("glCreateBuffers failed");
            auto storage_flags = Flags::Persistent | Flags::Coherent | Flags::Write;
            glNamedBufferStorage(id_, C * sizeof(T), (void*)data , storage_flags);
            data_ = static_cast<T*>(glMapNamedBufferRange(id_, 0, C * sizeof(T), storage_flags));
            if (!data_) throw std::runtime_error("glMapNamedBufferRange failed");
        }

        ~mapped_buffer() {
            if (id_== 0) return; 
            glUnmapNamedBuffer(id_);
            glDeleteBuffers(1, &id_);
        }

        T& operator[](const std::size_t idx) {
            assert(idx < C && "index out of range");
            return data_[idx];
        }

        mapped_buffer(const mapped_buffer&) = delete;
        mapped_buffer& operator=(const mapped_buffer&) = delete;
        mapped_buffer& operator=(mapped_buffer&&) noexcept;


        uint id() const {return id_;}
        std::size_t capacity() const { return C;}
        std::size_t size() const { return size_;}
        T* data() const {return data_;}

        void binding_loc(const uint32_t loc) const {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, loc, id_); 
        }

        void push_back(const T& v) {
            if (size_ >= C) throw std::overflow_error("mapped_buffer capacity exceeded");
            data_[size_++] = v;
        }

        void insert(const std::vector<T>& src) {
            auto s = size_ + src.size();
            if (s > C) throw std::overflow_error("mapped_buffer capacity exceeded");
            host_to_device(data_+size_, src);
            size_+= src.size();
        }

    private:
        uint id_{0};
        T* data_{nullptr};
        size_t size_{0};
};


}
