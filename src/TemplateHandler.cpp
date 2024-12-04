#include "TemplateHandler.h"
#include <utility>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

TemplateHandler::TemplateHandler() {
    loadAllTemplates();
}

void TemplateHandler::loadAllTemplates() {
//    std::vector<fs::path> jsonFiles;
//    // this is development path TODO
//    std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;
//    fs::path templatesDir = fs::path("../templates");
//    try {
//        if (fs::exists(templatesDir) && fs::is_directory(templatesDir)) {
//            for (const auto& file : fs::directory_iterator(templatesDir)) {
//                if (file.is_regular_file() && file.path().extension() == ".json") {
//                    jsonFiles.push_back(file.path());
//                }
//            }
//        } else {
//            std::cerr << "Templates directory not found at root level." << std::endl;
//        }
//    } catch (const std::exception& e) {
//        std::cerr << "Error while searching for JSON files: " << e.what() << std::endl;
//    }
//
//    // TODO here we only take into consideration templates that are in config file
//    if (jsonFiles.empty()) {
//        std::cout << "No JSON files found in the 'templates' directory." << std::endl;
//    } else {
//        for (const auto& path : jsonFiles) {
//            std::cout << " - " << path.string() << std::endl;
//            Template aTemplate(path);
//            allTemplates.push_back(aTemplate);
//        }
//    }
}

void TemplateHandler::setActiveTemplate(Template aTemplate) {
    activeTemplate = std::move(aTemplate);
}
