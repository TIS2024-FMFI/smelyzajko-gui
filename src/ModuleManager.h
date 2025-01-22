#pragma once
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include <vector>
#include "Module.h"

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

#include "../TestModules/MapModuleGraphics.h"
#include "../TestModules/CounterModuleGraphics.h"
#include "../TestModules/UltrasonicModuleGraphics.h"



class ModuleManager  {
public:
    static ModuleManager& getInstance() {
        static ModuleManager instance;
        return instance;
    }

    int registerModule(const std::string& name, Module *module);
    int registerGraphicModule(const std::string& graphicElementName,const std::string& moduleName,  int moduleID);
    void logSettings(YAML::Node configFile);
    void setLogDirectory(int moduleID,int graphicModuleID,const std::string& logDir);

    void updateValueOfModule(int moduleID,int graphicModuleID, const std::string& value);
    void updateValueOfModule(int moduleID,int graphicModuleID, int value);
    void updateValueOfModule(int moduleID,int graphicModuleID, const std::vector<int>& value);
    void updateValueOfModule(int moduleID,int graphicModuleID, const std::vector<float>& value);
    void updateValueOfModule(int moduleID,int graphicModuleID, const std::vector<std::vector<int>>& value);


    void setValueFromInputElements(const std::string& moduleName, const std::string& elementName, const std::string& value);
    void setValueFromInputElements(const std::string& moduleName, const std::string& elementName, bool value);
    void setValueFromInputElements(const std::string& moduleName, const std::string& elementname, int value);
    void setValueFromInputElements(const std::string& moduleName, const std::string& elementName, float value);

    void setActiveModuleAndDraw(const std::vector<GraphicModule *>& graphicModules_, ImGuiIO &io);
    std::vector<Module*> getModules() const;
    std::vector<GraphicModule*> getGraphicModules() const;
    const std::unordered_map<std::string, std::function<GraphicModule*()>> &getModuleConstructors() const;
    void clearModules();
    std::vector<GraphicModule*> graphicModules;
    std::vector<Module*> modules;

private:
    std::unordered_map<std::string, std::function<GraphicModule*()>> moduleConstructors = {
            {"MapGraphicElement", []() { return new MapModuleGraphics(); }},
             {"CounterGraphicElement", []() { return new CounterModuleGraphics(); }},
            {"UltrasonicGraphicElement", []() { return new UltrasonicModuleGraphics(); }},
            {"TextArea", []() { return new TextArea(); }},
    };
    std::string logDirectory;

};

#endif //MODULEMANAGER_H