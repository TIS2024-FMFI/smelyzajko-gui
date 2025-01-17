#include "Module.h"
#include "iostream"

//module settings
int Module::getModuleID() const {return moduleId;}
void Module::setModuleID(int id) {moduleId = id;}
std::string Module::getModuleName() const {return moduleName;}
void Module::setModuleName(std::string name) {moduleName = name;}

void Module::setValueFromInputElements(std::string elementName, std::string value) {
    std::cerr << "Not implemented value type: " << typeid(value).name() << std::endl;
}
void Module::setValueFromInputElements(std::string elementName, int value) {
    std::cerr << "Not implemented value type: " << typeid(value).name() << std::endl;
}
void Module::setValueFromInputElements(std::string elementName, std::vector<int> value) {
    std::cerr << "Not implemented value type: " << typeid(value).name() << std::endl;
}
void Module::setValueFromInputElements(std::string elementName, float value) {
    std::cerr << "Not implemented value type: " << typeid(value).name() << std::endl;
}
void Module::setValueFromInputElements(std::string elementName, bool value) {
    std::cerr << "Not implemented value type: " << typeid(value).name() << std::endl;
}