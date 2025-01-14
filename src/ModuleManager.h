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

    int gegisterModule(const std::string& name, Module *module);
    int registerGraphicModule(const std::string& name,int moduleID);

    void updateValueOfModule(int moduleID, std::string value);
    void updateValueOfModule(int moduleID, int value);
    void updateValueOfModule(int moduleID, std::vector<int> value);
    void setActiveModuleAndDraw(std::vector<GraphicModule *> graphicModules_, ImGuiIO &io);



    const std::unordered_map<std::string, std::function<GraphicModule*()>> &getModuleConstructors() const;
    void clearModules();
    std::vector<GraphicModule*> graphicModules;

    std::vector<Module *> getModules();

private:
    std::vector<Module*> modules ;
    std::unordered_map<std::string, std::function<GraphicModule*()>> moduleConstructors = {
            {"Map Module", []() { return new MapModuleGraphics(); }},
             {"Counter Module", []() { return new CounterModuleGraphics(); }},
            {"Ultrasonic Module", []() { return new UltrasonicModuleGraphics(); }},
    };

};

#endif //MODULEMANAGER_H