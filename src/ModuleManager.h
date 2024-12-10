#pragma once
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include <vector>
#include "Module.h"
#include "../TestModules/TestGraphicModule.h"
#include "../TestModules/CounterModule.h"
#include "../TestModules/MapModule.h"
#include "../TestModules/UltrasonicModule.h"

class ModuleManager  {
public:
    void addModule(Module* module);
    void createModules();

    void renderModules(ImGuiIO io, ImVec2 possition, ImVec2 size);
    void readTemplateandCreateModules(const std::string& filename);

    const std::vector<Module *> &getModules() const;

private:
    TestGraphicModule testGraphicModule;
    CounterModule counterModule;
    MapModule mapModule = MapModule();
    UltrasonicModule ultrasonicModule;
    std::vector<Module*> modules ;

};

#endif //MODULEMANAGER_H