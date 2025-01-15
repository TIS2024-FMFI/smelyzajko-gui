#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "vector"
#include "yaml-cpp/yaml.h"

#include "ModuleManager.h"
#include "ShortcutsManager.h"
#include "ToastNotificationManager.h"
#include "ShortcutsManager.h"
#include "TemplateManager.h"

class GUI {
protected:
    ImGuiIO& io;
    int monitorWidth, monitorHeight;
    GLFWmonitor* primaryMonitor;
    GLFWwindow* window;

    ModuleManager moduleManager;
    ToastNotificationManager toastManager;
    ShortcutsManager shortcutsManager;
    YAML::Node configFile;
public:
    GUI(YAML::Node configFile);

    void setupImGui();
    void cleanupImGui();

    virtual int run() = 0;
};

