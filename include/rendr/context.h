#pragma once

#include "rendr/gl/draw.h"
#include "rendr/gl/program.h"
#include "rendr/model_storage.h"
#include "rendr/camera.h"

namespace rendr {

namespace fs = std::filesystem;

struct model_matrix {
    offset_t offset_{0,0,0,1};
    quaternion_t quaternion_{1,0,0,0};
    scale_t scale_{1};
};

// TODO: make context define and work with arbitrary uniforms
struct camera_uniform {
    mat4 view_;
    mat4 proj_;
};

struct context {
    std::vector<program> programs_;
    mesh_storage meshes_;
    model_storage models_; // TODO: resize after add_instance / arena allocator > per insance grow factor ?
    mvector<draw_command> draw_commands_; // TODO: update offsets after add_instance ? clear draw commd after each frame ? 
    mvector<camera_uniform> uniforms_;

    context();
    ~context();

    mesh_id create_mesh(const geometry&);
    instance_id create_instance(const mesh_id, const model_matrix& model = {}, const color_t = Black);

    program_id create_program(const fs::path& vertex, const fs::path& fragment);
    void use_program(const program_id);

    void update_uniform(const camera_uniform&);

    // TODO: add explicit fencing and triple ring buffer
    template<Primitive P = Triangle>
    void draw() const {
        multi_draw<P>(0, draw_commands_.size());
    }

    void clear() const;
    void clear_color(const color_t color) const;
    void viewport(const int x,const int y, const int w, const int h) const;
    void wireframe(const bool b) const;
    void point_size(float);

    private:
        void set_initial_state();
        void set_bindings();
        void create_vertex_layout();
        void upload_geom();
        void sync();
};

} // namespace rendr


