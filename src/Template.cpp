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

using json = nlohmann::json;

std::vector<Element *> Template::getElements() {
    return elements;
}

void Template::addElement(Element* element) {
    elements.push_back(element);
}

void Template::clear() {
    elements.clear();
}

nlohmann::json Template::to_json() const {
    json j;
    j["name"] = name;
    j["elements"] = json::array();

    for (const auto& element : elements) {
        json elementJson;
        element->to_json(elementJson);
        j["elements"].push_back(elementJson);
    }

    return j;
}

void Template::from_json(const nlohmann::json& j) {
    clear();

    if (j.contains("name") && j["name"].is_string()) {
        name = j["name"];
    } else {
        throw std::invalid_argument("Template JSON is missing a valid 'name' field.");
    }

    if (j.contains("elements") && j["elements"].is_array()) {
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
    } else {
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

void Template::removeElement(int index) {
    elements.erase(elements.begin() + index);
}

