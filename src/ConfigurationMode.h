#pragma once
#include "GUI.h"
#include "TemplateHandler.h"
#include <algorithm>
#include "Module.h"
#include "ModuleManager.h"

class ConfigurationMode : GUI {
public:
    ModuleManager moduleManager;

    ConfigurationMode() : io(ImGui::GetIO()) {}

    std::vector<Element *> activeElements;
    TemplateHandler templateHandler;
    ImGuiIO& io;

    int run() override;

    void drawElements();
    void setupMenuBar();
    void drawGrid() const;
    void bringElementToTop(Element* element);
    void renderSettingsPopup(Module& module, const std::string& part);

    void createFloatSliderSettings();
    void createIntSliderSettings();
    void createLabelSettings();
private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
};


