#include "TemplateManager.h"
#include <utility>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

TemplateManager::TemplateManager() {
    loadAllTemplates();
}

void TemplateManager::loadAllTemplates() {
    std::vector<fs::path> jsonFiles;
    // this is development path TODO
    fs::path templatesDir = fs::path("templates");
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
        std::cout << "No JSON files found in the 'templates' directory." << std::endl;
    } else {
        for (const auto& path : jsonFiles) {
            Template aTemplate(path);
            allTemplates.push_back(aTemplate);
        }
    }
}

void TemplateManager::setActiveTemplate(Template aTemplate) {
    activeTemplate = std::move(aTemplate);
}

std::vector<Element *> TemplateManager::getActiveTemplateElements() {
    return activeTemplate.getElements();
}

std::vector<Template> TemplateManager::getAllTemplates() {
    return allTemplates;
}

void TemplateManager::saveCurrentTemplate(const std::string& fileName) {
    activeTemplate.saveTemplate(fileName);
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
