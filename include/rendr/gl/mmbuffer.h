#pragma once 

#include "glad/gl.h"

#include "rendr/gl/allocator.h"
#include "rendr/gl/enums.h"
#include "rendr/utils.h"
#include <cstddef>

namespace rendr {
// GLINVALID_OPERATION:
// - The buffer is already in a mapped state.
// - Neither MAP_READ_BIT nor MAP_WRITE_BIT is set.
// - MAP_READ_BIT is set and any of MAP_INVALIDATE_RANGE_BIT,MAP_INVALIDATE_BUFFER_BIT, or MAP_UNSYNCHRONIZED_BIT is set.
// - MAP_FLUSH_EXPLICIT_BIT is set and MAP_WRITE_BIT is not set.
// - Any of MAP_READ_BIT, MAP_WRITE_BIT, MAP_PERSISTENT_BIT, or MAP_COHERENT_BIT are set, but the same bit is not set in the buffer’sstorage flags.

constexpr auto kIncreaseFactor = 2U;

// NOTE: can the allocator be made stl compatible ? -> how to handle id on resize ? 
// TODO: assign , copy constructor
template<typename T, Protection P>
class mmbuffer {
    public :
        mmbuffer() {
            mmblock_ = allocate<T, P>(capacity_);
        }
        mmbuffer(const std::size_t n) : capacity_(n) {
            assert(n != 0 && "capacity cant be 0");
            mmblock_ = allocate<T, P>(n);
        }
        ~mmbuffer() {
            deallocate<T, P>(mmblock_);
        }

        mmbuffer(const mmbuffer&) = delete;
        mmbuffer& operator=(const mmbuffer&) = delete;
        mmbuffer& operator=(mmbuffer&&);

        T* data() const {return mmblock_.data_;}
        T* begin() const {return mmblock_.data_;}
        T* end() const {return mmblock_.data_+size_;}

        uint id() const {return mmblock_.id_;}
        size_t size() const { return size_;}
        std::size_t capacity() const {return capacity_;}

        T& operator[](const std::size_t idx) {
            // assert(idx < size_ && "index out of range");
            return mmblock_.data_[idx];
        }

        const T& operator[](const std::size_t idx) const {
            // assert(idx < size_ && "index out of range");
            return mmblock_.data_[idx];
        }

        void push_back(const T& value) {
            auto c = required_capacity(++size_);
            if (c != 0) {
                reallocate(c);
            }
            mmblock_.data_[size_- 1] = value;
        }

        bool empty() const {
            return size_ == 0;
        }

        void clear() {
            size_ = 0;
        }

        void reserve(const std::size_t n) {
            auto c = required_capacity(n);
            if (c != 0) {
                reallocate(c);
            }
        }

        // void insert(const std::size_t pos, const T& value) {
        // }

        template<typename Container>
        void insert_back(const Container& container) {
            auto n = size_+container.size();
            auto c = required_capacity(n);
            if (c != 0) {
                reallocate(c);
            }
            copy(data()+size_, container.data(), container.size());
            size_ = n;
        }

    private:
        mmblock<T, P> mmblock_;
        size_t size_{0};
        size_t capacity_{1};

        void reallocate(const std::size_t n) {
            auto a = allocate<T, P>(n);
            copy(a.data_, mmblock_.data_, size_);
            deallocate<T, P>(mmblock_);
            mmblock_ = a;
        }

        std::size_t required_capacity(const std::size_t n) const {
            auto c = capacity();
            assert(c != 0 && "capacity cant be 0");

            if (n <= c) return 0;

            while (c < n) {
                c *= kIncreaseFactor;
            }
            return c;
        }
};

template<typename T>
using mvector = mmbuffer<T, WriteO>;

}
