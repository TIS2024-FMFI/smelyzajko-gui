#pragma once
#include "GUI.h"
#include <algorithm>
#include "Module.h"
#include "ImGuiFileDialog.h"



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
            templateManager = TemplateManager(true);
        } else {
            templateManager = TemplateManager(templateNames, true);
        }

        setupShortcuts();

        templateManager.setConfigMode(true);
        menuBarHeight = ImGui::GetFrameHeight();

    };

    int run() override;

    void addElementToActiveTemplate(Element* element);

    void drawElements();
    void drawElementsWithSnappingOn();
    void setupMenuBar();
    void drawGrid() const;
    void bringElementToTop(std::vector<Element*>& elements, Element* element);
//    void renderSettingsPopup(Module& module, const std::string& part);
    void createFloatSliderSettings(std::string elementName, std::string moduleName);
    void createIntSliderSettings(std::string elementName, std::string moduleName);
    void createLabelSettings();
    void createButton(std::string elementName, std::string moduleName);
    void createCheckbox(std::string elementName, std::string moduleName);
    void createTextInput(std::string elementName, std::string moduleName);
    void setNewElementAndAddToActiveTemplate(Element *element, std::string elementName, std::string moduleName, ImVec2 position, ImVec2 elementSize);
    void handleElementClick(Element* element,int i);
    void handleClicksOnElements(std::vector<Element*>& elements);
    bool isAnyPendingElement(std::vector<Element*>& elements);
    void setupShortcuts() override;
    void saveTemplate();
    void processFileDialog();
    ImVec2 getPosition(ImVec2 elementSize);

    TemplateManager templateManager = TemplateManager(true);

private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
    float menuBarHeight = 25.0f;

    void addModuleToActiveTemplate(GraphicModule *graphicModule);

    const float minGridValue = 10.0f;
    const float maxGridValue = 1000.0f;
};

