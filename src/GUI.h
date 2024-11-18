#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class GUI {

public:
    int monitorWidth, monitorHeight;
    GLFWmonitor* primaryMonitor;
    GLFWwindow* window;

    GUI();

    void setupImGui();
    void cleanupImGui();
    virtual int run() = 0;
};

