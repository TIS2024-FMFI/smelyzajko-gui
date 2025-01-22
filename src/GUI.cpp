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
    if (configFile["mode"].as<std::string>() != "--replay"){

        loadModules(configFile["modules"]);
    }
    configFile.remove("mode");
}

void GUI::setupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    io.DisplaySize = ImVec2(static_cast<float>(monitorWidth), static_cast<float>(monitorHeight));

    ImFontConfig fontConfig;
    fontConfig.RasterizerDensity = 5.0f;

    io.Fonts->AddFontDefault(&fontConfig);
}

void GUI::cleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void GUI::loadModules(YAML::Node modules) {
    if (!modules || !modules.IsSequence()) {
        throw std::runtime_error("Invalid modules configuration in the config file.");
    }

    for (const auto& moduleNode : modules) {
        if (!moduleNode.IsMap() || moduleNode.size() != 1) {
            std::cerr << "Invalid module format. Each module should be a map with one key-value pair." << std::endl;
            continue;
        }

        std::string moduleName = moduleNode.begin()->first.as<std::string>();
        YAML::Node moduleParams = moduleNode.begin()->second;

        if (moduleName.empty()) {
            std::cerr << "Empty module name found. Skipping." << std::endl;
            continue;
        }

        Module* module = nullptr;

        if (moduleName == "MapModule") {
            module = new MapModule(&moduleManager);
        } else if (moduleName == "CounterModule") {
            module = new CounterModule(&moduleManager);
        } else if (moduleName == "UltrasonicModule") {
            module = new UltrasonicModule(&moduleManager);
        } else {
            std::cerr << "Unknown module: " << moduleName << std::endl;
            continue;
        }
        moduleManager.registerModule(moduleName, module);
    }
    moduleManager.logSettings(configFile);

}


