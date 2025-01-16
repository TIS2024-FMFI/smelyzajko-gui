// File: TestModules/TestGraphicModule.cpp
#include "TestGraphicModule.h"
#include "imgui.h"
#include <cmath>
#include <iostream>

//void TestGraphicModule::draw(ImGuiIO& io){
//    renderStandalone(io, ImVec2(0,0));
//
//}
//void TestGraphicModule::renderStandalone(ImGuiIO io, ImVec2 possition) {
//
//    ImGui::SetNextWindowPos(getPos());// Set the position of the window
//    static float phase = 0.0f;
//    ImGui::SetNextWindowSize(getSize());// Set the size of the window
//    phase += 0.01f;
//    ImGui::Begin(moduleName.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar |    // Remove title bar
//                                              ImGuiWindowFlags_NoCollapse |   // Prevent collapsing
//                                              ImGuiWindowFlags_NoResize |     // Disable resizing
//                                              ImGuiWindowFlags_NoBringToFrontOnFocus | // Prevent window focus changes
//                                              ImGuiWindowFlags_NoScrollbar |   // Disable scrollbar (optional)
//                                              ImGuiWindowFlags_NoBackground);// Disable resizing
//
//    ImDrawList* draw_list = ImGui::GetWindowDrawList();
//    float width = ImGui::GetContentRegionAvail().x;
//    float height = 150.0f;
//    draw_list->AddRectFilled(possition, ImVec2(possition.x + width, possition.y + height), IM_COL32(200, 200, 200, 255));
//
//    for (int i = 0; i < width; ++i) {
//        float x = i / width;
//        float y = 0.5f * sinf(10.0f * x + phase) + 0.5f;
//        draw_list->AddCircleFilled(ImVec2(possition.x + i, possition.y + height * (1 - y)), 2.0f, IM_COL32(255, 0, 0, 255));
//    }
//
//    //TestGraphicModule testGraphicModule;
//    ImGui::End();
//}

void TestGraphicModule::run() {

}



