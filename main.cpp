#include <string>
#include <vector>

//#include "src/ConfigurationMode.h"

//enum ModuleElementType {
//    TEXT, GRAPHIC, OTHER
//};
//
//class ModuleElement {
//public:
//    std::string name;
//    ModuleElementType type;
//
//    float defaultWidth = 0;
//    float defaultHeight = 0;
//
//    ModuleElement(std::string name, ModuleElementType type) {
//        this->name = name;
//        this->type = type;
//        if (this->type == GRAPHIC) {
//            this->defaultHeight = 200;
//            this->defaultWidth = 200;
//        }
//    }
//
//    void placeElementInTemplate() {
//        ImGui::Begin(this->name.c_str());
//        if (this->type == TEXT) {
//            char inputText[256] = "";  // buffer
//            ImVec2 availableRegion = ImGui::GetContentRegionAvail();
//            ImGui::InputTextMultiline("##label", inputText, IM_ARRAYSIZE(inputText), availableRegion);
//        } else if (this->type == GRAPHIC) {
//            ImDrawList* draw_list = ImGui::GetWindowDrawList();
//            ImVec2 p = ImGui::GetCursorScreenPos();
//            ImVec2 size = ImVec2(defaultWidth, defaultHeight);
//            ImU32 color = IM_COL32(255, 0, 0, 255);
//            draw_list->AddRect(p, ImVec2(p.x + size.x, p.y + size.y), color);
//            static bool boolean = false;
//            ImGui::Checkbox("checkbox", &boolean);
//            static int input = 0;
//            ImGui::InputInt("input int", &input);
//        } else {
//
//        }
//        ImGui::End();
//    }
//};
//
//class Module {
//public:
//    std::string name;
//    std::vector<ModuleElement> moduleElements;
//
//    Module(std::string name, std::vector<ModuleElement> moduleElements) {
//        this->name = name;
//        this->moduleElements = moduleElements;
//    }
//};
//
//std::vector<ModuleElement> moduleElements1 = { ModuleElement("1. textovy prvok", TEXT),  ModuleElement("2. graficky prvok", GRAPHIC)};
//std::vector<ModuleElement> moduleElements2 = { ModuleElement("3. graficky prvok", GRAPHIC), ModuleElement("4. graficky prvok", GRAPHIC)};
//std::vector<ModuleElement> moduleElements3 = { ModuleElement("5. textovy prvok", TEXT),  ModuleElement("6. graficky prvok", GRAPHIC)};
//
//std::vector<Module> modules = { Module("1. modul", moduleElements1),
//                                Module("2. modul", moduleElements2),
//                                Module("3. modul", moduleElements3) };
//
//std::vector<ModuleElement> selectedElements;
//
//void createModuleMenuitem() {
//    if (ImGui::BeginMenu("Modules")) {
//        for (const Module& module : modules ) {
//            if (ImGui::BeginMenu(module.name.c_str())) {
//                for (ModuleElement moduleElement : module.moduleElements) {
//                    if (ImGui::MenuItem(moduleElement.name.c_str())) {
//                        selectedElements.push_back(moduleElement);
//                    }
//                }
//                ImGui::EndMenu();
//            }
//        }
//        ImGui::EndMenu();
//    }
//}


int main() {
    //ConfigurationMode gui;

    gui.run();
}


//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//#include <GLFW/glfw3.h>
//#include <vector>
//#include <string>
//#include "iostream"
//// Structure to represent a resizable and movable rectangle
//struct Rectangle {
//    ImVec2 position;
//    ImVec2 size;
//    std::string label;
//};
//
//int main() {
//    // Initialize GLFW
//    if (!glfwInit())
//        return -1;
//
//    // Set OpenGL version (optional)
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create a GLFW window
//    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Example", nullptr, nullptr);
//    if (!window) {
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSwapInterval(1); // Enable vsync
//
//    // Initialize ImGui
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//
//    // Initialize ImGui backend
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 330");
//
//    // Style
//    ImGui::StyleColorsDark();
//
//    // Vector of rectangles
//    std::vector<Rectangle> rectangles = {
//            {{50, 50}, {100, 50}, "Rectangle 1"},
//            {{200, 100}, {150, 75}, "Rectangle 2"},
//    };
//
//    // Main loop
//    while (!glfwWindowShouldClose(window)) {
//        // Poll events
//        glfwPollEvents();
//
//        // Start new ImGui frame
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        ImGui::SetNextWindowPos(ImVec2(0, 0)); // Top-left corner of the screen
//        ImGui::SetNextWindowSize(io.DisplaySize); // Fullscreen size
//
//        ImGui::Begin("Main Window", nullptr,
//                     ImGuiWindowFlags_NoTitleBar |    // Remove title bar
//                     ImGuiWindowFlags_NoCollapse |   // Prevent collapsing
//                     ImGuiWindowFlags_NoResize |     // Disable resizing
//                     ImGuiWindowFlags_NoMove |       // Prevent moving the window
//                     ImGuiWindowFlags_NoBringToFrontOnFocus | // Prevent window focus changes
//                     ImGuiWindowFlags_NoScrollbar    // Disable scrollbar (optional)
//        );
//
//        // Moveable text
//        static ImVec2 text_position = {300, 300};
//        static float margin = 10.0f; // Margin size
//
//// Define the size of the text
//        ImVec2 text_size = ImGui::CalcTextSize("Moveable Text");
//
//// Define the area for interaction (text position plus margin)
//        ImVec2 hover_area_size = ImVec2(text_size.x + 2 * margin, text_size.y + 2 * margin);
//
//// Draw the invisible button for the hover area
//        ImGui::SetCursorScreenPos(ImVec2(text_position.x - margin, text_position.y - margin));
//        ImGui::InvisibleButton("MoveableText", hover_area_size);
//
//// Allow movement if the hover area is active and the mouse is dragging
//        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
//            ImVec2 delta = io.MouseDelta;
//            text_position.x += delta.x;
//            text_position.y += delta.y;
//        }
//
//// Draw the text at the updated position
//        ImGui::SetCursorScreenPos(text_position);
//        ImGui::Text("Text text text");
//
//        // Render and handle rectangles
//        for (size_t i = 0; i < rectangles.size(); ++i) {
//            Rectangle& rect = rectangles[i];
//
//            // Define the resizing handle (bottom-right corner)
//            ImVec2 resize_handle_pos = ImVec2(rect.position.x + rect.size.x, rect.position.y + rect.size.y);
//            ImVec2 handle_size = ImVec2(10, 10); // Size of the resizing handle
//
//            // Draw and check interaction for the resizing handle
//            ImGui::SetCursorScreenPos(ImVec2(resize_handle_pos.x - handle_size.x, resize_handle_pos.y - handle_size.y));
//            ImGui::InvisibleButton(("ResizeHandle" + std::to_string(i)).c_str(), handle_size);
//
//            bool resizing = false; // Track if resizing is active
//            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
//                ImVec2 delta = io.MouseDelta;
//                rect.size.x += delta.x;
//                rect.size.y += delta.y;
//
//                // Clamp the rectangle size to prevent collapsing
//                rect.size.x = std::max(rect.size.x, 20.0f); // Minimum width
//                rect.size.y = std::max(rect.size.y, 20.0f); // Minimum height
//
//                resizing = true; // Mark resizing as active
//            }
//
//            // Only allow moving the rectangle if resizing is NOT active
//            if (!resizing) {
//                // Draw and check interaction for the rectangle
//                ImGui::SetCursorPos(rect.position);
//                ImGui::InvisibleButton(("Rect" + std::to_string(i)).c_str(), rect.size);
//
//                // Moving the rectangle
//                if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
//                    ImVec2 delta = io.MouseDelta;
//                    rect.position.x += delta.x;
//                    rect.position.y += delta.y;
//                }
//            }
//
//            // Draw the rectangle outline
//            ImDrawList* draw_list = ImGui::GetWindowDrawList();
//            ImVec2 rect_min = ImVec2(rect.position.x + ImGui::GetWindowPos().x,
//                                     rect.position.y + ImGui::GetWindowPos().y);
//            ImVec2 rect_max = ImVec2(rect.position.x + rect.size.x + ImGui::GetWindowPos().x,
//                                     rect.position.y + rect.size.y + ImGui::GetWindowPos().y);
//
//            draw_list->AddRect(rect_min, rect_max, IM_COL32(255, 255, 255, 255)); // White rectangle outline
//
//            // Optional: Draw the resizing handle for debugging (visual aid)
//            ImVec2 handle_screen_pos_min = ImVec2(resize_handle_pos.x + ImGui::GetWindowPos().x - handle_size.x,
//                                                  resize_handle_pos.y + ImGui::GetWindowPos().y - handle_size.y);
//            ImVec2 handle_screen_pos_max = ImVec2(resize_handle_pos.x + ImGui::GetWindowPos().x,
//                                                  resize_handle_pos.y + ImGui::GetWindowPos().y);
//            draw_list->AddRectFilled(handle_screen_pos_min, handle_screen_pos_max, IM_COL32(255, 0, 0, 255)); // Red handle
//        }
//
//        ImGui::End();
//
//        // Render
//        ImGui::Render();
//        glClear(GL_COLOR_BUFFER_BIT);
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        // Swap buffers
//        glfwSwapBuffers(window);
//    }
//
//    // Cleanup
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//    glfwDestroyWindow(window);
//    glfwTerminate();
//
//    return 0;
//}