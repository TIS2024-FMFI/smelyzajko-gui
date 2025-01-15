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
    void to_json(nlohmann::json& j) const override;
    void from_json(const nlohmann::json& j) override;
    void updateValueOfModule(std::string value) override;
    void updateValueOfModule(int value) override;
    void updateValueOfModule(std::vector<int> value) override;

    // Getter and Setter methods
    ImVec2 getPos() override { return position; }
    ImVec2 getSize() override { return size; }
    std::string getName() const override { return moduleName; }
    int getGraphicModuleID() override { return graphicModuleId; }
    void setPos(ImVec2 pos_) override { position = pos_; }
    void setSize(ImVec2 size_) override { size = size_; }
    void setGraphicModuleID(int id) override { graphicModuleId = id; }

private:
    // Graphics properties
    ImVec2 position;       // Position of the module
    ImVec2 size;           // Size of the module
    std::string moduleName = "Ultrasonic Module";
    int graphicModuleId;
    int moduleId;

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
