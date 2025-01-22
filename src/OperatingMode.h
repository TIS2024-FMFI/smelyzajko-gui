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
        std::cout<<templateNames.size()<<std::endl;
        if (templateNames.empty()) {
            templateManager = TemplateManager(false);
        } else {
            templateManager = TemplateManager(templateNames, false);
        }
        std::cout << "OperatingMode constructor" << std::endl;

        setupShortcuts();

        templateManager.setActiveTemplate(templateManager.getAllTemplates().front());
        std::string activeTemplateName = templateManager.getActiveTemplateName();
        std::string windowTitle = std::string("GUI") + " - " + activeTemplateName;
        glfwSetWindowTitle(window, windowTitle.c_str());
    };

    int run() override;
    void drawElements();
    void drawMenuBar();
    void setupShortcuts() override;
    TemplateManager templateManager;

private:
    int currentTemplateIndex = 0;

    void switchTemplate(int direction);

};


