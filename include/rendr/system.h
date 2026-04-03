#pragma once

#include <iterator>
#include <vector>

#include "rendr/glw.hpp"
#include "rendr/shader_sources.h"
#include "rendr/types.h"
#include "rendr/constants.h"
#include "rendr/window.h"


namespace rendr {

using mesh_id = size_t;
using instance_id = size_t;

struct geometry {
    std::vector<position_t> positions_;
    std::vector<index_t> indices_;
};

struct mesh {
    uint count_;
    uint instance_count;
    uint first_index_;
    int base_vertex_;
    uint base_instance_;
};

struct shader_data {
    gl::shader* vert_shader_;
    gl::shader* frag_shader_;
    gl::program* program_;

    ~shader_data();
};

struct buffer_data {
    gl::vertex_array* vertex_array_;
    gl::buffer_object* element_buffer_;
    gl::buffer_object* indirect_buffer_;

    gl::storage_buffer* positions_;
    gl::storage_buffer* transforms_;
    gl::storage_buffer* colors_;

    ~buffer_data();
};

struct instance_data {
    std::vector<transform_t> transforms_data_;
    std::vector<color_t> colors_data_;
};

struct system {
    glfw::window* window_;
    shader_data* shaders_;
    buffer_data* buffers_;

    instance_data instances_;
    std::vector<mesh> meshes_;
    geometry geom_;

    system();
    ~system();

    mesh_id add_mesh(const geometry&);
    void remove_mesh(const mesh_id);

    instance_id add_instance(const mesh_id); 
    void remove_instance(const mesh_id); 

    void update_transform(instance_id iid, transform_t t);
    void update_color(instance_id iid, color_t c);
    void draw();
};

} // namespace rendr


