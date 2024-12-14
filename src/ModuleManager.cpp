#include "ModuleManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>



int ModuleManager::gegisterModule(const std::string &name, Module *module) {
    modules.push_back(module);
    return modules.size() - 1;
}

void ModuleManager::clearModules() {
    modules.clear();
}

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
