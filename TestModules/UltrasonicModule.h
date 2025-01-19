#pragma once
#include "imgui.h"
#include <vector>
#include <thread>
#include "../src/Module.h"
#include "../src/ModuleManager.h"


/**
 * @class UltrasonicModule
 * @brief UltrasonicModule class is responsible for generating a ultrasonic sensor data and updating the graphic element with the value.
 */


class UltrasonicModule : public Module{

private:
    std::thread generatorThread;
    std::vector<UltrasonicSensorData> sensors;
    std::vector<int> graphicElementIds;
    int  textAreaId;
    float deltaTime;
    std::atomic<bool> running;
    ModuleManager* moduleManager;

    int updateDelayMs = 500;
    std::chrono::steady_clock::time_point lastUpdateTime;
    std::vector<float> previousDistances;
    int frameCounter = 0;
    int updateIntervalFrames = 4;

    void run() override;
    void registerShortcuts(ShortcutsManager &shortcutsManager, ToastNotificationManager &toastNotificationManager) override;
    void updateDynamicSensors();

    void setValueFromInputElements(std::string elementName, std::string value) override;
    void setValueFromInputElements(std::string elementName, int value ) override;
    std::unordered_map<std::string,std::vector<std::string>> getPossibleInputElements() override {
        return {{"button", {"Stop","Start"}},
                {"horizontal-slider-int", {"Interval"}}
                };
    }
    std::vector<std::string> getPossibleGraphicsElement() override {
        return {"Ultrasonic Graphic Element","Text Area","Counter Graphic Element"};
    }

public:
    UltrasonicModule(ModuleManager *moduleManager);
    ~UltrasonicModule();
};

