#pragma once
#ifndef ULTRASONIC_MODULE_H
#define ULTRASONIC_MODULE_H

#include "imgui.h"
#include <vector>
#include <thread>
#include "../src/Module.h"
#include "../src/ModuleManager.h"



class UltrasonicModule : public Module{

private:
    std::mutex logMutex;
    std::thread generatorThread;
    ImVec2 position;
    ImVec2 size;
    std::vector<UltrasonicSensorData> sensors;
    std::string name = "Ultrasonic Module";
    int graphicModuleId;
    float deltaTime;
    std::atomic<bool> running;
    ModuleManager* moduleManager;
    void logSensorDataToJson();
    void run() override; // Implementation required
    std::string getName() const override; // Implementation required

public:
    UltrasonicModule(ModuleManager *moduleManager);
    ~UltrasonicModule();
};

#endif // ULTRASONIC_MODULE_H
