#include "src/Replay/ReplayMode.h"


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

void setupImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void cleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    ReplayMode gui;

    gui.run();
}
