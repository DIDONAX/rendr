#include "rendr/system.h"
#include "rendr/primitives.h"
#include "rendr/types.h"


namespace rendr {

constexpr auto map_flags = Flags::Write | Flags::Persistent | Flags::Coherent;

system::system() {
    raster_program_ = new shader_program(vertex_shader, fragment_shader);
    meshes_ = new mesh_storage{};
    models_ = new model_storage{};
    mdi_ = new shader_storage{};

    allocate_resources();
    specify_attributes();
    load_geom();
    set_initial_state();
};

system::~system() {
    delete raster_program_;
    delete meshes_;
    delete models_;
    delete mdi_;
};

void system::wireframe(const bool b) {
    glPolygonMode(GL_FRONT_AND_BACK, b ? GL_LINE : GL_FILL);
}

void system::clear() const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}

// TODO: change when allocation logic is changed
mesh_id system::add_mesh(const geometry& geom) {
    // add checks
    auto& size = meshes_->info_.size_;
    auto& cmd = static_cast<draw_command*>(mdi_->data())[size];
    cmd = { 
        .count_ = static_cast<uint>(geom.indices_.size()),
        .instance_count = 0,
        .first_index_ = static_cast<uint>(meshes_->info_.ind_size_), 
        .base_vertex_ = static_cast<int>(meshes_->info_.vert_size_),
        // assumes same slot size per mesh type
        .base_instance_ = static_cast<uint>(size*models_->capacity_), 
    };
    meshes_->add(geom);
    return size-1;
}

// TODO: add fencing and triple ring buffer
void system::draw() {
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, meshes_->info_.size_, 0);
};

object system::add_instance(const mesh_id id, const offset_t& off, const color_t& col, const rotation_t& rot, const scale_t& scale) {
    // add checks 
    auto& cmd = static_cast<draw_command*>(mdi_->data())[id];
    auto ins_id = id * models_->capacity_ + cmd.instance_count;
    static_cast<offset_t*>(models_->offsets_.data())[ins_id] = off;
    static_cast<color_t*>(models_->colors_.data())[ins_id] = col;
    static_cast<rotation_t*>(models_->rotations_.data())[ins_id] = rot;
    static_cast<scale_t*>(models_->scales_.data())[ins_id] = scale;
    cmd.instance_count++;
    return {.id_ = ins_id};
}

object system::add_instance(const mesh_id id, const glm::vec3& off, const color_t& col, const rotation_t& rot, const scale_t& scale) {
    // add checks 
    auto& cmd = static_cast<draw_command*>(mdi_->data())[id];
    auto ins_id = id * models_->capacity_ + cmd.instance_count;
    static_cast<offset_t*>(models_->offsets_.data())[ins_id] = {off,1};
    static_cast<color_t*>(models_->colors_.data())[ins_id] = col;
    static_cast<rotation_t*>(models_->rotations_.data())[ins_id] = rot;
    static_cast<scale_t*>(models_->scales_.data())[ins_id] = scale;
    cmd.instance_count++;
    return {.id_ = ins_id};
}

void system::update_camera(const camera& cam) {
    raster_program_->update_view(compute_view(cam));
}

void system::load_geom() {
    add_mesh(load_triangle());
    add_mesh(load_cube());
    add_mesh(load_sphere());
    // add_mesh(load_cylinder());
}

void system::update_rotations(const mesh_id id, const std::vector<rotation_t>& rots) const {
    assert(rots.size() <= models_->capacity_ && "instance capacity reached, resize needed");
    auto off = id * models_->capacity_;
    host_to_device((rotation_t*)models_->rotations_.data()+off, rots);
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
    assert(colors.size() <= models_->capacity_ && "instance capacity reached, resize needed");
    auto off = id * models_->capacity_; 
    host_to_device((color_t*)models_->colors_.data()+off, colors);
}

void system::update_offsets(const mesh_id id, const std::vector<offset_t>& offsets) const {
    assert(offsets.size() <= models_->capacity_ && "instance capacity reached, resize needed");
    auto off = id * models_->capacity_; 
    host_to_device((offset_t*)models_->offsets_.data()+off, offsets);
}

void system::update_global_cols(const std::vector<color_t>& colors) const {
    assert(colors.size() <= models_->capacity_ * meshes_->info_.capacity_ && "instance capacity reached, resize needed");
    host_to_device((color_t*)models_->offsets_.data(), colors);
}

void system::update_global_offs(const std::vector<offset_t>& offsets) const {
    assert(offsets.size() <= models_->capacity_ * meshes_->info_.capacity_ && "instance capacity reached, resize needed");
    host_to_device((offset_t*)models_->offsets_.data(), offsets);
} 

void system::set_initial_state() {
    raster_program_->use();
    auto proj = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10000.f);
    raster_program_->update_proj(proj);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glBindVertexArray(meshes_->attributes_.id_);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, mdi_->id_);
    camera cam;
    update_camera(cam);
}

void system::set_attr(const object& obj, const object_desc& desc) {
    static_cast<offset_t*>(models_->offsets_.data())[obj.id_] = {desc.position_, 1};
    static_cast<color_t*>(models_->colors_.data())[obj.id_] = desc.color_;
    auto s = desc.uniform_scale_;
    static_cast<scale_t*>(models_->scales_.data())[obj.id_] = glm::scale(glm::mat4{1}, {s,s,s});
}

// TODO: refractor into a resizable buffer/vector class, removes capacity bs
// vector<T, F> 
void system::allocate_resources() {
    auto& vert = meshes_->vertices_;
    auto& ind = meshes_->indices_;

    auto& off = models_->offsets_;
    auto& col = models_->colors_;
    auto& rot = models_->rotations_;
    auto& scale = models_->scales_;

    mdi_->alloc(nullptr, meshes_->info_.capacity_* sizeof(draw_command), map_flags | Static);
    vert.alloc(nullptr, meshes_->info_.vert_capacity_ * sizeof(position_t), map_flags | Dynamic);
    ind.alloc(nullptr, meshes_->info_.ind_capacity_ * sizeof(index_t), map_flags | Dynamic);
    vert.map(map_flags);
    ind.map(map_flags);
    mdi_->map(map_flags);

    off.alloc(nullptr, meshes_->info_.capacity_ * models_->capacity_ * sizeof(offset_t), map_flags | Dynamic);
    col.alloc(nullptr, meshes_->info_.capacity_ * models_->capacity_ * sizeof(color_t), map_flags | Dynamic);
    rot.alloc(nullptr, meshes_->info_.capacity_ * models_->capacity_ * sizeof(rotation_t), map_flags | Dynamic);
    scale.alloc(nullptr, meshes_->info_.capacity_ * models_->capacity_ * sizeof(scale_t), map_flags | Dynamic);
    off.map(map_flags);
    col.map(map_flags);
    rot.map(map_flags);
    scale.map(map_flags);

    off.binding_loc(0);
    col.binding_loc(1);
    rot.binding_loc(2);
    scale.binding_loc(3);
}

void system::specify_attributes() {
    auto& attr = meshes_->attributes_;
    attr.bind_vertex_buff(meshes_->vertices_, 0, sizeof(position_t));
    attr.bind_element_buff(meshes_->indices_);

    attr.enable_attrib(0);
    attr.format_attrib(0, 3, 0);
    attr.bind_attrib(0, 0);
}

}  // namespace rendD
