#include "rendr/context.h"

#include "glad/gl.h"

#include "rendr/gl/bind.h"


namespace rendr {

program_id context::create_program(const fs::path& vertex, const fs::path& fragment) {
    programs_.emplace_back();
    program& p = programs_.back();

    shader fs = shader(fragment, ShaderType::Fragment);
    shader vs = shader(vertex, ShaderType::Vertex);

    p.attach(fs);
    p.attach(vs);
    p.link();
    p.detach(fs);
    p.detach(vs);
    return programs_.size()-1;
}

void context::use_program(const program_id id) {
    programs_[id].use();
}

context::context() {
    fs::path v = "../assets/vertex.glsl";
    fs::path f = "../assets/fragment.glsl";
    create_program(v, f);

    set_bindings();
    create_vertex_layout();
    set_initial_state();
};

context::~context() {};

void context::wireframe(const bool b) const {
    glPolygonMode(GL_FRONT_AND_BACK, b ? GL_LINE : GL_FILL);
}

void context::viewport(const int x,const int y, const int w, const int h) const {
    glViewport(x, y, w, h);
}

void context::clear_color(const color_t color) const {
    glClearColor(color.r, color.g, color.b, color.a);
}

void context::clear() const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}

mesh_id context::create_mesh(const geometry& geom) {
    auto id = meshes_.add(geom);
    auto base_instance = [&](auto& id) {
        auto b{0};
        if (id != 0) {
            b = draw_commands_[id-1].instance_count + draw_commands_[id-1].base_instance_;
        }
        return b;
    };

    draw_commands_.push_back({ 
        .count_ = static_cast<uint>(geom.indices_.size()),
        .instance_count = 0,
        .first_index_ = static_cast<uint>(meshes_.data_[id].ioff_),
        .base_vertex_ = static_cast<int>(meshes_.data_[id].voff_),
        .base_instance_ = static_cast<uint>(base_instance(id))
    });

    sync();
    return id;
}

instance_id context::create_instance(const mesh_id id, const model_matrix& mat, const color_t color) {
    auto& cmd = draw_commands_[id];
    auto idx = cmd.base_instance_ + cmd.instance_count;
    auto& m = models_;

    m.offsets_[idx] = mat.offset_;
    m.quaternions_[idx] = mat.quaternion_;
    m.scales_[idx] = mat.scale_;
    m.colors_[idx] = color;
    cmd.instance_count++;
    sync();
    return idx;
}

void context::update_uniform(const camera_uniform& uniform) {
    uniforms_[0] = uniform;
}

void context::set_initial_state() {
    programs_[0].use();
    glEnable(GL_DEPTH_TEST);

    point_size(4);
    clear_color(White);
    viewport(0, 0, 900, 900);
    sync();
}
 
void context::point_size(float size) {
    glPointSize(size);
}

void context::sync() {
    set_bindings();
    create_vertex_layout();
    bind<VertexLayout>(meshes_.attributes_.id_);
    bind<IndirectBuffer>(draw_commands_.id());
}

void context::set_bindings() {
    const auto& m = models_;
    bind<StorageBuffer, 0>(m.offsets_.id());
    bind<StorageBuffer, 1>(m.colors_.id());
    bind<StorageBuffer, 2>(m.quaternions_.id());
    bind<StorageBuffer, 3>(m.scales_.id());
    bind<UniformBuffer, 4>(uniforms_.id());
}

// TODO:add multiple layout default formats
void context::create_vertex_layout() {
    auto& a = meshes_.attributes_;
    a.set(meshes_.vertices_, 0, sizeof(position_t));
    a.set(meshes_.indices_);

    vertex_array::attribute geom = {
        .location_ = 0,
        .size_ = 3 ,
        .stride_ = 0
    };

    a.set(geom);
}

}  // namespace rendD
