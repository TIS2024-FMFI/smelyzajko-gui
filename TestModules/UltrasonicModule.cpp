// UltrasonicModule.cpp

#include "UltrasonicModule.h"
#include "imgui.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include "nlohmann/json.hpp"

UltrasonicModule::UltrasonicModule(ModuleManager* moduleManager)
        : position(ImVec2(0, 0)), size(ImVec2(300, 300)), moduleManager(moduleManager), running(true), deltaTime(0.0f) {
    moduleId = moduleManager->gegisterModule("Ultrasonic Module", this);
    graphicModuleId = moduleManager->registerGraphicModule("Ultrasonic Module", moduleId);

    sensors = {
            {0, 5}, {45, 7}, {90, 4}, {135, 8},
            {180, 6}, {225, 3}, {270, 2}, {315, 5}};
    std::string filename = "../TestModules/logs/ultrasonic_module_log.json";
    std::ofstream outFile(filename, std::ios::trunc);

    if (outFile.is_open()) {
        nlohmann::json j;
        j["sensor_data"] = nlohmann::json::array();
        outFile << std::setw(4) << j << std::endl;
        outFile.close();
    } else {
        std::cerr << "[ERROR] Could not initialize log file at: " << filename << std::endl;
    }

    // Start the logging thread
    generatorThread = std::thread(&UltrasonicModule::run, this);
}

UltrasonicModule::~UltrasonicModule() {
    running = false;
    if (generatorThread.joinable()) {
        generatorThread.join();
    }
}

void UltrasonicModule::logSensorDataToJson() {
    std::lock_guard<std::mutex> lock(logMutex);

    std::string filename = "../TestModules/logs/ultrasonic_module_log.json";
    nlohmann::json j;
    std::ifstream inFile(filename);

    if (inFile.is_open()) {
        try {
            inFile >> j;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << std::endl;
        }
        inFile.close();
    }

    if (!j.contains("sensor_data") || !j["sensor_data"].is_array()) {
        j["sensor_data"] = nlohmann::json::array();
    }

    for (const auto& sensor : sensors) {
        nlohmann::json sensorEntry;
        sensorEntry["angle"] = sensor.angle;
        sensorEntry["distance"] = sensor.distance;
        j["sensor_data"].push_back(sensorEntry);
    }

    std::ofstream outFile(filename, std::ios::trunc);
    if (outFile.is_open()) {
        outFile << std::setw(4) << j << std::endl;
        outFile.close();
    } else {
        std::cerr << "[ERROR] Could not open file for writing: " << filename << std::endl;
    }
}

void UltrasonicModule::run() {
    while (running) {
        logSensorDataToJson();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

std::string UltrasonicModule::getName() const {
    return name;
}
