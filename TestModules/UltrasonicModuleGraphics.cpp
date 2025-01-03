#include "UltrasonicModuleGraphics.h"
#include <cstdlib>
#include <cmath>
void UltrasonicModuleGraphics::draw(ImGuiIO& io) {
    updateRandomSensorData();

    ImVec2 center = ImVec2(position.x + size.x / 2, position.y + size.y / 2);
    float radius = std::min(size.x, size.y) / 3.0f;

    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    ImVec2 topLeft = ImVec2(center.x - radius, center.y - radius);
    ImVec2 bottomRight = ImVec2(center.x + radius, center.y + radius);

    draw_list->AddRect(topLeft, bottomRight, IM_COL32(255, 255, 255, 255), 0.0f, 0, 2.0f);

    draw_list->AddCircle(center, radius, IM_COL32(255, 255, 255, 255), 64);

    for (const auto& sensor : sensors) {
        float angle_rad = sensor.angle * 3.14159265f / 180.0f;
        float normalizedDistance = sensor.distance / 10.0f;
        if (normalizedDistance > 1.0f) normalizedDistance = 1.0f;
        float x = center.x + radius * normalizedDistance * cos(angle_rad);
        float y = center.y - radius * normalizedDistance * sin(angle_rad);
        draw_list->AddLine(center, ImVec2(x, y), IM_COL32(0, 255, 0, 255), 2.0f);
        draw_list->AddCircleFilled(ImVec2(x, y), 4.0f, IM_COL32(0, 255, 0, 255));
    }
}

void UltrasonicModuleGraphics::updateRandomSensorData() {
    frameCounter++;

    if (frameCounter >= updateIntervalFrames) {
        for (auto& sensor : sensors) {
            sensor.distance = static_cast<float>(std::rand() % 1000) / 100.0f;
        }
        frameCounter = 0;
    }
}

