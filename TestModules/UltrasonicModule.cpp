#include "UltrasonicModule.h"
#include "imgui.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "UltrasonicModule.h"
#include <cmath>

UltrasonicModule::UltrasonicModule()
        : position(ImVec2(0, 0)), size(ImVec2(300, 300)) {
    sensors = {
            {0, 5}, {45, 7}, {90, 4}, {135, 8},
            {180, 6}, {225, 3}, {270, 2}, {315, 5}
    };
}

void UltrasonicModule::updateRandomSensorData() {
    frameCounter++;

    if (frameCounter >= updateIntervalFrames) {
        for (auto& sensor : sensors) {
            sensor.distance = static_cast<float>(std::rand() % 1000) / 100.0f;
        }
        frameCounter = 0;
    }
}

void UltrasonicModule::setPos(ImVec2 pos) {
    position = pos;
}

void UltrasonicModule::setSize(ImVec2 sz) {
    size = sz;
}

void UltrasonicModule::draw(ImGuiIO& io) {
    updateRandomSensorData();

    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Ultrasonic Sensor Visualization", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImVec2 center = ImVec2(position.x + size.x / 2, position.y + size.y / 2);
    float radius = std::min(size.x, size.y) / 3.0f;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

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

    ImGui::End();
}


void UltrasonicModule::renderStandalone(ImGuiIO io, ImVec2 position) {
    setPos(position);
    draw(io);
}

ImVec2 UltrasonicModule::getSize() {
    return size;
}

ImVec2 UltrasonicModule::getPos() {
    return position;
}

std::string UltrasonicModule::getName() const {
    return "UltrasonicModule";
}
