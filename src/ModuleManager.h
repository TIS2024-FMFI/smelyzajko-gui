#pragma once
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include <vector>
#include "Module.h"
#include "../TestModules/TestGraphicModule.h"
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
    void setLogDirectory( std::string& logDirectory_);

    void updateValueOfModule(int moduleID,int graphicModuleID, std::string value);
    void updateValueOfModule(int moduleID,int graphicModuleID, int value);
    void updateValueOfModule(int moduleID,int graphicModuleID, std::vector<int> value);
    void updateValueOfModule(int moduleID,int graphicModuleID, std::vector<float> value);


    void setValueFromInputElements(std::string moduleName, std::string elementName, std::string value);
    void setValueFromInputElements(std::string moduleName, std::string elementName, bool value);
    void setValueFromInputElements(std::string moduleName, std::string elementname, int value);
    void setValueFromInputElements(std::string moduleName, std::string elementName, float value);

    void setActiveModuleAndDraw(std::vector<GraphicModule *> graphicModules_, ImGuiIO &io);
    std::vector<Module*> getModules() const;
    const std::unordered_map<std::string, std::function<GraphicModule*()>> &getModuleConstructors() const;
    void clearModules();
    std::vector<GraphicModule*> graphicModules;
    std::vector<Module*> modules ;


private:
    std::unordered_map<std::string, std::function<GraphicModule*()>> moduleConstructors = {
            {"Map Graphic Element", []() { return new MapModuleGraphics(); }},
             {"Counter Graphic Element", []() { return new CounterModuleGraphics(); }},
            {"Ultrasonic Graphic Element", []() { return new UltrasonicModuleGraphics(); }},
            {"Text Area", []() { return new TextArea(); }},
    };
    std::string logDirectory;



    void readTemplateandCreateModules(const std::string &filename);
};

#endif //MODULEMANAGER_H