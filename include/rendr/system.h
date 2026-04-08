#pragma once

#include "rendr/mesh_storage.h"
#include "rendr/model_storage.h"
#include "rendr/shader_source.h"
#include "rendr/draw_command.h"
#include "rendr/types.h"
#include "rendr/camera.h"
#include "rendr/window.h"
#include <cstring>

namespace rendr {

// too keep ref stable in case of swaping
struct object {
    instance_id id_;
};

// TODO: SDP vs noSDP possible 2x
struct system {
    shader_program* raster_program_;
    mesh_storage* meshes_;
    model_storage* models_;
    shader_storage* mdi_;

    system();
    ~system();

    void update_colors(const mesh_id, const std::vector<color_t>&) const;
    void update_offsets(const mesh_id, const std::vector<offset_t>&) const;
    void update_global_cols(const std::vector<color_t>&) const;
    void update_global_offs(const std::vector<offset_t>&) const;

    void update_camera(const camera&);

    object add_instance(const mesh_id, const offset_t& off = {}, const color_t& col = glm::vec4{1}); 
    mesh_id add_mesh(const geometry&);
 
    void draw();

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


