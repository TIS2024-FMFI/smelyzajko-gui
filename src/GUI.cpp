#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "GUI.h"

GUI::GUI() {
    if (!glfwInit()) {
        throw std::runtime_error("GLFW initialization error");
    }
    primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

    monitorWidth = videoMode->width;
    monitorHeight = videoMode->height;

    window = glfwCreateWindow(monitorWidth, monitorHeight, "GUI", nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("Error at window creation");
    }

    glfwMakeContextCurrent(window);

    setupImGui();
}

void GUI::setupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void GUI::cleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
