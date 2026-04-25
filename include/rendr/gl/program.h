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
    void set_umat4f(const std::string& uniform , const glm::mat4& value) const;
    bool link() const; 
};

}
