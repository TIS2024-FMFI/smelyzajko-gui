#include "TemplateManager.h"
#include <utility>
#include <filesystem>
#include <iostream>
#include "widgets/Element.h"
#include "vector"

namespace fs = std::filesystem;

TemplateManager::TemplateManager() {}

TemplateManager::TemplateManager(bool mode) : configMode(mode) {
    loadAllTemplates();
}

TemplateManager::TemplateManager(const std::vector<std::string> &templateNames, bool mode) : configMode(mode) {
    loadTemplates(templateNames);
}

void TemplateManager::loadAllTemplates() {
    std::vector<fs::path> jsonFiles;
    fs::path templatesDir = fs::path("../templates");
    try {
        if (fs::exists(templatesDir) && fs::is_directory(templatesDir)) {
            for (const auto& file : fs::directory_iterator(templatesDir)) {
                if (file.is_regular_file() && file.path().extension() == ".json") {
                    jsonFiles.push_back(file.path());
                }
            }
        } else {
            std::cerr << "Templates directory not found at root level." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error while searching for JSON files: " << e.what() << std::endl;
    }

    // TODO here we only take into consideration templates that are in config file
    if (jsonFiles.empty()) {
        std::cerr << "No JSON files found in the 'templates' directory." << std::endl;
    } else {
        for (const auto& path : jsonFiles) {

            Template aTemplate(path,configMode);
            allTemplates.push_back(aTemplate);
        }

    }

}

void TemplateManager::loadTemplates(const std::vector<std::string>& templateNames) {
    std::vector<fs::path> jsonFiles;
    fs::path templatesDir = fs::path("../templates");
    try {
        if (fs::exists(templatesDir) && fs::is_directory(templatesDir)) {
            for (const auto& file : fs::directory_iterator(templatesDir)) {
                if (file.is_regular_file() && file.path().extension() == ".json") {
                    jsonFiles.push_back(file.path());
                }
            }
        } else {
            std::cerr << "Templates directory not found at root level." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error while searching for JSON files: " << e.what() << std::endl;
    }

    // Load only templates that are listed in the config file
    if (jsonFiles.empty()) {
        std::cerr << "No JSON files found in the 'templates' directory." << std::endl;
    } else {
        for (const auto& path : jsonFiles) {
            std::string fileName = path.filename().string();
            if (std::find(templateNames.begin(), templateNames.end(), fileName) != templateNames.end()) {
                // Load template if it is in the list of templates from the config file
                Template aTemplate(path,configMode);
                allTemplates.push_back(aTemplate);
            }
        }
    }
}

void TemplateManager::setActiveTemplate(Template aTemplate) {
    activeTemplate = std::move(aTemplate);
}

std::vector<Element *> &TemplateManager::getActiveTemplateElements() {
    return activeTemplate.getElements();
}

std::vector<Template> TemplateManager::getAllTemplates() {
    return allTemplates;
}

void TemplateManager::addElementToActiveTemplate(Element *element) {
    activeTemplate.addElement(element);
}


std::string TemplateManager::getActiveTemplateName() const {
    return activeTemplate.getName();
}

void TemplateManager::clearActiveTemplateElements() {
    activeTemplate.clear();
}

void TemplateManager::removeElementFromActiveTemplate(int index) {
    activeTemplate.removeElement(index);
}

Template TemplateManager::getActiveTemplate() {
    return activeTemplate;
}
void TemplateManager::addModuleToActiveTemplate(GraphicModule *module) {
    activeTemplate.addModule(module);
}

std::vector<GraphicModule *> TemplateManager::getActiveTemplateModules() {
    return activeTemplate.getModules();
}
void TemplateManager::setConfigMode(bool mode) {
    configMode = mode;
}
