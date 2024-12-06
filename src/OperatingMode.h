#pragma once
#include "GUI.h"
#include "TemplateHandler.h"
#include <algorithm>
#include "Module.h"
class OperatingMode :  GUI {
public:
    OperatingMode() : io(ImGui::GetIO()) {}

    std::vector<Element *> activeElements;
    TemplateHandler templateHandler;
    ImGuiIO& io;
    int run() override;
    void drawElements();
    void setupMenuBar();
    void renderSettingsPopup(Module& module, const std::string& part);


private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;

    void bringElementToTop(Element *element);
};


