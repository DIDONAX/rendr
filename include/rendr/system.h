#pragma once

#include "rendr/mesh_storage.h"
#include "rendr/model_storage.h"
#include "rendr/draw_command.h"
#include "rendr/types.h"
#include "rendr/camera.h"
#include "rendr/window.h"
#include <cstring>

namespace rendr {

// keeps ref stable in case of swaping
struct object {
    instance_id id_;
};

struct object_desc {
    glm::vec3 position_{0};
    float uniform_scale_{1};
    color_t color_{1};
};

// TODO: SDP vs noSDP possible 2x
struct system {
    program* program_;
    mesh_storage* meshes_;
    model_storage* models_;
    shader_storage* mdi_;

    system();
    ~system();
    void set_attr(const object&, const object_desc&);
    void update_colors(const mesh_id, const std::vector<color_t>&) const;
    void update_rotations(const mesh_id, const std::vector<rotation_t>&) const;
    void update_offsets(const mesh_id, const std::vector<offset_t>&) const;
    void update_global_cols(const std::vector<color_t>&) const;
    void update_global_offs(const std::vector<offset_t>&) const;
    void update_camera(const camera&);
    object add_instance(const mesh_id, const offset_t& off = {}, const color_t& col = glm::vec4{1}, const rotation_t& rot = glm::mat4{1}, const scale_t& scale = glm::mat4{1});
    object add_instance(const mesh_id, const glm::vec3& off = {}, const color_t& col = glm::vec4{1}, const rotation_t& rot = glm::mat4{1}, const scale_t& scale = glm::mat4{1});
    mesh_id add_mesh(const geometry&);
 
    void draw();
    void clear() const;

    // removing will prob never happen in real scenario, for debug gui mostly
    // void remove_instance(const object&);
    void wireframe(const bool b);

    private:
        void set_initial_state();
        void allocate_resources();
        void specify_attributes();
        void upload_geom();
        void load_geom();
};

} // namespace rendr


