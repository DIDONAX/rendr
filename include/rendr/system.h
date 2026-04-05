#pragma once

#include <vector>

#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_float3.hpp"

#include "rendr/glw.h"
#include "rendr/shader_source.h"
#include "rendr/window.h"


namespace rendr {

using namespace glw;
using namespace glfw;

using mesh_id = size_t;
using instance_id = size_t;

using position_t = glm::vec3;
using offset_t = glm::vec4;
using index_t = uint;

constexpr auto sf = Flags::Dynamic | Flags::Write | Flags::Persistent | Flags::Coherent;
constexpr auto mf = Flags::Write | Flags::Persistent | Flags::Coherent;

struct draw_command {
    uint count_;
    uint32_t instance_count;
    uint first_index_;
    int  base_vertex_;
    uint base_instance_;
};

struct mesh_storage_info {
    size_t size_{1};
    size_t capacity_{1};
    size_t vert_capacity_{3};
    size_t ind_capacity_{3};
};

struct geometry {
    std::vector<position_t> vertices_;
    std::vector<index_t> indices_;
};

struct mesh_storage {
    mesh_storage_info info_;
    geometry geom_;
    vertex_array attributes_;
    vertex_buffer vertices_;
    vertex_buffer indices_;
    mesh_id add(const geometry&);
};

struct model_storage {
    // per mesh
    size_t capacity_{10000000};
    vertex_buffer offset_buff_;
    std::vector<offset_t> offsets_;
};

struct system {
    window* window_;
    shader_programm* program_;

    mesh_storage* meshes_;
    model_storage* models_;
    std::vector<draw_command> commands_;
    vertex_buffer* indirect_buffer_;

    system();
    ~system();
 
    instance_id add_instance(const mesh_id, const offset_t off); 
    void remove_instance(const instance_id); 
    void upload_batch();
    void draw() const;
    void update_cam();
    void update_instance(const instance_id, const offset_t off);

    private:
        void set_initial_state();
        void allocate_resources();
        void specify_attributes();
        void upload_geom();
};

} // namespace rendr


