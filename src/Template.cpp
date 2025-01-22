#include "Template.h"
#include <fstream>
#include <stdexcept>
#include <GLFW/glfw3.h>

#include "libs/json.hpp"
#include "widgets/Button.h"
#include "widgets/HorizontalSlider.h"
#include "widgets/VerticalSlider.h"
#include "widgets/Checkbox.h"
#include "widgets/Rectangle.h"
#include "widgets/SingleLineLabel.h"
#include "widgets/MultiLineLabel.h"
#include "widgets/TextInput.h"
#include "TemplateManager.h"
#include "ModuleManager.h"

using json = nlohmann::json;

std::vector<Element *> &Template::getElements() {
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
        std::cout<<elementJson["type"]<<std::endl;
        if (elementJson["type"] == "rectangle"){
            j["graphicModules"].push_back(elementJson);
        }else {
            j["elements"].push_back(elementJson);
        }
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

    if (j.contains("resolution") && j["resolution"].is_array() && j["resolution"].size() == 2) {
        float width = j["resolution"][0];
        float height = j["resolution"][1];

        resolution.x = width;
        resolution.y = height;
    } else {
        throw std::invalid_argument("Invalid or missing resolution in JSON!");
    }

    if (j.contains("elements") && j["elements"].is_array()) {
        rightFlag ++;
        for (const auto& elementJson : j["elements"]) {
            if (elementJson.contains("type") && elementJson["type"].is_string()) {
                std::string type = elementJson["type"];

                static const std::unordered_map<std::string, std::function<Element*()>> elementCreators = {
                        {"button", []() { return new Button(); }},
                        {"checkbox", []() { return new Checkbox(); }},
                        {"horizontal-slider-int", []() { return new HorizontalSlider<int>(); }},
                        {"horizontal-slider-float", []() { return new HorizontalSlider<float>(); }},
                        {"vertical-slider-int", []() { return new VerticalSlider<int>(); }},
                        {"vertical-slider-float", []() { return new VerticalSlider<float>(); }},
                        {"rectangle", []() { return new Rectangle(); }},
                        {"single-line-label", []() { return new SingleLineLabel(); }},
                        {"multi-line-label", []() { return new MultiLineLabel(); }},
                        {"text-input", []() { return  new TextInput(); }}
                };

                auto it = elementCreators.find(type);
                if (it != elementCreators.end()) {
                    Element* element = it->second();
                    element->from_json(elementJson, resolution);
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
            if (moduleJson.contains("graphicElementName") && moduleJson["graphicElementName"].is_string()) {
                std::string graphicElementName = moduleJson["graphicElementName"];
                ModuleManager moduleManager;
                std::unordered_map<std::string, std::function<GraphicModule*()>> moduleConstructors = moduleManager.getModuleConstructors();
                auto it = moduleConstructors.find(graphicElementName);
                if (it == moduleConstructors.end()) {
                    std::cerr << "Error: Unknown module type in JSON: " << graphicElementName << std::endl;
                    continue;
                }
                if (configurationMode) {
                    // Create a Rectangle based on parameters in the template
                    Rectangle* rectangle = new Rectangle();
                    rectangle->from_json(moduleJson,resolution);
                    elements.push_back(rectangle);
                }
                else if (it != moduleConstructors.end()) {
                    // Create and add the module as usual
                    GraphicModule* module = it->second();
                    module->from_json(moduleJson, resolution);
                    graphicModules.push_back(module);

                } else {
                    throw std::invalid_argument("Unknown module type in JSON:  " + graphicElementName);
                }
            }

        }
    }
    if (rightFlag < 1){
        throw std::invalid_argument("Template JSON is missing a valid 'elements' array.");
    }
}


void Template::saveTemplate(const std::filesystem::path& filePath, std::string newName) {
    std::ofstream outFile(filePath);
    if (!outFile) {
        throw std::runtime_error("Failed to open file for saving template: " + filePath.string());
    }

    if (!newName.empty()) {
        std::string fileName = filePath.filename().string();
        if (fileName.size() > 5 && fileName.substr(fileName.size() - 5) == ".json") {
            fileName = fileName.substr(0, fileName.size() - 5);  // Remove the ".json" part
        }

        name = fileName;
    }

    json j = to_json();

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

    int monitorWidth = videoMode->width;
    int monitorHeight = videoMode->height;

    j["resolution"] = {monitorWidth, monitorHeight};
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

