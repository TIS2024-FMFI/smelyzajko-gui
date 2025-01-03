#include "UltrasonicModule.h"
#include "imgui.h"
#include <cstdlib>
#include <cmath>

UltrasonicModule::UltrasonicModule()
        : position(ImVec2(0, 0)), size(ImVec2(300, 300)) {
    sensors = {
            {0, 5}, {45, 7}, {90, 4}, {135, 8},
            {180, 6}, {225, 3}, {270, 2}, {315, 5}
    };
}









