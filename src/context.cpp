#include "rendr/context.h"

#include "glm/ext/matrix_clip_space.hpp"

#include "rendr/primitives.h"

namespace rendr {

void create_pipeline(const program& p ) {
    shader fs = shader("assets/fragment.glsl", ShaderType::Fragment);
    shader vs = shader("assets/vertex.glsl", ShaderType::Vertex);
    p.attach(fs);
    p.attach(vs);
    p.link();
    p.detach(fs);
    p.detach(vs);
}

context::context() {
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

void context::clear() const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}

// TODO: change when allocation logic is changed
mesh_id context::add_mesh(const geometry& geom) {
    auto idx = mdi_.size();
    mdi_.push_back({ 
        .count_ = static_cast<uint>(geom.indices_.size()),
        .instance_count = 0,
        .first_index_ = static_cast<uint>(meshes_.indices_.size()), 
        .base_vertex_ = static_cast<int>(meshes_.vertices_.size()),
        // assumes same slot size per mesh type
        .base_instance_ = static_cast<uint>(idx*kInstanceCapacity), 
    });
    meshes_.add(geom);
    return idx;
}

// TODO: add explicit fencing and triple ring buffer
void context::draw() const {
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, mdi_.size(), 0);
};

instance_id context::add_instance(const mesh_id id, const model_matrix& mat, const color_t color) {
    auto& cmd = mdi_[id];
    auto idx = id * kInstanceCapacity + cmd.instance_count;
    auto& m = models_;
    m.offsets_[idx] = mat.offset_;
    m.rotations_[idx] = mat.rotation_;
    m.scales_[idx] = mat.scale_;
    m.colors_[idx] = color;
    cmd.instance_count++;
    return idx;
}

void context::update_instance_model(const instance_id id, const model_matrix& mat) {
    models_.offsets_[id] = mat.offset_;
    models_.rotations_[id] = mat.rotation_;
    models_.scales_[id] = mat.scale_;
}

void context::update_camera(const camera& cam) {
    program_.set_umat4f("view", compute_view(cam));
}

void context::load_geom() {
    add_mesh(load_triangle());
    add_mesh(load_quad());
    add_mesh(load_cube());
}

void context::update_offsets(const mesh_id id, const std::vector<offset_t>& offsets) {
    update_buffer(id, models_.offsets_, offsets);
}

void context::update_rotations(const mesh_id id, const std::vector<rotation_t>& rotations) {
    update_buffer(id, models_.rotations_, rotations);
}

void context::update_colors(const mesh_id id, const std::vector<color_t>& colors) {
    update_buffer(id, models_.rotations_, colors);
}

void context::set_initial_state() {
    program_.use();
    program_.set_umat4f("proj",glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10000.f));
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(meshes_.attributes_.id_);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, mdi_.id());
    update_camera({});
}

void context::set_bindings() {
    const auto& m = models_;
    m.offsets_.binding_loc(0);
    m.colors_.binding_loc(1);
    m.rotations_.binding_loc(2);
    m.scales_.binding_loc(3);
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
// void context::remove_instance(const object& obj) {
//     // add checks
//     auto mesh_id = obj.id_ / models_->capacity_;
//     auto off = static_cast<offset_t*>(models_->offsets_.data());
//     auto& cmd = static_cast<draw_command*>(mdi_->data())[mesh_id];
//     auto last = mesh_id * models_->capacity_ + cmd.instance_count - 1;
//     off[obj.id_] = off[last];
//     cmd.instance_count--;
// }

}  // namespace rendD
