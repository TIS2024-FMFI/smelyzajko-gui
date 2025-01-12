#include "Template.h"
#include <fstream>
#include <stdexcept>
#include <../json.hpp>
#include "widgets/Button.h"
#include "widgets/Slider.h"
#include "widgets/Checkbox.h"
#include "widgets/Rectangle.h"
#include "widgets/SingleLineLabel.h"
#include "widgets/MultiLineLabel.h"
#include "iostream"

#include "TemplateManager.h"
#include "ModuleManager.h"
using json = nlohmann::json;

std::vector<Element *> Template::getElements() {
    return elements;
}

void Template::addElement(Element* element) {
    elements.push_back(element);
}
void Template::addModule(GraphicModule* module) {
    graphicModules.push_back(module);
}

void Template::clear() {
    elements.clear();
    graphicModules.clear();
}

nlohmann::json Template::to_json() const {
    json j;
    j["name"] = name;
    j["elements"] = json::array();
    j["graphicModules"] = json::array();

    for (const auto& element : elements) {
        json elementJson;
        element->to_json(elementJson);
        j["elements"].push_back(elementJson);
    }

    for (const auto& module : graphicModules) {
        json moduleJson;
        module->to_json(moduleJson);
        j["graphicModules"].push_back(moduleJson);
    }
    return j;
}

void Template::from_json(const nlohmann::json& j) {
    clear();
    int rightFlag = 0;
    if (j.contains("name") && j["name"].is_string()) {
        name = j["name"];
    } else {
        throw std::invalid_argument("Template JSON is missing a valid 'name' field.");
    }
    if (j.contains("elements") && j["elements"].is_array()) {
        rightFlag ++;
        for (const auto& elementJson : j["elements"]) {
            if (elementJson.contains("type") && elementJson["type"].is_string()) {
                std::string type = elementJson["type"];

                static const std::unordered_map<std::string, std::function<Element*()>> elementCreators = {
                        {"button", []() { return new Button(); }},
                        {"checkbox", []() { return new Checkbox(); }},
                        {"slider-int", []() { return new Slider<int>; }},
                        {"slider-float", []() { return new Slider<float>; }},
                        {"rectangle", []() { return new Rectangle(); }},
                        {"single-line-label", []() { return new SingleLineLabel(); }},
                        {"multi-line-label", []() { return new MultiLineLabel(); }},
                };

                auto it = elementCreators.find(type);
                if (it != elementCreators.end()) {
                    Element* element = it->second();
                    element->from_json(elementJson);
                    elements.push_back(element);
                } else {
                    throw std::invalid_argument("Unknown element type in JSON: " + type);
                }
            }
        }
    }
    if (j.contains("graphicModules") && j["graphicModules"].is_array()) {
        rightFlag ++;
        for (const auto& moduleJson : j["graphicModules"]) {
            if (moduleJson.contains("name") && moduleJson["name"].is_string()) {
                std::string name = moduleJson["name"];
                ModuleManager moduleManager;
                std::unordered_map<std::string, std::function<GraphicModule*()>> moduleConstructors = moduleManager.getModuleConstructors();
                auto it = moduleConstructors.find(name);
                if (it != moduleConstructors.end()) {
                    GraphicModule* module = it->second();
                    module->from_json(moduleJson);
                    graphicModules.push_back(module);
                } else {
                    throw std::invalid_argument("Unknown module type in JSON: " + name);
                }
            }
        }
    }
    if (rightFlag < 1){
        throw std::invalid_argument("Template JSON is missing a valid 'elements' array.");
    }
}


void Template::saveTemplate(const std::filesystem::path& filePath) const {
    std::ofstream outFile(filePath);
    if (!outFile) {
        throw std::runtime_error("Failed to open file for saving template: " + filePath.string());
    }

    json j = to_json();
    outFile << j.dump(4);
}


void Template::loadTemplate(const std::filesystem::path& filePath) {
    std::ifstream inFile(filePath);
    if (!inFile) {
        throw std::runtime_error("Failed to open file for loading template: " + filePath.string());
    }

    json j;
    inFile >> j;
    from_json(j);
}

std::string Template::getName() const {
    return name;
}

std::vector<GraphicModule *> Template::getModules() {
    return graphicModules;
}

void Template::removeElement(int index) {
    elements.erase(elements.begin() + index);
}

