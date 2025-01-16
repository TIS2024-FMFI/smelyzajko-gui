#include "Module.h"

//module settings
int Module::getModuleID() const {return moduleId;}
void Module::setModuleID(int id) {moduleId = id;}
std::string Module::getModuleName() const {return moduleName;}
void Module::setModuleName(std::string name) {moduleName = name;}

