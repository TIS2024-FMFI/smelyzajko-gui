// File: TestModules/TestGraphicModule.cpp
#include "TestGraphicModule.h"
#include "imgui.h"
#include <cmath>
#include <iostream>

void TestGraphicModule::draw(ImGuiIO& io){
    renderStandalone(io, ImVec2(0,0));

}
void TestGraphicModule::renderStandalone(ImGuiIO io, ImVec2 possition) {

    if (1 ==1){
        ImGui::SetNextWindowPos(getPos()); // Set the position of the window
        ImGui::SetNextWindowSize(getSize()); // Set the size of the window
        ImGui::Begin(moduleName.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar |    // Remove title bar
                                            ImGuiWindowFlags_NoCollapse |   // Prevent collapsing
                                            ImGuiWindowFlags_NoResize |     // Disable resizing
                                            ImGuiWindowFlags_NoBringToFrontOnFocus | // Prevent window focus changes
                                            ImGuiWindowFlags_NoScrollbar |   // Disable scrollbar (optional)
                                            ImGuiWindowFlags_NoBackground); // Disable resizing

    }
    else {
        ImGui::Begin(moduleName.c_str());
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

    //TestGraphicModule testGraphicModule;
    ImGui::End();
}
std::string TestGraphicModule::getName() const {
    return moduleName;
}

ImVec2 TestGraphicModule::getSize() {
    return size;
}

ImVec2 TestGraphicModule::getPos() {
    return position;
}

void TestGraphicModule::setPos(ImVec2 pos) {
    TestGraphicModule::position = pos;
}

void TestGraphicModule::setSize(ImVec2 size) {
    TestGraphicModule::size = size;
}

//void TestGraphicModule::setModuleActive(bool active) {
//    isModuleActive = active;
//}
//
//bool TestGraphicModule::getModuleActive() {
//        return isModuleActive;
//}

void TestGraphicModule::to_json(nlohmann::json &j) const {
    j["id"] = moduleId;
    j["name"] = moduleName;
    j["position"] = {position.x, position.y};
    j["size"] = {size.x, size.y};
    j["graphicsFrequency"] = graphicsFrequency;
    j["graphicsLogEnabled"] = graphicsLogEnabled;
    j["textFrequency"] = textFrequency;
    j["textLogEnabled"] = textLogEnabled;
}

void TestGraphicModule::from_json(const nlohmann::json &j) {
    if (j.contains("id") && j["id"].is_number_integer()) {
        moduleId = j["id"];
    }

    if (j.contains("name") && j["name"].is_string()) {
        moduleName = j["name"];
    }

    if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2) {
        position.x = j["position"][0];
        position.y = j["position"][1];
    }

    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
    }

    if (j.contains("graphicsFrequency") && j["graphicsFrequency"].is_number_float()) {
        graphicsFrequency = j["graphicsFrequency"];
    }

    if (j.contains("graphicsLogEnabled") && j["graphicsLogEnabled"].is_boolean()) {
        graphicsLogEnabled = j["graphicsLogEnabled"];
    }

    if (j.contains("textFrequency") && j["textFrequency"].is_number_float()) {
        textFrequency = j["textFrequency"];
    }

    if (j.contains("textLogEnabled") && j["textLogEnabled"].is_boolean()) {
        textLogEnabled = j["textLogEnabled"];
    }
}



void TestGraphicModule::run() {

}

