#include <stdexcept>

#include "GUI.h"

GUI::GUI(YAML::Node configFile) : io(ImGui::GetIO()), configFile(configFile) {
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
    setupTemplates();
}


void GUI::setupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));

    ImFontConfig fontConfig;
    fontConfig.RasterizerDensity = 5.0f;

    io.Fonts->AddFontDefault(&fontConfig);
}

void GUI::cleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::setupTemplates() {
    std::vector<std::string> templateNames;

    if (configFile["templates"]) {
        for (const auto& templateNode : configFile["templates"]) {
            std::string templateName = templateNode.as<std::string>();
            templateNames.push_back(templateName);
        }
    } else {
        std::cerr << "No templates found in config file." << std::endl;
    }

    if (templateNames.empty()) {
        templateManager = TemplateManager();
    } else {
        templateManager = TemplateManager(templateNames);
    }
}



