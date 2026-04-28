#include "rendr/context.h"
#include "rendr/load.h"
#include "editor.h"


using namespace rendr;
int main() {
    editor e;
    auto ctx = e.ctx();

    auto geom =load_obj("assets/teapot.obj");
    auto bb  = compute_bbox(geom);
    auto mesh_id = ctx->add_mesh(geom);
    ctx->add_instance(mesh_id);

    e.default_camera().target_ = bb.center_;
    e.default_camera().position_.y += 5.f;

    while (e.running()) {
        // do stuff with opengl context

        ctx->clear();
        ctx->draw();
        e.display();
    }
}
