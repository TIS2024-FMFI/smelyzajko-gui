#pragma once
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

    ImVec2 position;
    ImVec2 size;
    std::vector<UltrasonicSensorData> sensors;
    std::string name = "Counter Module";


public:
    UltrasonicModule();


};

#endif // ULTRASONIC_MODULE_H
