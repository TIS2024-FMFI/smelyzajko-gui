#include "UltrasonicModule.h"
#include <iostream>
#include <iomanip>
#include <fstream>


UltrasonicModule::UltrasonicModule(ModuleManager* moduleManager)
        : moduleManager(moduleManager), running(true), deltaTime(0.0f) {
    setModuleName("UltrasonicModule");
    for (const std::string& element : this->getPossibleGraphicsElement()) {
        graphicElementIds.push_back(this->moduleManager->registerGraphicModule(element, moduleName, moduleId));
    }
    sensors = {
            {0, 5}, {45, 7}, {90, 4}, {135, 8},
            {180, 6}, {225, 3}, {270, 2}, {315, 5}};
    for (const auto& sensor : sensors) {
        previousDistances.push_back(sensor.distance);
    }


    generatorThread = std::thread(&UltrasonicModule::run, this);
}

UltrasonicModule::~UltrasonicModule() {
    running = false;
    if (generatorThread.joinable()) {
        generatorThread.join();
    }
}

void UltrasonicModule::run() {
    while (running) {
        updateDynamicSensors();
    }
}

void UltrasonicModule::updateDynamicSensors() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);

    if (elapsed.count() < updateDelayMs) {
        return; // Skip update if delay has not passed
    }

    lastUpdateTime = now; // Reset the timer

    for (size_t i = 0; i < sensors.size(); ++i) {
        auto& sensor = sensors[i];
        float distanceVariation = static_cast<float>(std::rand() % 21 - 10) / 10.0f;
        sensor.distance = std::clamp(sensor.distance + distanceVariation, 0.0f, 10.0f);

        // Log only if the distance has changed
        if (sensor.distance != previousDistances[i]) {
            std::vector<float> value = {sensor.distance, sensor.angle};
            moduleManager->updateValueOfModule(moduleId, graphicElementIds[0],i);
            moduleManager->updateValueOfModule(moduleId, graphicElementIds[0], value);
            moduleManager->updateValueOfModule(moduleId, graphicElementIds[2], sensor.distance);
            previousDistances[i] = sensor.distance;
        }
    }
}

void UltrasonicModule::setValueFromInputElements(std::string elementName, std::string value) {
    if (elementName =="Stop") {
        running = false;
        if (generatorThread.joinable()) {
            generatorThread.join();
        }
        moduleManager->updateValueOfModule(moduleId,graphicElementIds[1] , "Stopped");
    }else if (elementName == "Start") {
        if (!running) {
            running = true;
            generatorThread = std::thread(&UltrasonicModule::run, this);
        }
        moduleManager->updateValueOfModule(moduleId,graphicElementIds[1] , "Running");
    }
}

void UltrasonicModule::setValueFromInputElements(std::string elementName, int value) {
    if (elementName == "Interval" && value != updateDelayMs) {
        updateDelayMs = value;
        std::string message = "Interval set to " + std::to_string(updateDelayMs) + " ms";
        moduleManager->updateValueOfModule(moduleId, graphicElementIds[1], message);
    }

}

void UltrasonicModule::registerShortcuts(ShortcutsManager& shortcutsManager, ToastNotificationManager& toastManager) {}

