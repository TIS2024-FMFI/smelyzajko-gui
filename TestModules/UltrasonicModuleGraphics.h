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
    void logToJson() override;

    void updateValueOfModule(int value) override;
    void updateValueOfModule(std::vector<float>) override;



    int i;
private:
    std::mutex logMutex;

    // Sensor data
    std::vector<UltrasonicSensorData> sensors = {
            {0, 5}, {45, 7}, {90, 4}, {135, 8},
            {180, 6}, {225, 3}, {270, 2}, {315, 5}
    };


};

#endif // SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H
