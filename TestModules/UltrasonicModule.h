#ifndef ULTRASONIC_MODULE_H
#define ULTRASONIC_MODULE_H

#include "imgui.h"
#include <vector>
#include "../src/Module.h"

struct UltrasonicSensorData {
    float angle;
    float distance;
};

class UltrasonicModule : public Module{

private:
    int frameCounter = 0;
    int updateIntervalFrames = 180;
    ImVec2 position;
    ImVec2 size;
    std::vector<UltrasonicSensorData> sensors;


public:

    UltrasonicModule();
    void setPos(ImVec2 pos) override;
    void setSize(ImVec2 size) override;
    void draw(ImGuiIO& io) override;
    void renderStandalone(ImGuiIO io, ImVec2 position) override;
    ImVec2 getSize() override;
    ImVec2 getPos() override;
    std::string getName() const override;
    void updateRandomSensorData();
};

#endif // ULTRASONIC_MODULE_H
