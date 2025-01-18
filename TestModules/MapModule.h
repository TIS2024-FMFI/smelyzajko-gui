#ifndef MAPMODULE_H
#define MAPMODULE_H

#include <vector>
#include <utility>
#include <string>
#include "imgui.h"
#include "../src/Module.h"
#include <thread>
#include <atomic>
#include <chrono>
#include "../src/ModuleManager.h"


class MapModule : public Module {


public:
    MapModule(ModuleManager* moduleManager);
//    MapModule(int id, const char* name) : Module(id, name) {}
    ~MapModule() ;
    void run() override;
    void generatePassableMap();
    void saveMapToJson();
    void generatePath();
    ModuleManager& moduleManager;
    std::vector<int> graphicModuleId;


    void setValueFromInputElements(std::string elementName, std::string value) override ;
    void setValueFromInputElements(std::string elementName, bool value) override ;
    void setValueFromInputElements(std::string elementName, float value) override ;
//    void drawButtons();
    std::vector<std::string> getPossibleGraphicsElement() override {
        return {"Map Graphic Element","Text Area","Counter Graphic Element"};
    }
    std::unordered_map<std::string,std::vector<std::string>> getPossibleInputElements() override {
        return {{"button", {"Reset"}},
                {"checkbox", {"Running"}},
                {"vertical-slider-float", {"Speed"}}
                };
    }


private:
    std::thread mapThread;
    std::atomic<bool> running;
    float deltaTime;
    bool isStopped = false;
    int currentStep = 0;
    bool mapInitialized = false;
    float speed = 1.0f;
    std::pair<int, int> startPosition;
    int rows = 20;
    int cols = 20;
    const float cellSize = 40.0f;
    std::vector<std::pair<int, int>> path;
    std::vector<std::vector<int>> map;

};


#endif // MAPMODULE_H
