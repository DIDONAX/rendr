#include "rendr/system.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include "rendr/glw.h"
#include "rendr/window.h"
#include <cassert>
#include <cstring>

namespace rendr {

system::system() {
    glfwInit();

    window_ = new window();
    program_ = new shader_programm(VS_SRC, FS_SRC);
    meshes_ = new mesh_storage();
    models_ = new model_storage();
    indirect_buffer_ = new vertex_buffer();

    allocate_resources();
    specify_attributes();

    meshes_->geom_ = {
        .vertices_ = {{-0.005, 0, 0}, {0.005, 0, 0}, {0, 0.005, 0}},
        .indices_ = {0, 1, 2}
    };
    commands_.push_back({
        .count_ = 3,
        .instance_count = 0,
        .first_index_ = 0,
        .base_vertex_ = 0,
        .base_instance_ = 0
    });

    upload_geom();
    upload_batch();
    set_initial_state();
};

system::~system() {
    delete window_;
    delete program_;
    delete meshes_;
    delete models_;
    delete indirect_buffer_;
    glfwTerminate();
};

void system::update_cam() {
    // cam_.process_keyboard(window_->handle());
    // cam_.process_mouse(window_->handle());
    // auto view = cam_.view();
    // auto proj = cam_.proj();
    // program_->update_view(glm::value_ptr(view));
    // program_->update_proj(glm::value_ptr(proj));
}

void system::draw() const {
    // this becomes slow when 10M > triangles
    // std::memcpy(models_->offset_buff_.data(), models_->offsets_.data(), models_->offsets_.size() * sizeof(offset_t));
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, (GLsizei)commands_.size(), 0);
};

instance_id system::add_instance(const mesh_id id, const offset_t off = {}) {
    // auto count = commands_[id].instance_count + 1;
    // auto capacity = models_->capacity_;
    // assert(count <= capacity && "instance capacity reached");
    // assert(id < commands_.size() && "invalid mesh_id");

    auto ins_id = id * models_->capacity_ + commands_[id].instance_count;
    models_->offsets_[ins_id] = off;
    commands_[id].instance_count++;
    return ins_id;
}

// TODO: swapping desyncs ids, find better approach
// void system::remove_instance(const instance_id id) {
//     assert(id < models_->offsets.size() && "model_id does not exist");
//     auto mesh_id = id;
//     while(mesh_id >= models_->capacity_) {
//         mesh_id--;
//     }
//
//     mesh_id = id - mesh_id;
//     auto& cmd = commands_[mesh_id];
//     auto last = cmd.instance_count - 1;
//     models_->offsets_[id] = models_->offsets_[last];
//     cmd.instance_count--;
// }

void system::update_instance(const instance_id id, const offset_t off) {
    // models_->offsets_[id] = off;
    static_cast<offset_t*>(models_->offset_buff_.data())[id] = off;
}

void system::set_initial_state() {
    program_->use(); 
    glBindVertexArray(meshes_->attributes_.id_);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, models_->offset_buff_.id_);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer_->id_);
}

void system::allocate_resources() {
    auto& vert = meshes_->vertices_;
    auto& ind = meshes_->indices_;
    auto& off = models_->offset_buff_;

    indirect_buffer_->alloc(nullptr, meshes_->info_.capacity_* sizeof(draw_command), Flags::Dynamic);
    vert.alloc(nullptr, meshes_->info_.vert_capacity_ * sizeof(position_t), sf);
    ind.alloc(nullptr, meshes_->info_.ind_capacity_ * sizeof(index_t), sf);

    off.alloc(nullptr, meshes_->info_.capacity_ * models_->capacity_ * sizeof(offset_t), sf);
    vert.map(mf);
    ind.map(mf);
    off.map(mf);
    models_->offsets_.resize(models_->capacity_ * meshes_->info_.capacity_);
}

void system::specify_attributes() {
    auto& attr = meshes_->attributes_;
    attr.bind_vertex_buff(meshes_->vertices_, 0, sizeof(position_t));
    attr.bind_element_buff(meshes_->indices_);
    attr.enable_attrib(0);
    attr.format_attrib(0, 3, 0);
    attr.bind_attrib(0, 0);
}

// resize check handled by the caller
void system::upload_geom() {
    auto& vert = meshes_->vertices_;
    auto& ind = meshes_->indices_;
    auto& geomv = meshes_->geom_.vertices_;
    auto& geomi = meshes_->geom_.indices_;
    std::memcpy(vert.data(), geomv.data(), geomv.size() * sizeof(position_t));
    std::memcpy(ind.data(), geomi.data(), geomi.size() * sizeof(index_t));
}

// resize check handled by the caller
void system::upload_batch() {
    glNamedBufferSubData(indirect_buffer_->id_, 0, commands_.size() * sizeof(draw_command), commands_.data());
}

}  // namespace rendr
