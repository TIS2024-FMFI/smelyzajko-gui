#pragma once

#include <filesystem>
#include "widgets/Element.h"
#include "GraphicModule.h"

#include "vector"
#include "string"


/**
 * @class Template
 * @brief Template class represents a template that can be loaded, saved, and modified.
 */

class Template {
private:
    std::vector<Element *> elements;
    std::vector<GraphicModule*> graphicModules;
    std::string name;
    bool configurationMode = true;
    ImVec2 resolution;
public:
    Template() = default;
    Template(const std::filesystem::path& filePath, bool configurationMode) : configurationMode(configurationMode) {
        loadTemplate(filePath);
    }

    void addElement(Element* element);
    void removeElement(int index);
    void clear();

    std::string getName() const;
    std::vector<Element *> &getElements();

    void loadTemplate(const std::filesystem::path& filePath);
    void saveTemplate(const std::filesystem::path& filePath, std::string newName = "");

    nlohmann::json to_json() const;
    void from_json(const nlohmann::json& j);

    void addModule(GraphicModule *module);
    std::vector<GraphicModule *> getModules();
};

