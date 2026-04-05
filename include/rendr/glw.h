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

struct vertex_buffer {
    uint id_{0};
    ~vertex_buffer() {
        if (id_!= 0) {
            unmap();
            glDeleteBuffers(1, &id_);
        }
    }
    vertex_buffer() {glCreateBuffers(1, &id_);}
    void alloc(const auto data, const size_t bytes, const uint32_t sf) {
        assert(id_ != 0 && "alloc() fail: delete and reallocate instead");
        glNamedBufferStorage(id_, bytes, data, sf);
        capacity_ = bytes;
        alloc_flags_ = sf;
    }
    void map(const uint32_t mf) {
        data_ = glMapNamedBufferRange(id_, 0, capacity_, mf);
        assert(data_ && "map() fail: check flags");
    }
    void unmap() {glUnmapNamedBuffer(id_);}
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
    void bind_element_buff(const vertex_buffer& ebo) const {
        glVertexArrayElementBuffer(id_, ebo.id_);
    }
    void bind_vertex_buff(const vertex_buffer& vbo, const uint bind_loc, const size_t stride) const {
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

struct shader_programm {
    uint id_;
    int view_loc_;
    int proj_loc_;

    void update_view(float* mat4f) {
        glUniformMatrix4fv(view_loc_, 1, GL_FALSE, mat4f);
    }
    void update_proj(float* mat4f) {
        glUniformMatrix4fv(proj_loc_, 1, GL_FALSE, mat4f);
    }
    void use() {glUseProgram(id_);}

    ~shader_programm(){glDeleteProgram(id_);}
    shader_programm(const char* vs, const char* fs){
        id_ = glCreateProgram();

        auto compile = [&](auto type, auto src) -> uint {
            uint s = glCreateShader(type);
            glShaderSource(s, 1, &src, nullptr);
            glCompileShader(s);
            glAttachShader(id_, s);
            return s;
        };

        auto f = compile(GL_FRAGMENT_SHADER, fs);
        auto v = compile(GL_VERTEX_SHADER, vs);

        glLinkProgram(id_);

        glDeleteShader(f);
        glDeleteShader(v);

        view_loc_ = glGetUniformLocation(id_, "view");
        proj_loc_ = glGetUniformLocation(id_, "proj");
    }
};

} // namespace rendr
