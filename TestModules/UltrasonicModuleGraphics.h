#ifndef SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H

#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"
#include <vector>
#include <string>
#include <mutex>
#include <chrono>
#include "TextArea.h"

struct UltrasonicSensorData {
    float angle;    // Angle of the sensor
    float distance; // Distance reading from the sensor
};

class UltrasonicModuleGraphics : public GraphicModule {
public:
    UltrasonicModuleGraphics();

    // Overridden methods
    void draw(ImGuiIO& io) override;                          // Render the module
    void updateValueOfModule(std::vector<int> value) override; // Update sensor values
    void updateValueOfModule(int value) override;             // Update other properties (e.g., update interval)

private:
    // Sensor data
    std::vector<UltrasonicSensorData> sensors = {
            {0, 5}, {45, 7}, {90, 4}, {135, 8},
            {180, 6}, {225, 3}, {270, 2}, {315, 5}
    };
    std::vector<float> previousDistances; // Track previous distances for logging changes

    // Graphics settings
    float graphicsFrequency = 0.0f;
    bool graphicsLogEnabled = false;
    float textFrequency = 0.0f;
    bool textLogEnabled = false;

    // Scrollable log using TextArea
    TextArea textArea;                     // Instance of TextArea for log rendering

    // Sensor update control
    int updateDelayMs = 500;               // Delay between updates in milliseconds
    std::chrono::steady_clock::time_point lastUpdateTime; // Timestamp of last update

    // Sensor update control
    int frameCounter = 0;
    int updateIntervalFrames = 4;

    // Private methods
    void updateDynamicSensors(); // Updates sensor readings dynamically
};

#endif // SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H
