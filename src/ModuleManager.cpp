#include "ModuleManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ModuleManager.h"

int ModuleManager::registerModule(const std::string &name, Module *module) {
    modules.push_back(module);
    return modules.size() - 1;
}

void ModuleManager::clearModules() {
    modules.clear();
}

//void ModuleManager::renderModules() {
//    for (Module* module : modules) {
//        module->renderStandalone();
//    }
//}

//void ModuleManager::readTemplateandCreateModules(const std::string& filename) {
//    // Check if the file exists
//    if (!std::filesystem::exists(filename)) {
//        throw std::runtime_error("File not found: " + filename);
//    }
//
//    // Open the file
//    std::ifstream file(filename);
//    if (!file.is_open()) {
//        throw std::runtime_error("Failed to open file: " + filename);
//    }
//
//    // Read the file content
//    std::stringstream buffer;
//    buffer << file.rdbuf();
//    std::string jsonString = buffer.str();
//    file.close();
//
//    // Parse the JSON content
//    std::istringstream jsonStream(jsonString);
//    std::string line;
//    while (std::getline(jsonStream, line)) {
//        if (line.find("{") != std::string::npos) {
//            std::getline(jsonStream, line);
//            std::string name = line.substr(line.find(":") + 3);
//            name = name.substr(0, name.find_last_of("\""));
//            std::getline(jsonStream, line);
//            ImVec2 pos;
//            sscanf(line.c_str(), "    \"pos\": [%f, %f],", &pos.x, &pos.y);
//            std::getline(jsonStream, line);
//            ImVec2 size;
//            sscanf(line.c_str(), "    \"size\": [%f, %f]", &size.x, &size.y);
//            std::cout << name << " " << pos.x << " " << pos.y << " " << size.x << " " << size.y << std::endl;
//            for (Module* module : modules) {
//                std::cout << module->getName().c_str() << std::endl;
//                if (module->getName() == name) {
//                    std::cout << name << " " << pos.x << " " << pos.y << " " << size.x << " " << size.y << std::endl;
//                    module->setPos(pos);
//                    module->setSize(size);
//                }
//            }
//        }
//    }
//}

const std::unordered_map<std::string, std::function<GraphicModule *()>> &ModuleManager::getModuleConstructors() const {
    return moduleConstructors;
}

void ModuleManager::updateValueOfModule(int moduleID, std::string value) {
    graphicModules[moduleID]->updateValueOfModule(value);
}
void ModuleManager::updateValueOfModule(int moduleID,std::vector<int> value) {
    graphicModules[moduleID]->updateValueOfModule(value);
}
void ModuleManager::updateValueOfModule(int moduleID, int value) {
    graphicModules[moduleID]->updateValueOfModule(value);
}

int ModuleManager::registerGraphicModule(const std::string &name, int moduleID) {
    GraphicModule *graphicModule = moduleConstructors.at(name)();
    graphicModule->setGraphicModuleID(graphicModules.size());
    graphicModule->setModuleID(moduleID);
    graphicModules.push_back(graphicModule);
    return graphicModules.size() - 1;

}


void ModuleManager::setActiveModuleAndDraw(std::vector<GraphicModule *> graphicModules_, ImGuiIO &io) {
    for (GraphicModule *module : graphicModules) {
        for (GraphicModule *module_ : graphicModules_) {
            if (module->getGraphicModuleID() == module_->getGraphicModuleID()) {
                auto pos = module_->getPos();
                auto size = module_->getSize();
                module->setPos(pos);
                module->setSize(size);
                module->draw(io);
            }
        }
    }
}
