#pragma once

#include "rendr/gl/mmbuffer.h"
#include "rendr/gl/program.h"
#include "rendr/draw_command.h"
#include "rendr/mesh_storage.h"
#include "rendr/model_storage.h"
#include "rendr/camera.h"
#include "rendr/types.h"


namespace rendr {

struct model_matrix {
    offset_t offset_{0};
    rotation_t rotation_{1};
    scale_t scale_{1};
};

// TODO: SDP vs noSDP possible 2x
struct context {

    program program_;
    mesh_storage meshes_;

    model_storage models_; // TODO: resize after add_instance
    mmbuffer<draw_command, WriteO> mdi_{}; // TODO: update offsets after add_instance

    context();
    ~context();

    void update_colors(const mesh_id, const std::vector<color_t>&);
    void update_rotations(const mesh_id, const std::vector<rotation_t>&);
    void update_offsets(const mesh_id, const std::vector<offset_t>&);
    void update_camera(const camera&);
    void update_instance_model(const instance_id, const model_matrix&);

    instance_id add_instance(const mesh_id, const model_matrix& model = {}, const color_t = White);
    mesh_id add_mesh(const geometry&);
 
    void draw() const;
    void clear() const;
    void wireframe(const bool b) const;

    private:
        void set_initial_state();
        void set_bindings();
        void specify_attributes();
        void upload_geom();
        void load_geom();
        void sync();

        template<typename T, typename Container>
        void update_buffer(const mesh_id id, Container& buffer, const std::vector<T>& data); 
};

template<typename T, typename Container>
void context::update_buffer(const mesh_id id, Container& buffer, const std::vector<T>& data) {
    auto cmd = mdi_[id]; 
    auto off = cmd.base_instance_;;
    copy(reinterpret_cast<T*>(buffer.data()) + off, data.data(), data.size());
}

} // namespace rendr


