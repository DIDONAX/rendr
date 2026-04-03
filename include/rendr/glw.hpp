#pragma once
#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"
#include "types.h"
#include <vector>

namespace rendr::gl {

using uint = GLuint;

enum class buffer_target : GLenum {
    array    = GL_ARRAY_BUFFER,
    element  = GL_ELEMENT_ARRAY_BUFFER,
    indirect = GL_DRAW_INDIRECT_BUFFER,
};

enum class buffer_flags : GLbitfield {
    dynamic    = GL_DYNAMIC_STORAGE_BIT,
    persistent = GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT,
};

enum class shader_type : GLenum {
    vertex   = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER,
};

enum class pixel_type : GLenum {
    uint8  = GL_UNSIGNED_BYTE,
    float32 = GL_FLOAT,
};

struct vertex_array {
    uint id_;
    vertex_array() { glCreateVertexArrays(1, &id_); }
    ~vertex_array() { glDeleteVertexArrays(1, &id_); }
    void bind() { glBindVertexArray(id_); }
    void unbind() { glBindVertexArray(0); }
};

struct buffer_object {
    uint id_;
    buffer_object() { glCreateBuffers(1, &id_); }
    ~buffer_object() { glDeleteBuffers(1, &id_); }

    template<typename T>
    void storage(const std::vector<T>& data, buffer_flags flags) {glNamedBufferStorage(id_, sizeof(T) * data.size(), data.data(), static_cast<GLbitfield>(flags));}
    void bind(buffer_target target) {glBindBuffer(static_cast<GLenum>(target), id_);}
};

struct storage_buffer {
    uint id_;
    storage_buffer() { glCreateBuffers(1, &id_); }
    ~storage_buffer() { glDeleteBuffers(1, &id_); }

    template<typename T>
    void storage(const std::vector<T>& data, buffer_flags flags) {glNamedBufferStorage(id_, sizeof(T) * data.size(), data.data(), static_cast<GLbitfield>(flags));}
    void bind_base(uint binding) {glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, id_);}
};

struct shader {
    uint id_;
    shader(shader_type type) { id_ = glCreateShader(static_cast<GLenum>(type)); }
    ~shader() { glDeleteShader(id_); }

    void source(const char* src) {
        glShaderSource(id_, 1, &src, nullptr);
        glCompileShader(id_);
    }
};

struct program {
    uint id_;
    program() { id_ = glCreateProgram(); }
    ~program() { glDeleteProgram(id_); }

    void attach(const shader& s) { glAttachShader(id_, s.id_); }
    void link() { glLinkProgram(id_); }
    void use() { glUseProgram(id_); }

    void uniform(GLint loc, const glm::mat4& m) {glProgramUniformMatrix4fv(id_, loc, 1, GL_FALSE, glm::value_ptr(m));}
    void uniform(GLint loc, const glm::vec4& v) {glProgramUniform4fv(id_, loc, 1, glm::value_ptr(v));}
    void uniform(GLint loc, float v) { glProgramUniform1f(id_, loc, v); }
    void uniform(GLint loc, int v) { glProgramUniform1i(id_, loc, v); }
};

} // namespace rendr::gl
