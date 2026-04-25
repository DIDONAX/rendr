#include "rendr/system.h"
#include "rendr/draw_command.h"
#include "rendr/glw.h"
#include "rendr/model_storage.h"
#include "rendr/primitives.h"
#include "rendr/types.h"
#include "rendr/constants.h"
#include <GL/gl.h>
#include <iostream>
#include <print>


namespace rendr {

void create_pipeline(const program& p ) {
    shader fs = shader("../assets/fragment.glsl", ShaderType::Fragment);
    shader vs = shader("../assets/vertex.glsl", ShaderType::Vertex);
    p.attach(fs);
    p.attach(vs);
    p.link();
    p.detach(fs);
    p.detach(vs);
}

system::system() {
    create_pipeline(program_);
    set_bindings();
    specify_attributes();
    load_geom();
    set_initial_state();
};

system::~system() {};

void system::wireframe(const bool b) {
    glPolygonMode(GL_FRONT_AND_BACK, b ? GL_LINE : GL_FILL);
}

void system::clear() const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}

// TODO: change when allocation logic is changed
mesh_id system::add_mesh(const geometry& geom) {
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
void system::draw() {
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, mdi_.size(), 0);
};

object system::add_instance(const mesh_id id, const offset_t& off, const color_t& col, const rotation_t& rot, const scale_t& scale) {
    // add checks 
    auto& cmd = mdi_.data()[id];
    auto ins_id = id * kInstanceCapacity+ cmd.instance_count;
    models_.offsets_.data()[ins_id] = off;
    models_.colors_.data()[ins_id] = col;
    models_.rotations_.data()[ins_id] = rot;
    models_.scales_.data()[ins_id] = scale;
    cmd.instance_count++;
    return {.id_ = ins_id};
}

object system::add_instance(const mesh_id id, const glm::vec3& off, const color_t& col, const rotation_t& rot, const scale_t& scale) {
    // add checks 
    auto& cmd = static_cast<draw_command*>(mdi_.data())[id];
    auto ins_id = id * kInstanceCapacity+ cmd.instance_count;
    models_.offsets_.data()[ins_id] = {off,1};
    models_.colors_.data()[ins_id] = col;
    models_.rotations_.data()[ins_id] = rot;
    models_.scales_.data()[ins_id] = scale;
    cmd.instance_count++;
    return {.id_ = ins_id};
}

void system::update_camera(const camera& cam) {
    program_.set_umat4f("view", compute_view(cam));
}

void system::load_geom() {
    add_mesh(load_triangle());
    add_mesh(load_cube());
    add_mesh(load_quad());
}

void system::update_rotations(const mesh_id id, const std::vector<rotation_t>& rots) const {
    assert(rots.size() <= kInstanceCapacity && "instance capacity reached, resize needed");
    auto off = id * kInstanceCapacity;
    host_to_device((rotation_t*)models_.rotations_.data()+off, rots);
}

// TODO: track added objs if remove is enabled, find obj with id = last and set id = obj.id_;
// void system::remove_instance(const object& obj) {
//     // add checks
//     auto mesh_id = obj.id_ / models_->capacity_;
//     auto off = static_cast<offset_t*>(models_->offsets_.data());
//     auto& cmd = static_cast<draw_command*>(mdi_->data())[mesh_id];
//     auto last = mesh_id * models_->capacity_ + cmd.instance_count - 1;
//     off[obj.id_] = off[last];
//     cmd.instance_count--;
// }

// TODO: check if instance alive
// offset with mesh id, if not specified update all
void system::update_colors(const mesh_id id, const std::vector<color_t>& colors) const {
    assert(colors.size() <= kInstanceCapacity && "instance capacity reached, resize needed");
    auto off = id * kInstanceCapacity; 
    host_to_device((color_t*)models_.colors_.data()+off, colors);
}

void system::update_offsets(const mesh_id id, const std::vector<offset_t>& offsets) const {
    assert(offsets.size() <= kInstanceCapacity && "instance capacity reached, resize needed");
    auto off = id * kInstanceCapacity; 
    host_to_device(models_.offsets_.data()+off, offsets);
}


void system::set_initial_state() {
    program_.use();
    program_.set_umat4f("proj",glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10000.f));
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glBindVertexArray(meshes_.attributes_.id_);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, mdi_.id());
    update_camera({});
}

void system::set_attr(const object& obj, const object_desc& desc) {
    models_.offsets_.data()[obj.id_] = {desc.position_, 1};
    models_.colors_.data()[obj.id_] = desc.color_;
    auto s = desc.uniform_scale_;
    models_.scales_.data()[obj.id_] = glm::scale(glm::mat4{1}, {s,s,s});
}

void system::set_bindings() {
    auto& off = models_.offsets_;
    auto& col = models_.colors_;
    auto& rot = models_.rotations_;
    auto& scale = models_.scales_;

    off.binding_loc(0);
    col.binding_loc(1);
    rot.binding_loc(2);
    scale.binding_loc(3);
}

void system::specify_attributes() {
    auto& attr = meshes_.attributes_;
    attr.set_vertex_buff(meshes_.vertices_, 0, sizeof(position_t));
    attr.set_element_buff(meshes_.indices_);

    vertex_array::attribute geom = {
        .location_ = 0,
        .size_ = 3 ,
        .stride_ = 0
    };

    attr.set(geom);
}

}  // namespace rendD
