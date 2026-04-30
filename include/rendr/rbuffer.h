#pragma once

#include "glad/gl.h"
#include "rendr/gl/mmbuffer.h"
#include <cstddef>
#include <vector>
#include <cstring>

namespace rendr {

constexpr auto kNumSlots = 3U;

template <typename T>
struct rbuffer {
    std::size_t write_idx_{0};
    std::size_t read_idx_{0};

    mvector<T> data_;
    std::vector<GLsync> fences_;

    rbuffer(std::size_t n) : data_(kNumSlots * n), fences_(kNumSlots * n, nullptr) {}

    void wait(GLsync& fence) {
        if (!fence) return;
        while (true) {
            auto res = glClientWaitSync(fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
            if (res == GL_ALREADY_SIGNALED || res == GL_CONDITION_SATISFIED) break;
        }
        glDeleteSync(fence);
        fence = nullptr;
    }

    void write(const T* src, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) {
            auto idx = (write_idx_ + i) % data_.size();
            wait(fences_[idx]);
            data_[idx] = src[i];
        }

        GLsync fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

        for (std::size_t i = 0; i < n; ++i) {
            std::size_t idx = (write_idx_ + i) % data_.size();
            fences_[idx] = fence;
        }

        write_idx_ = (write_idx_ + n) % data_.size();
    }
};

} // namespace rendr
