#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "vector"

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

#include "ModuleManager.h"
#include "ShortcutsManager.h"
#include "ToastNotificationManager.h"
#include "ShortcutsManager.h"
#include "TemplateManager.h"

#include "widgets/Element.h"
#include "widgets/Rectangle.h"
#include "widgets/Checkbox.h"
#include "widgets/Button.h"
#include "widgets/HorizontalSlider.h"
#include "widgets/VerticalSlider.h"
#include "widgets/SingleLineLabel.h"
#include "widgets/MultiLineLabel.h"
#include "widgets/TextInput.h"
#include "TextArea.h"


/* Here include all Modules header files */
#include "../TestModules/UltrasonicModule.h"
#include "../TestModules/MapModule.h"
#include "../TestModules/CounterModule.h"

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
    virtual void setupShortcuts() = 0;

    void loadModules(YAML::Node modules);
};