// File: TestModules/TestGraphicModule.cpp
#include "TestGraphicModule.h"
#include "imgui.h"
#include <cmath>

void TestGraphicModule::renderStandalone() {
    ImGui::Begin("Sinusoida");
    static float phase = 0.0f;
    phase += 0.01f;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    float width = ImGui::GetContentRegionAvail().x;
    float height = 150.0f;
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), IM_COL32(200, 200, 200, 255));

    for (int i = 0; i < width; ++i) {
        float x = i / width;
        float y = 0.5f * sinf(10.0f * x + phase) + 0.5f;
        draw_list->AddCircleFilled(ImVec2(p.x + i, p.y + height * (1 - y)), 2.0f, IM_COL32(255, 0, 0, 255));
    }

    TestGraphicModule testGraphicModule;
    ImGui::End();
}


