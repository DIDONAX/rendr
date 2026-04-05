#pragma once

#include "rendr/camera.h"
#include "rendr/window.h"
#include "rendr/shader_source.h"
#include "rendr/draw_command.h"
#include "rendr/mesh_storage.h"
#include "rendr/model_storage.h"

namespace rendr {

using namespace glfw;

struct system {
    window* window_;
    shader_programm* program_;
    camera cam_;

    mesh_storage* meshes_;
    model_storage* models_;
    std::vector<draw_command> commands_;
    vertex_buffer* mdi_buff_;

    system();
    ~system();
 
    instance_id add_instance(const mesh_id, const offset_t off); 
    void update_instance(const instance_id, const offset_t off);
    void upload_batch();
    void update_cam();
    mesh_id add_mesh(const geometry&);
    void draw() const;

    private:
        void set_initial_state();
        void allocate_resources();
        void specify_attributes();
        void upload_geom();
};

} // namespace rendr


