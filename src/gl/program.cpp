#include "rendr/gl/program.h"
#include "glm/gtc/type_ptr.hpp"

namespace rendr {

program::program() {id_ = glCreateProgram();}
program::~program() {glDeleteProgram(id_);}

void program::attach(const shader& shader) const {glAttachShader(id_, shader.id_);}

void program::detach(const shader& shader) const {glDetachShader(id_, shader.id_);}

void program::use() const {glUseProgram(id_);}

void program::set_umat4f(const std::string& uniform , const glm::mat4& value) const {
    int loc = glGetUniformLocation(id_, uniform.data());
    glProgramUniformMatrix4fv(id_, loc, 1, false, glm::value_ptr(value));
}

bool program::link() const {
    glLinkProgram(id_);
    int linked = 0;
    glGetProgramiv(id_, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[512]; 
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, nullptr);
        glGetProgramInfoLog(id_, sizeof(log), nullptr , log);
        std::println("link status: {}", log);

        glDeleteProgram(id_);
        return false;
    }
    return true;
}

}
