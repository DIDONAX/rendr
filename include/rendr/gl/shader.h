#pragma once

#include <filesystem>
#include <print>

#include "rendr/gl/enums.h"
#include "rendr/load.h"

namespace rendr {

struct shader {
uint id_{0};

~shader() {glDeleteShader(id_);} 

shader(std::filesystem::path path, const ShaderType& type) {
    id_ = glCreateShader(static_cast<GLenum>(type));
    auto src = load_file(path);
    auto ptr = src.data();
    glShaderSource(id_, 1, &ptr, nullptr);
    glCompileShader(id_);

    int compiled = 0;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        char log[512]; 
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, nullptr);
        glGetShaderInfoLog(id_, sizeof(log), nullptr , log);
        std::println("compile status: {}", log);
        glDeleteShader(id_);
    }
}

};

}
