// File: TestModules/TestGraphicModule.cpp
#include "TestGraphicModule.h"
#include "imgui.h"
#include <cmath>
#include <iostream>


void TestGraphicModule::renderStandalone() {

    if (counter ==1){
        ImGui::SetNextWindowPos(getPos()); // Set the position of the window
        ImGui::SetNextWindowSize(getSize()); // Set the size of the window
        ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse); // Disable resizing

    }
    else {
        ImGui::Begin(name.c_str());
        setPos(ImGui::GetWindowPos());
        setSize(ImGui::GetWindowSize());
    }

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
std::string TestGraphicModule::getName() const {
    return name;
}

ImVec2 TestGraphicModule::getSize() {
    return size;
}

ImVec2 TestGraphicModule::getPos() {
    return pos;
}

void TestGraphicModule::setPos(ImVec2 pos) {
    TestGraphicModule::pos = pos;
}

void TestGraphicModule::setSize(ImVec2 size) {
    TestGraphicModule::size = size;
}




