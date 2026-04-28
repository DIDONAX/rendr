#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "input.h"
#include "rendr/context.h"
#include "rendr/load.h"
#include "utils.h"
#include "editor.h"

static void on_glfw_error(int err, const char* msg) {
    fprintf(stderr, "GLFW %d: %s\n", err, msg);
}

using namespace rendr::editor;
int main() {
    glfwSetErrorCallback(on_glfw_error);
    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height,
                                                  "Editor", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGL(glfwGetProcAddress)) {
        fprintf(stderr, "GLAD init failed\n");
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.FontGlobalScale = 1.7f;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    rendr::context ctx;
    rendr::camera  cam;

    auto geom = rendr::load_obj("assets/teapot.obj");
    auto mesh_id = ctx.add_mesh(geom);
    ctx.add_instance(mesh_id);

    auto bb  = compute_bbox(geom);
    cam.aspect_ = (float)mode->width / mode->height;
    cam.target_ = bb.center_;
    cam.position_.y += 5.f;
    ctx.update_camera(cam);

    glViewport(0, 0, mode->width, mode->height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);

    editor_state ed;
    float last_frame = 0.f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float now = (float)glfwGetTime();
        ed.delta = now - last_frame;
        ed.fps = ed.delta > 0.f ? 1.f / ed.delta : 0.f;
        last_frame = now;

        handle_input();

        rendr::editor::rotate_cam(cam, 6.f, ed.delta, bb.center_);
        ctx.update_camera(cam);

        ctx.clear();
        ctx.draw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        tick(ed, ctx);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
