#pragma once
#include "GUI.h"
#include <algorithm>
#include "Module.h"
#include "ImGuiFileDialog.h"

template <typename T>
struct SliderSettings {
    char label[128]{};
    T minValue = 0;
    T maxValue = 10;
    T initialValue = 5;
    bool isHorizontal = true;

    SliderSettings() = default;

    SliderSettings(const std::string &labelStr, T min, T max, T init, bool horizontal)
            : minValue(min), maxValue(max), initialValue(init), isHorizontal(horizontal) {
        std::strncpy(label, labelStr.c_str(), sizeof(label) - 1);
        label[sizeof(label) - 1] = '\0';
    }
};

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
    void drawMenuBar();
    void drawGrid() const;
    void bringElementToTop(std::vector<Element*>& elements, Element* element);

    void createLabelSettings();
    void createButton(std::string elementName, std::string moduleName);
    void createCheckbox(std::string elementName, std::string moduleName);
    void createTextInput(std::string elementName, std::string moduleName);

    void handleElementClick(Element* element,int i);
    void handleClicksOnElements(std::vector<Element*>& elements);
    bool isAnyPendingElement(std::vector<Element*>& elements);
    void setupShortcuts() override;
    void saveTemplate();
    void openLogDirectoryDialog();
    void processLogDirectoryDialog();
    void updateLogDirectoryInConfig(const std::string& logDir);

    void processFileDialog();
    ImVec2 getPosition(ImVec2 elementSize);
    TemplateManager templateManager;

private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
    float menuBarHeight = 25.0f;

    void addModuleToActiveTemplate(GraphicModule *graphicModule);

    const float minGridValue = 10.0f;
    const float maxGridValue = 1000.0f;



    template <typename T>
    void createSliderSettings(const std::string &elementName, const std::string &moduleName, bool horizontal);
};

