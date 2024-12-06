// File: TestModules/TestGraphicModule.cpp
#include "TestGraphicModule.h"
#include "imgui.h"
#include <cmath>
#include <iostream>

void TestGraphicModule::draw(ImGuiIO& io){


}
void TestGraphicModule::renderStandalone(ImGuiIO io, ImVec2 possition) {

    if (counter ==1){
        ImGui::SetNextWindowPos(getPos()); // Set the position of the window
        ImGui::SetNextWindowSize(getSize()); // Set the size of the window
        ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar |    // Remove title bar
                                            ImGuiWindowFlags_NoCollapse |   // Prevent collapsing
                                            ImGuiWindowFlags_NoResize |     // Disable resizing
                                            ImGuiWindowFlags_NoBringToFrontOnFocus | // Prevent window focus changes
                                            ImGuiWindowFlags_NoScrollbar |   // Disable scrollbar (optional)
                                            ImGuiWindowFlags_NoBackground); // Disable resizing

    }
    else {
        ImGui::Begin(name.c_str());
        setPos(ImGui::GetWindowPos());
        setSize(ImGui::GetWindowSize());
    }

    static float phase = 0.0f;
    phase += 0.01f;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float width = ImGui::GetContentRegionAvail().x;
    float height = 150.0f;
    draw_list->AddRectFilled(possition, ImVec2(possition.x + width, possition.y + height), IM_COL32(200, 200, 200, 255));

    for (int i = 0; i < width; ++i) {
        float x = i / width;
        float y = 0.5f * sinf(10.0f * x + phase) + 0.5f;
        draw_list->AddCircleFilled(ImVec2(possition.x + i, possition.y + height * (1 - y)), 2.0f, IM_COL32(255, 0, 0, 255));
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
    return possition;
}

void TestGraphicModule::setPos(ImVec2 pos) {
    TestGraphicModule::possition = pos;
}

void TestGraphicModule::setSize(ImVec2 size) {
    TestGraphicModule::size = size;
}