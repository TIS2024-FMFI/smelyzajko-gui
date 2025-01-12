#pragma once

#include <filesystem>
#include "widgets/Element.h"
#include "vector"
#include "string"

class Template {
private:
    std::vector<Element *> elements;
    std::string name;
public:
    Template() = default;
    Template(const std::filesystem::path& filePath) {
        loadTemplate(filePath);
    }

    void addElement(Element* element);
    void removeElement(int index);
    void clear();

    std::string getName() const;
    std::vector<Element *> getElements();

    void loadTemplate(const std::filesystem::path& filePath);
    void saveTemplate(const std::filesystem::path& filePath, std::string newName = "");


    nlohmann::json to_json() const;
    void from_json(const nlohmann::json& j);
};

