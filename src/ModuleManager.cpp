#include "ModuleManager.h"
#include <iostream>
#include <algorithm>


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
void ModuleManager::updateValueOfModule(int moduleID, int graphicModuleID, const std::vector<std::vector<int>> &value) {
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
std::vector<GraphicModule *> ModuleManager::getGraphicModules() const {
    return graphicModules;
}

void ModuleManager::setValueFromInputElements(const std::string& moduleName, const std::string& elementName, const std::string& value) {
    for (Module *module : modules) {
        if (module->getModuleName() == moduleName) {
            module->setValueFromInputElements(elementName, value);
            return;
        }
    }
    std::cerr << "Module not found: " << moduleName << std::endl;
}

void ModuleManager::setValueFromInputElements(const std::string& moduleName, const std::string& elementName, bool value) {
    for (Module *module : modules) {
        if (module->getModuleName() == moduleName) {
            module->setValueFromInputElements(elementName, value);
            return;
        }
    }
    std::cerr << "Module not found: " << moduleName << std::endl;
}
void ModuleManager::setValueFromInputElements(const std::string& moduleName, const std::string& elementName, int value) {
    for (Module *module : modules) {
        if (module->getModuleName() == moduleName) {
            module->setValueFromInputElements(elementName, value);
            return;
        }
    }
    std::cerr << "Module not found: " << moduleName << std::endl;
}
void ModuleManager::setValueFromInputElements(const std::string& moduleName, const std::string& elementName, float value) {
    for (Module *module : modules) {
        if (module->getModuleName() == moduleName) {
            module->setValueFromInputElements(elementName, value);
            return;
        }
    }
    std::cerr << "Module not found: " << moduleName << std::endl;

}

void ModuleManager::logSettings(YAML::Node configFile) {

    if (!configFile["logDirectory"]) {
        std::cerr << "[ERROR] No log directory specified in the configuration file." << std::endl;
        return;
    }
    std::string logDirectory;
    if (configFile["logDirectory"] && configFile["logDirectory"].as<std::string>().empty()) {
        logDirectory = configFile["logDirectory"].as<std::string>();
    } else {
        logDirectory = "../logs";
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string timestamp = ss.str();
    std::filesystem::path newLogDir = std::filesystem::path(logDirectory) / timestamp;

    bool atLeastOneModuleLogEnabled = false;
    for (GraphicModule *module : graphicModules) {
        for (const auto& moduleNode : configFile["modules"]) {
            if (!moduleNode.IsMap() || moduleNode.size() != 1) {
                std::cerr << "Invalid module format. Each module should be a map with one key-value pair." << std::endl;
                continue;
            }

            std::string moduleName = moduleNode.begin()->first.as<std::string>();
            YAML::Node moduleParams = moduleNode.begin()->second;
            if (!moduleParams.IsMap()){
                std::cerr<< "Invalid module format. Each module should be a map with one key-value pair."<<std::endl;
            }
            if (moduleName.empty()) {
                std::cerr << "Empty module name found. Skipping." << std::endl;
                continue;
            }

            if (moduleName == module->getModuleName()) {
                module->setGraphicsFrequency(moduleParams["graphicsFrequency"].as<float>());
                module->setGraphicsLogEnabled(moduleParams["graphicsLogEnabled"].as<bool>());
                module->setTextFrequency(moduleParams["textFrequency"].as<float>());
                module->setTextLogEnabled(moduleParams["textLogEnabled"].as<bool>());
                std::string newLogDirForGraphicElement = newLogDir.string() + "/" + moduleName;
                module->setLogDirectory(newLogDirForGraphicElement);
            }
            if (!atLeastOneModuleLogEnabled&&(moduleParams["graphicsLogEnabled"].as<bool>() || moduleParams["textLogEnabled"].as<bool>())) {
                atLeastOneModuleLogEnabled = true;
                // Ensure the directory is writable
                if (std::filesystem::exists(newLogDir) && !std::filesystem::is_directory(newLogDir)) {
                    std::cerr << "[ERROR] Path exists and is not a directory: " << newLogDir << std::endl;
                    return;
                }

                try {
                    std::filesystem::create_directories(newLogDir);
                } catch (const std::filesystem::filesystem_error& e) {
                    std::cerr << "[ERROR] Could not create directory: " << e.what() << std::endl;
                    return;
                }
            }
            if (moduleParams["graphicsLogEnabled"].as<bool>() || moduleParams["textLogEnabled"].as<bool>()) {
                std::filesystem::path moduleLogDir = std::filesystem::path(newLogDir) / moduleName;
                if (!std::filesystem::exists(moduleLogDir)) {
                    try {
                        std::filesystem::create_directories(moduleLogDir);
                    } catch (const std::filesystem::filesystem_error& e) {
                        std::cerr << "[ERROR] Could not create module directory: " << e.what() << std::endl;
                        return;
                    }
                }

            }


        }


    }
}

void ModuleManager::setLogDirectory(int moduleID, int graphicModuleID, const std::string &logDir) {
    for (GraphicModule *module : graphicModules) {
        if (module->getGraphicModuleID() == graphicModuleID && module->getModuleID() == moduleID) {
            module->setLogDirectory(logDir);
        }
    }
}
