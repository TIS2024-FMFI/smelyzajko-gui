#include "CounterModule.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

CounterModule::CounterModule(ModuleManager* moduleManager)
        : moduleManager(*moduleManager), counter(0), stopGeneration(false) {
    moduleId = this->moduleManager.gegisterModule("Counter Module", this);
    graphicModuleId = this->moduleManager.registerGraphicModule("Counter Module", moduleId);
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
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        counter.fetch_add(1);
        moduleManager.updateValueOfModule(graphicModuleId, counter.load());
    }
}




void CounterModule::saveLogToJson(const std::vector<int>& values) {
    std::lock_guard<std::mutex> lock(logMutex); // Protect access
    std::ofstream outFile("counter_log.json");
    if (outFile.is_open()) {
        outFile << "{\n  \"counter_values\": [";
        for (size_t i = 0; i < values.size(); ++i) {
            outFile << values[i];
            if (i != values.size() - 1) outFile << ", ";
        }
        outFile << "]\n}";
        outFile.close();
    }
}







std::string CounterModule::getName() const {
    return moduleName;
}
