#include <stdexcept>

#include "GUI.h"

GUI::GUI() {
    if (!glfwInit()) {
        throw std::runtime_error("GLFW initialization error");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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
    ImGui_ImplOpenGL3_Init("#version 330");


    ImFontConfig fontConfig;
    // higher resolution when rendering text
    fontConfig.RasterizerDensity = 5.0f;

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault(&fontConfig);

     // io.Fonts->AddFontFromFileTTF("../Turbo-Pascal-Font.ttf", 18.0f);
}

void GUI::cleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
