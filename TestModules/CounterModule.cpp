#include "CounterModule.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

CounterModule::CounterModule(ModuleManager* moduleManager)
        : moduleManager(*moduleManager), counter(0), stopGeneration(false) {
    setModuleName("Counter Module");
    moduleId = this->moduleManager.registerModule(moduleName, this);
    graphicModuleIds.push_back(this->moduleManager.registerGraphicModule("Counter Graphic Element",moduleName, moduleId));
    graphicModuleIds.push_back(this->moduleManager.registerGraphicModule("Text Area",moduleName, moduleId));
    graphicModuleIds.push_back(this->moduleManager.registerGraphicModule("Ultrasonic Graphic Element",moduleName, moduleId));
    generatorThread = std::thread(&CounterModule::run, this);
}

CounterModule::~CounterModule() {
    stopGeneration.store(true);
    if (generatorThread.joinable()) {
        generatorThread.join();
    }
}

void CounterModule::run() {
    while (!stopGeneration.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        counter.fetch_add(1);
        moduleManager.updateValueOfModule(moduleId,graphicModuleIds[0], counter.load());
        std::vector<float> value = {hodnota1, static_cast<float>(counter.load())};
        moduleManager.updateValueOfModule(moduleId, graphicModuleIds[2], hodnota2);
        moduleManager.updateValueOfModule(moduleId,graphicModuleIds[2],value);
        std::string message = "Counter: " + std::to_string(counter.load()) + " | Hodnota 1: " + std::to_string(hodnota1) + " | Hodnota 2: " + std::to_string(hodnota2);
        moduleManager.updateValueOfModule(moduleId,graphicModuleIds[1],message);

    }
}






void CounterModule::setValueFromInputElements(std::string elementName, std::string value) {
    if (elementName == "Stop") {
        if (!stopGeneration.load()) {
            stopGeneration.store(true);
            if (generatorThread.joinable()) {
                generatorThread.join();
            }
        }
    }
    else if (elementName == "Start") {
        if (stopGeneration.load()) {
            stopGeneration.store(false);
            generatorThread = std::thread(&CounterModule::run, this);
        }
    }


}
void CounterModule::setValueFromInputElements(std::string elementName, int value) {
    if ("Hodnota 1" == elementName) {
        hodnota1 = value;
    }
    else if ("Hodnota 2" == elementName) {
        hodnota2 = value;
    }
    else if ("Speed" == elementName) {
        speed = value;
    }
}




