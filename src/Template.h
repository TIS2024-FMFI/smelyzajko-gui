#pragma once
#include <filesystem>
#include "widgets/Element.h"
#include "GraphicModule.h"

#include "vector"
#include "string"

class Template {
private:
    std::vector<Element *> elements;
    std::vector<GraphicModule*> graphicModules;
    std::string name;
public:
    Template() = default;
    Template(const std::filesystem::path& filePath) {
        loadTemplate(filePath);
    }

    void addElement(Element* element);
    void clear();

    std::string getName() const;
    std::vector<Element *> getElements();

    void loadTemplate(const std::filesystem::path& filePath);
    void saveTemplate(const std::filesystem::path& filePath) const;


    nlohmann::json to_json() const;
    void from_json(const nlohmann::json& j);

    void addModule(GraphicModule *module);
    std::vector<GraphicModule *> getModules();
};

