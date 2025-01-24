#ifndef SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H

#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"
#include <vector>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>


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
    void logFromJson() override;
    void logForward() override;
    void logBackwards() override;

    void updateValueOfModule(int value) override;
    void updateValueOfModule(std::vector<float>) override;
    void startLoggingThread() override;




    int i;
private:
    void stopLoggingThread();
    void loggingThreadFunction();
    std::thread loggingThread;
    std::atomic<bool> loggingThreadRunning;

    std::vector<std::vector<std::pair<float, float>>> sensorsFromLog; // 8 sensors per chunk
    size_t currentSensorIndexLog = 0;  // Index to track the current chunk

    // Sensor data
    std::vector<UltrasonicSensorData> sensors = {
            {0, 5}, {45, 7}, {90, 4}, {135, 8},
            {180, 6}, {225, 3}, {270, 2}, {315, 5}
    };
    std::mutex logMutex;



};

#endif // SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H
