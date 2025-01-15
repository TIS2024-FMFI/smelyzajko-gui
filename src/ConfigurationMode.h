#pragma once
#include "GUI.h"
#include <algorithm>
#include "Module.h"
#include "ImGuiFileDialog.h"
#include "ModuleManager.h"

#include "widgets/Element.h"
#include "widgets/Rectangle.h"
#include "widgets/Checkbox.h"
#include "widgets/Button.h"
#include "widgets/Slider.h"
#include "widgets/SingleLineLabel.h"
#include "widgets/MultiLineLabel.h"

class ConfigurationMode : GUI {
public:

    ConfigurationMode(YAML::Node configFile) : GUI(configFile) {};

    int run() override;

    void addElementToActiveTemplate(Element* element);

    void drawElements();
    void drawElementsWithSnappingOn();
    void setupMenuBar();
    void drawGrid() const;
    void bringElementToTop(Element* element);
    void renderSettingsPopup(Module& module, const std::string& part);

    void createFloatSliderSettings();
    void createIntSliderSettings();
    void createLabelSettings();
    void setupShortcuts();
    void processShortcuts();
    void initializeWindow(GLFWwindow* window);

private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
    float menuBarHeight;

    const float minGridValue = 10.0f;
    const float maxGridValue = 1000.0f;
};

