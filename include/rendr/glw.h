#pragma once

#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"
#include "rendr/utils.h"
#include <cassert>
#include <cstddef>
#include <print>
#include <string>
#include <filesystem>

namespace rendr::glw {

using uint = GLuint;

enum Flags {
    Static = 0,
    Dynamic = GL_DYNAMIC_STORAGE_BIT,
    Read = GL_MAP_READ_BIT,
    Write = GL_MAP_WRITE_BIT,
    Persistent = GL_MAP_PERSISTENT_BIT,
    Coherent = GL_MAP_COHERENT_BIT,
};

enum ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Compute = GL_COMPUTE_SHADER
};

struct shader_storage {
    uint id_{0};
    ~shader_storage() {
        if (id_!= 0) {
            unmap();
            glDeleteBuffers(1, &id_);
        }
    }
    shader_storage() {glCreateBuffers(1, &id_);}
    void alloc(const auto data, const size_t bytes, const uint32_t sf) {
        assert(id_ != 0 && "alloc() fail: delete and reallocate instead");
        glNamedBufferStorage(id_, bytes, data, sf);
        capacity_ = bytes;
        alloc_flags_ = sf;
    }
    void binding_loc(const uint32_t loc) const {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, loc, id_); 
    }
    void map(const uint32_t mf) {
        data_ = glMapNamedBufferRange(id_, 0, capacity_, mf);
        assert(data_ && "map() fail: check flags");
    }
    void unmap() {glUnmapNamedBuffer(id_);}

    // this shouldnt be stored in case of realloc
    void* data() const {
        assert(data_ && "data() fail: check flags");
        return data_;
    }

    shader_storage(const shader_storage&) = delete;
    shader_storage& operator=(const shader_storage&) = delete;
    private:
        void* data_{nullptr};
        size_t capacity_{0};
        uint32_t alloc_flags_{0};
};

struct vertex_array {
    uint id_;

    ~vertex_array() {if (id_ != 0) glDeleteVertexArrays(1, &id_);}
    vertex_array() {glCreateVertexArrays(1, &id_);}
    void bind_element_buff(const shader_storage& ebo) const {
        glVertexArrayElementBuffer(id_, ebo.id_);
    }
    void bind_vertex_buff(const shader_storage& vbo, const uint bind_loc, const size_t stride) const {
        glVertexArrayVertexBuffer(id_, bind_loc, vbo.id_, 0, stride);
    }
    void enable_attrib(const uint loc) const {
        glEnableVertexArrayAttrib(id_, loc);
    }
    void format_attrib(const uint loc, const int size, const uint rel_off) const {
        glVertexArrayAttribFormat(id_, loc, size, GL_FLOAT, GL_FALSE, rel_off);
    }
    void bind_attrib(const uint loc, const uint bind_loc = 0) const {
        glVertexArrayAttribBinding(id_, loc, bind_loc);
    }

};

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

struct program {
    uint id_{0};

    program() {id_ = glCreateProgram();}
    ~program() {glDeleteProgram(id_);}

    void attach(const shader& shader) const {glAttachShader(id_, shader.id_);}
    void detach(const shader& shader) const {glDetachShader(id_, shader.id_);}
    void use() const {glUseProgram(id_);}
    void set_umat4f(const std::string& uniform , const glm::mat4& value) const {
        int loc = glGetUniformLocation(id_, uniform.data());
        glProgramUniformMatrix4fv(id_, loc, 1, false, glm::value_ptr(value));
    }

    bool link() const {
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
};

} // namespace rendr
