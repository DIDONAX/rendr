#pragma once

#include <print>
#include "glad/gl.h"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "rendr/utils.h"

namespace rendr::glw {

using uint = GLuint;

enum Flags : GLuint {
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

template<std::size_t C, typename T>
class mapped_buffer {
    public :
        // investigate usage flag bug
        mapped_buffer(T* data = nullptr, uint usage = Dynamic) {
            glCreateBuffers(1, &id_);
            if (id_ == 0) throw std::runtime_error("glCreateBuffers failed");
            auto storage_flags = Flags::Persistent | Flags::Coherent | Flags::Write;
            glNamedBufferStorage(id_, C * sizeof(T), (void*)data , storage_flags);
            data_ = static_cast<T*>(glMapNamedBufferRange(id_, 0, C * sizeof(T), storage_flags));
            if (!data_) throw std::runtime_error("glMapNamedBufferRange failed");
        }

        ~mapped_buffer() {
            if (id_== 0) return; 
            glUnmapNamedBuffer(id_);
            glDeleteBuffers(1, &id_);
        }

        mapped_buffer(const mapped_buffer&) = delete;
        mapped_buffer& operator=(const mapped_buffer&) = delete;
        mapped_buffer& operator=(mapped_buffer&&) noexcept;


        uint id() const {return id_;}
        std::size_t capacity() const { return C;}
        std::size_t size() const { return size_;}
        T* data() const {return data_;}

        void binding_loc(const uint32_t loc) const {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, loc, id_); 
        }

        void push_back(const T& v) {
            if (size_ >= C) throw std::overflow_error("mapped_buffer capacity exceeded");
            data_[size_++] = v;
        }

        void insert(const std::vector<T>& src) {
            auto s = size_ + src.size();
            if (s > C) throw std::overflow_error("mapped_buffer capacity exceeded");
            host_to_device(data_+size_, src);
            size_+= src.size();
        }

    private:
        uint id_{0};
        T* data_{nullptr};
        size_t size_{0};
};

struct vertex_array {
    struct attribute {
        uint location_{};
        int size_{};
        uint stride_{}; // relative offset 
    };

    uint id_;

    ~vertex_array() {if (id_ != 0) glDeleteVertexArrays(1, &id_);}
    vertex_array() {glCreateVertexArrays(1, &id_);}

    void set(const attribute& attr) {
        glEnableVertexArrayAttrib(id_, attr.location_);
        glVertexArrayAttribFormat(id_, attr.location_, attr.size_, GL_FLOAT, GL_FALSE, attr.stride_);
        glVertexArrayAttribBinding(id_, attr.location_, 0);
    }
    void set_element_buff(const auto& ebo) const {
        glVertexArrayElementBuffer(id_, ebo.id());
    }
    void set_vertex_buff(const auto& vbo, const uint bind_loc, const size_t stride) const {
        glVertexArrayVertexBuffer(id_, bind_loc, vbo.id(), 0, stride);
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
