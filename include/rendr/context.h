#pragma once

#include "rendr/camera.h"
#include "rendr/draw_command.h"
#include "rendr/mesh_storage.h"
#include "rendr/model_storage.h"
#include "rendr/types.h"


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

struct model_matrix {
    offset_t offset_{0};
    rotation_t rotation_{1};
    scale_t scale_{1};
};

// TODO: SDP vs noSDP possible 2x
struct context {

    program program_;
    mesh_storage meshes_;
    model_storage models_;
    mapped_buffer<kMeshCapacity, draw_command> mdi_{nullptr, Static};

    context();
    ~context();

    void set_attr(const object&, const object_desc&);

    void update_colors(const mesh_id, const std::vector<color_t>&) const;
    void update_rotations(const mesh_id, const std::vector<rotation_t>&) const;
    void update_offsets(const mesh_id, const std::vector<offset_t>&) const;
    void update_camera(const camera&);
    void update_instance_model(const instance_id, const model_matrix&);

    instance_id add_instance(const mesh_id, const model_matrix& model, const color_t = glm::vec4{1}) const;
    mesh_id add_mesh(const geometry&);
 
    void draw();
    void clear() const;
    void wireframe(const bool b);

    private:
        void set_initial_state();
        void set_bindings();
        void specify_attributes();
        void upload_geom();
        void load_geom();

        template<typename T, typename Container>
        void update_buffer(const mesh_id id, Container& buffer, const std::vector<T>& data) const; 
};

template<typename T, typename Container>
void context::update_buffer(const mesh_id id, Container& buffer, const std::vector<T>& data) const {
    assert(data.size() <= kInstanceCapacity && "instance capacity reached, resize needed");
    auto off = id * kInstanceCapacity;
    host_to_device(reinterpret_cast<T*>(buffer.data()) + off, data);
}

} // namespace rendr


