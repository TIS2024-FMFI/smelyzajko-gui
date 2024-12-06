#pragma once
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H
#include <vector>
#include "Module.h"

class ModuleManager  {
public:
    void addModule(Module* module);
    void createModules();
    void renderModules(ImGuiIO io, ImVec2 possition);
    void readTemplateandCreateModules(const std::string& filename);

    const std::vector<Module *> &getModules() const;

private:
    std::vector<Module*> modules;
};

#endif //MODULEMANAGER_H