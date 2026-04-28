#pragma once 

#include "glad/gl.h"

#include "rendr/gl/allocator.h"
#include "rendr/gl/enums.h"
#include "rendr/utils.h"
#include <cstddef>
#include <cstring>
#include <string>

namespace rendr {

// GLINVALID_OPERATION:
// - The buffer is already in a mapped state.
// - Neither MAP_READ_BIT nor MAP_WRITE_BIT is set.
// - MAP_READ_BIT is set and any of MAP_INVALIDATE_RANGE_BIT,MAP_INVALIDATE_BUFFER_BIT, or MAP_UNSYNCHRONIZED_BIT is set.
// - MAP_FLUSH_EXPLICIT_BIT is set and MAP_WRITE_BIT is not set.
// - Any of MAP_READ_BIT, MAP_WRITE_BIT, MAP_PERSISTENT_BIT, or MAP_COHERENT_BIT are set, but the same bit is not set in the buffer’sstorage flags.

constexpr auto kIncreaseFactor = 2U;

// TODO: assign , copy constructor
template<typename T, Protection P>
class mmbuffer {
    public :
        mmbuffer() {
            allocation_ = allocator_.allocate(allocation_.size_);
        }
        mmbuffer(const std::size_t n ) {
            allocation_ = allocator_.allocate(n);
            assert(n != 0 && "capacity cant be 0");
        }
        ~mmbuffer() {
            allocator_.deallocate(allocation_);
        }

        mmbuffer(const mmbuffer&) = delete;
        mmbuffer& operator=(const mmbuffer&) = delete;
        mmbuffer& operator=(mmbuffer&&);

        T* data() const {return allocation_.data_;}
        T* begin() const {return allocation_.data_;}
        T* end() const {return allocation_.data_+size_;}

        uint id() const {return allocation_.id_;}
        size_t size() const { return size_;}
        std::size_t capacity() const {return allocation_.size_;}

        T& operator[](const std::size_t idx) {
            // assert(idx < size_ && "index out of range");
            return allocation_.data_[idx];
        }

        const T& operator[](const std::size_t idx) const {
            assert(idx < size_ && "index out of range");
            return allocation_.data_[idx];
        }

        void push_back(const T& value) {
            auto c = require_realloc(++size_);
            if (c != 0) {
                reallocate(c);
            }
            allocation_.data_[size_- 1] = value;
        }

        template<typename Container>
        void insert(const Container& container) {
            auto n = size_+container.size();
            auto c = require_realloc(n);
            if (c != 0) {
                reallocate(c);
            }
            copy(data()+size_, container.data(), container.size());
            size_ = n;
        }

    private:
        allocation<T, P> allocation_;
        allocator<T, P> allocator_;
        size_t size_{0};

        void reallocate(const std::size_t n) {
            auto a = allocator_.allocate(n);
            copy(a.data_, allocation_.data_, size_);
            allocator_.deallocate(allocation_);
            allocation_ = a;
        }

        std::size_t require_realloc(const std::size_t n) const {
            auto c = capacity();
            assert(c != 0 && "capacity cant be 0");

            if (n <= c) return 0;

            while (c < n) {
                c *= kIncreaseFactor;
            }
            return c;
        }
};

}
