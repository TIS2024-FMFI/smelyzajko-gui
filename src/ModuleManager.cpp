// File: ModuleManager.cpp

#include "ModuleManager.h"
#include "../TestModules/TestGraphicModule.h"
#include "../TestModules/CounterModule.h"
#include "../TestModules/MapModule.h"
#include "../TestModules/UltrasonicModule.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

TestGraphicModule testGraphicModule;
CounterModule counterModule;
MapModule mapModule = MapModule();
UltrasonicModule ultrasonicModule;

void ModuleManager::addModule(Module* module) {
    modules.push_back(module);
}

const std::vector<Module *> &ModuleManager::getModules() const {
    return modules;
}

void ModuleManager::createModules() {
    addModule(&testGraphicModule);
    addModule(&counterModule);
    addModule(&mapModule);
    addModule(&ultrasonicModule);

}
void ModuleManager::renderModules(ImGuiIO io, ImVec2 possition,ImVec2 size) {
    ultrasonicModule.setPos(possition);
    ultrasonicModule.setSize(size);
    ultrasonicModule.draw(io);

}


void ModuleManager::readTemplateandCreateModules(const std::string& filename) {
    // Check if the file exists
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error("File not found: " + filename);
    }

    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // Read the file content
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonString = buffer.str();
    file.close();

    // Parse the JSON content
    std::istringstream jsonStream(jsonString);
    std::string line;
    while (std::getline(jsonStream, line)) {
        if (line.find("{") != std::string::npos) {
            std::getline(jsonStream, line);
            std::string name = line.substr(line.find(":") + 3);
            name = name.substr(0, name.find_last_of("\""));
            std::getline(jsonStream, line);
            ImVec2 pos;
            sscanf(line.c_str(), "    \"pos\": [%f, %f],", &pos.x, &pos.y);
            std::getline(jsonStream, line);
            ImVec2 size;
            sscanf(line.c_str(), "    \"size\": [%f, %f]", &size.x, &size.y);
            std::cout << name << " " << pos.x << " " << pos.y << " " << size.x << " " << size.y << std::endl;
            for (Module* module : modules) {
                std::cout << module->getName().c_str() << std::endl;
                if (module->getName() == name) {
                    std::cout << name << " " << pos.x << " " << pos.y << " " << size.x << " " << size.y << std::endl;
                    module->setPos(pos);
                    module->setSize(size);
                }
            }
        }
    }
}