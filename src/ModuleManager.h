// File: ModuleManager.h

#ifndef SMELYZAJKO_GUI_MODULEMANAGER_H
#define SMELYZAJKO_GUI_MODULEMANAGER_H

#include <vector>
#include "Module.h"

class ModuleManager {
public:
    void addModule(Module* module);
    const std::vector<Module*>& getModules() const;
    void createModules();
    void renderModules();
    void readTemplateandCreateModules(const std::string& filename);

private:
    std::vector<Module*> modules;
};

#endif //SMELYZAJKO_GUI_MODULEMANAGER_H