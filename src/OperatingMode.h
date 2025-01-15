#pragma once
#include "GUI.h"
#include "TemplateManager.h"
#include <algorithm>
#include "Module.h"
class OperatingMode :  GUI {
public:
    OperatingMode() : io(ImGui::GetIO()) {}
    ModuleManager moduleManager;

    TemplateManager templateManager = TemplateManager(false);
    ImGuiIO& io;
    int run() override;
    void drawElements();
    void setupMenuBar();
    void renderSettingsPopup(Module& module, const std::string& part);
    void addElementToActiveTemplate(Element *element);
    void bringElementToTop(Element *element);


private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;


};


