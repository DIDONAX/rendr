#pragma once

#include "rendr/gl/shader.h"
#include "rendr/types.h"

namespace rendr {

struct program {
    uint id_{0};

    program();
    ~program();

    void attach(const shader& shader) const;
    void detach(const shader& shader) const;
    void use() const;
    bool link() const;
};

}
