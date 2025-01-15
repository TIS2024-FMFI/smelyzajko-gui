#pragma once
#include "GUI.h"
#include <algorithm>
#include "Module.h"

class OperatingMode :  GUI {
public:

    OperatingMode(YAML::Node configFile) : GUI(configFile) {
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


