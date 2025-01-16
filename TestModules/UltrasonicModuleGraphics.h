#ifndef SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H

#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"
#include <vector>
#include <string>
#include <mutex>
#include <chrono>

struct UltrasonicSensorData {
    float angle;    // Angle of the sensor
    float distance; // Distance reading from the sensor
};

class UltrasonicModuleGraphics : public GraphicModule {
public:
    UltrasonicModuleGraphics();

    // Overridden methods
    void draw(ImGuiIO& io) override;
    void updateValueOfModule(std::vector<int> value) override;
    void updateValueOfModule(int value) override;


private:

    // Sensor data
    std::vector<UltrasonicSensorData> sensors = {
            {0, 5}, {45, 7}, {90, 4}, {135, 8},
            {180, 6}, {225, 3}, {270, 2}, {315, 5}
    };
    std::vector<float> previousDistances; // Previous distances for comparison

    // Graphics settings
    float graphicsFrequency = 0.0f;
    bool graphicsLogEnabled = false;
    float textFrequency = 0.0f;
    bool textLogEnabled = false;

    // Scrollable log properties
    std::vector<std::string> logValues;  // Logs for displaying updates
    float scrollOffset = 0.0f;           // Current scroll position
    bool autoscrollEnabled = true;       // Enable or disable autoscroll
    std::mutex logMutex;                 // Protects log updates
    int updateDelayMs = 500;             // Delay in milliseconds
    std::chrono::steady_clock::time_point lastUpdateTime; // Timer for updates

    // Sensor update control
    int frameCounter = 0;
    int updateIntervalFrames = 4;

    // Private methods
    void updateDynamicSensors(); // Updates sensor readings dynamically
};

#endif // SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H
