#include "ModuleManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

int ModuleManager::registerModule(const std::string &name, Module *module) {
    modules.push_back(module);
    return modules.size() - 1;
}

void ModuleManager::clearModules() {
    modules.clear();
}
const std::unordered_map<std::string, std::function<GraphicModule *()>> &ModuleManager::getModuleConstructors() const {
    return moduleConstructors;
}

void ModuleManager::updateValueOfModule(int moduleID,int graphicModuleID, std::string value) {
    for (GraphicModule *module : graphicModules) {
        if (module->getGraphicModuleID() == graphicModuleID && module->getModuleID() == moduleID) {
            module->updateValueOfModule(value);
        }
    }
}
void ModuleManager::updateValueOfModule(int moduleID,int graphicModuleID,std::vector<int> value) {
    for (GraphicModule *module : graphicModules) {
        if (module->getGraphicModuleID() == graphicModuleID && module->getModuleID() == moduleID) {
            module->updateValueOfModule(value);
        }
    }
}
void ModuleManager::updateValueOfModule(int moduleID,int graphicModuleID, int value) {
    for (GraphicModule *module : graphicModules) {
        if (module->getGraphicModuleID() == graphicModuleID && module->getModuleID() == moduleID) {
            module->updateValueOfModule(value);
        }
    }
}

int ModuleManager::registerGraphicModule(const std::string &graphicElementName,const std::string &moduleName, int moduleID) {
    GraphicModule *graphicModule = moduleConstructors.at(graphicElementName)();
    graphicModule->setGraphicModuleID(graphicModules.size());
    graphicModule->setModuleName(moduleName);
    graphicModule->setModuleID(moduleID);
    graphicModules.push_back(graphicModule);
    return graphicModules.size() - 1;
}


void ModuleManager::setActiveModuleAndDraw(std::vector<GraphicModule *> graphicModules_, ImGuiIO &io) {
    for (GraphicModule *module : graphicModules) {
        for (GraphicModule *module_ : graphicModules_) {
            if (module->getGraphicElementName() == module_->getGraphicElementName() && module->getModuleName() == module_->getModuleName()) {
                auto pos = module_->getPos();
                auto size = module_->getSize();
                module->setPos(pos);
                module->setSize(size);
                module->draw(io);
            }

        }
    }
}

std::vector<Module *> ModuleManager::getModules() const {
    return modules;
}
