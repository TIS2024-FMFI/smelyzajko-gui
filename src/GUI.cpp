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
    loadModules(configFile["modules"]);
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

    for (const auto& moduleName : modules) {
        if (!moduleName.IsScalar()) {
            std::cerr << "Module name must be a scalar value." << std::endl;
            continue;
        }

        std::string moduleNameStr = moduleName.as<std::string>();

        if (moduleNameStr.empty()) {
            std::cerr << "Empty module name found. Skipping." << std::endl;
            continue;
        }

        if (moduleNameStr == "MapModule") {
            auto mapModule = new MapModule(&moduleManager);
            moduleManager.registerModule("MapModule", mapModule);
        } else if (moduleNameStr == "CounterModule") {
            auto counterModule = new CounterModule(&moduleManager);
            moduleManager.registerModule("CounterModule", counterModule);
        } else if (moduleNameStr == "UltrasonicModule") {
            auto ultrasonicModule = new UltrasonicModule(&moduleManager);
            moduleManager.registerModule("UltrasonicModule", ultrasonicModule);
        } else {
            std::cerr << "Unknown module: " << moduleNameStr << std::endl;
        }
    }
}



