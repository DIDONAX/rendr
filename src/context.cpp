#include "rendr/context.h"

#include "glm/ext/matrix_clip_space.hpp"

#include "rendr/gl/bind.h"
#include "rendr/gl/draw.h"
#include "rendr/primitives.h"

namespace rendr {

void create_pipeline(const program& p ) {
    std::println("create_pipeline()");
    shader fs = shader("../assets/fragment.glsl", ShaderType::Fragment);
    shader vs = shader("../assets/vertex.glsl", ShaderType::Vertex);
    p.attach(fs);
    p.attach(vs);
    p.link();
    p.detach(fs);
    p.detach(vs);
}

context::context() {
    std::println("context()");
    create_pipeline(program_);
    set_bindings();
    specify_attributes();
    load_geom();
    set_initial_state();
};

context::~context() {};

void context::wireframe(const bool b) const {
    glPolygonMode(GL_FRONT_AND_BACK, b ? GL_LINE : GL_FILL);
}

void context::set_viewport(const int x,const int y, const int w, const int h) const {
    glViewport(x, y, w, h);
}

void context::set_clear_color(const color_t color) const {
    glClearColor(color.r, color.g, color.b, color.a);
}

void context::clear() const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}

mesh_id context::add_mesh(const geometry& geom) {
    std::println("add_mesh()");
    auto id = mdi_.size();

    auto base_instance{0};
    if (id != 0) {
        base_instance = mdi_[id-1].instance_count + mdi_[id-1].base_instance_;
    }

    mdi_.push_back({ 
        .count_ = static_cast<uint>(geom.indices_.size()),
        .instance_count = 0,
        .first_index_ = static_cast<uint>(meshes_.indices_.size()), 
        .base_vertex_ = static_cast<int>(meshes_.vertices_.size()),
        .base_instance_ = static_cast<uint>(base_instance)
    });
    meshes_.add(geom);
    sync();
    return id;
}

// TODO: add explicit fencing and triple ring buffer
void context::draw() const {
    multi_draw<Triangle>(0, mdi_.size());
};

instance_id context::add_instance(const mesh_id id, const model_matrix& mat, const color_t color) {
    auto& cmd = mdi_[id];
    auto idx = cmd.base_instance_ + cmd.instance_count;
    auto& m = models_;

    m.offsets_[idx] = mat.offset_;
    m.quaternions_[idx] = mat.quaternions_;
    m.scales_[idx] = mat.scale_;
    m.colors_[idx] = color;
    cmd.instance_count++;
    sync();
    return idx;
}

void context::update_instance_model(const instance_id id, const model_matrix& mat) {
    models_.offsets_[id] = mat.offset_;
    models_.quaternions_[id] = mat.quaternions_;
    models_.scales_[id] = mat.scale_;
}

void context::update_camera(const camera& cam) {
    program_.set_umat4f("view", compute_view(cam));
    program_.set_umat4f("proj", glm::perspective(
        glm::radians(90.F),
        cam.aspect_,
        0.1F,
        1000.F 
    ));
}


void context::load_geom() {
    std::println("load_geom()");
    add_mesh(load_triangle());
    add_mesh(load_quad());
    add_mesh(load_cube());
}

void context::update_offsets(const mesh_id id, const std::vector<offset_t>& offsets) {
    update_buffer(id, models_.offsets_, offsets);
}

void context::update_rotations(const mesh_id id, const std::vector<quaternion_t>& rotations) {
    update_buffer(id, models_.quaternions_, rotations);
}

void context::update_colors(const mesh_id id, const std::vector<color_t>& colors) {
    update_buffer(id, models_.colors_, colors);
}

void context::set_initial_state() {
    std::println("set_inital_state()");
    program_.use();
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);  
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    set_clear_color(Black);
    set_viewport(0, 0, 900, 900);
    update_camera({.aspect_ = (float)900/(float)900});
    sync();
}

void context::sync() {
    set_bindings();
    specify_attributes();
    bind<VertexLayout>(meshes_.attributes_.id_);
    bind<IndirectBuffer>(mdi_.id());
}

void context::set_bindings() {
    std::println("set_bindings()");
    const auto& m = models_;
    bind<StorageBuffer, 0>(m.offsets_.id());
    bind<StorageBuffer, 1>(m.colors_.id());
    bind<StorageBuffer, 2>(m.quaternions_.id());
    bind<StorageBuffer, 3>(m.scales_.id());
}

void context::specify_attributes() {
    auto& a = meshes_.attributes_;
    a.set_vertex_buff(meshes_.vertices_, 0, sizeof(position_t));
    a.set_element_buff(meshes_.indices_);

    vertex_array::attribute geom = {
        .location_ = 0,
        .size_ = 3 ,
        .stride_ = 0
    };

    a.set(geom);
}

// TODO: track added objs if remove is enabled, find obj with id = last and set id = obj.id_;

}  // namespace rendD
