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

    ConfigurationMode(YAML::Node configFile) : GUI(configFile) {
        std::vector<std::string> templateNames;

        if (configFile["templates"]) {
            for (const auto& templateNode : configFile["templates"]) {
                std::string templateName = templateNode.as<std::string>();
                templateNames.push_back(templateName);
            }
        } else {
            std::cerr << "No templates found in config file." << std::endl;
        }

        if (templateNames.empty()) {
            templateManager = TemplateManager();
        } else {
            templateManager = TemplateManager(templateNames);
        }
    };

    TemplateManager templateManager;

    int run() override;

    void addElementToActiveTemplate(Element* element);

    void drawElements();
    void drawElementsWithSnappingOn();
    void setupMenuBar();
    void drawGrid() const;
    void bringElementToTop(Element* element);
    //void renderSettingsPopup(Module& module, const std::string& part);
    //void renderSettingsPopupForModule(const std::string& moduleName, const std::string& jsonFilePath);
    //void renderConfigurationMenu(const std::string& jsonFilePath);

    void createFloatSliderSettings();
    void createIntSliderSettings();
    void createLabelSettings();
    void setupShortcuts();
    void processShortcuts();
    void initializeWindow(GLFWwindow* window);
    void initializeModules();

private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
    float menuBarHeight;

    const float minGridValue = 10.0f;
    const float maxGridValue = 1000.0f;


};