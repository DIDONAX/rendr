#pragma once

#include "rendr/context.h"
#include "rendr/types.h"
#include "rendr/window.h"
#include "rendr/camera.h"

namespace rendr {

class editor {
    public:
        editor();
        ~editor();

        bool running() const;
        void display();
        context* ctx();
        camera& default_camera();

    private:
        window* window_;
        context* ctx_;
        rendr::camera camera_;

        void sync_frame_buffer();
};

}
