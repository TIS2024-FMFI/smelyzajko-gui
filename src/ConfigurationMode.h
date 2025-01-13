#pragma once
#include "GUI.h"
#include "TemplateManager.h"
#include "ToastNotificationManager.h"
#include <algorithm>
#include "Module.h"
#include "ModuleManager.h"
#include "ImGuiFileDialog.h"
#include "yaml-cpp/yaml.h"

class ConfigurationMode : GUI {
public:
    ConfigurationMode(YAML::Node config_file) : io(ImGui::GetIO()) {
        std::vector<std::string> templateNames;

        if (config_file["templates"]) {
            for (const auto& templateNode : config_file["templates"]) {
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
    }

    ModuleManager moduleManager;
    ToastNotificationManager toastManager;
    TemplateManager templateManager;
    ImGuiIO& io;

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
private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
    float menuBarHeight;

    const float minGridValue = 10.0f;
    const float maxGridValue = 1000.0f;
};

