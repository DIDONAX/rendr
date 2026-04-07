#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <print>

#include "glad/gl.h"

namespace rendr::glw {

using uint = GLuint;

enum Flags : uint32_t {
    Static = 0,
    Dynamic = GL_DYNAMIC_STORAGE_BIT,
    Read = GL_MAP_READ_BIT,
    Write = GL_MAP_WRITE_BIT,
    Persistent = GL_MAP_PERSISTENT_BIT,
    Coherent = GL_MAP_COHERENT_BIT,
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

// this is too coupled, needs rewrite
struct shader_program {
    uint id_{0};
    int view_loc_{-1};
    int proj_loc_{-1};

    void update_view(float* mat4f) {
        glUniformMatrix4fv(view_loc_, 1, GL_FALSE, mat4f);
    }
    void update_proj(float* mat4f) {
        glUniformMatrix4fv(proj_loc_, 1, GL_FALSE, mat4f);
    }
    void use() {glUseProgram(id_);}

    uint compile(auto type, const char* src) {
        uint s = glCreateShader(type);
        glShaderSource(s, 1, &src, nullptr);
        glCompileShader(s);
        int success;
        glAttachShader(id_, s);
        return s;
    }

    shader_program(const char* cs) {
        id_ = glCreateProgram();
        auto c = compile(GL_COMPUTE_SHADER, cs);
        int success;
        glGetShaderiv(c, GL_COMPILE_STATUS, &success);
        assert(success && "compute shader compile error");
        glLinkProgram(id_);
        glGetProgramiv(id_, GL_LINK_STATUS, &success);
        assert(success && "shader link error");
        glDeleteShader(c);
    }
    shader_program(const char* vs, const char* fs){
        id_ = glCreateProgram();
        int success;
        auto f = compile(GL_FRAGMENT_SHADER, fs);
        glGetShaderiv(f, GL_COMPILE_STATUS, &success);
        assert(success && "fragment shader compile error");
        auto v = compile(GL_VERTEX_SHADER, vs);
        glGetShaderiv(v, GL_COMPILE_STATUS, &success);
        assert(success && "vertex shader compile error");

        glLinkProgram(id_);
        glGetProgramiv(id_, GL_LINK_STATUS, &success);
        assert(success && "shader link error");
        glDeleteShader(f);
        glDeleteShader(v);

        view_loc_ = glGetUniformLocation(id_, "view");
        proj_loc_ = glGetUniformLocation(id_, "proj");
    }
};

} // namespace rendr
