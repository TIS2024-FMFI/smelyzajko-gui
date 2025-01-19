#include "ModuleManager.h"
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

void ModuleManager::updateValueOfModule(int moduleID,int graphicModuleID, const std::string& value) {
    for (GraphicModule *module : graphicModules) {
        if (module->getGraphicModuleID() == graphicModuleID && module->getModuleID() == moduleID) {
            module->updateValueOfModule(value);
        }
    }
}
void ModuleManager::updateValueOfModule(int moduleID,int graphicModuleID,const std::vector<int>& value) {
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
void ModuleManager::updateValueOfModule(int moduleID, int graphicModuleID, const std::vector<float>& value) {
    for (GraphicModule *module : graphicModules) {
        if (module->getGraphicModuleID() == graphicModuleID && module->getModuleID() == moduleID) {
            module->updateValueOfModule(value);
        }
    }
}
int ModuleManager::registerGraphicModule(const std::string &graphicElementName,const std::string &moduleName, int moduleID) {
    GraphicModule *graphicModule = moduleConstructors.at(graphicElementName)();
    graphicModule->setGraphicModuleID(graphicModules.size());
    std::string logDir = logDirectory + "/" + moduleName;
    graphicModule->setLogDirectory(logDir);

    // Set the element logging settings
//    graphicModule->setGraphicsFrequency(20);
//    graphicModule->setGraphicsLogEnabled(true);


    graphicModule->setModuleName(moduleName);
    graphicModule->setModuleID(moduleID);
    graphicModules.push_back(graphicModule);
    return graphicModules.size() - 1;
}


void ModuleManager::setActiveModuleAndDraw(const std::vector<GraphicModule *>& graphicModules_, ImGuiIO &io) {
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

void ModuleManager::setValueFromInputElements(const std::string& moduleName, const std::string& elementName, const std::string& value) {
    for (Module *module : modules) {
        if (module->getModuleName() == moduleName) {
            module->setValueFromInputElements(elementName, value);
        }
    }
}

void ModuleManager::setValueFromInputElements(const std::string& moduleName, const std::string& elementName, bool value) {
    for (Module *module : modules) {
        if (module->getModuleName() == moduleName) {
            module->setValueFromInputElements(elementName, value);
        }
    }
}
void ModuleManager::setValueFromInputElements(const std::string& moduleName, const std::string& elementName, int value) {
    for (Module *module : modules) {
        if (module->getModuleName() == moduleName) {
            module->setValueFromInputElements(elementName, value);
        }
    }
}
void ModuleManager::setValueFromInputElements(const std::string& moduleName, const std::string& elementName, float value) {
    for (Module *module : modules) {
        if (module->getModuleName() == moduleName) {
            module->setValueFromInputElements(elementName, value);
        }
    }
}
void ModuleManager::setLogDirectory(std::string &logDirectory_) {
    logDirectory = logDirectory_;
    for (GraphicModule *module : graphicModules) {
        std::string dir = logDirectory + "/"+module->getModuleName();
        module->setLogDirectory(dir);
    }
}

