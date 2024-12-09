#include "CounterModule.h"
#include "imgui.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

CounterModule::CounterModule()
        : counter(0), stopGeneration(false) {
    generatorThread = std::thread(&CounterModule::generateNumbers, this);
}

CounterModule::~CounterModule() {
    stopGeneration.store(true);
    if (generatorThread.joinable()) {
        generatorThread.join();

    }
}

void CounterModule::renderStandalone(ImGuiIO io, ImVec2 possition) {
    ImGui::SetNextWindowPos(possition);
    ImGui::SetNextWindowSize(getSize());
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::Text("Standalone Rendering Window");
    ImGui::End();
}


void CounterModule::draw(ImGuiIO &io) {

    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    // Text content
    std::string text = "Counter: " + std::to_string(counter.load());

    // Define the color of the text (white) and border
    ImU32 text_color = IM_COL32(255, 255, 255, 255);
    ImU32 border_color = IM_COL32(255, 255, 255, 255); // White border

    const char* text_cstr = text.c_str();
    ImVec2 text_size = ImGui::CalcTextSize(text_cstr);
    std::cout<<possition.x << "  " <<possition.y<<std::endl;

    ImVec2 rect_min = possition;
    ImVec2 rect_max = ImVec2(possition.x + size.x, possition.y + size.y);
    ImVec2 rect_center = ImVec2((rect_min.x + rect_max.x) / 2.0f, (rect_min.y + rect_max.y) / 2.0f);
    const float border_thickness = 2.0f;

    draw_list->AddRect(possition, size, border_color, 0.0f, 0, border_thickness);
    int text2 = (possition.x + size.x)/ 2.0f;
    int text1 = (possition.y + size.y)/ 2.0f;
    ImVec2 text_pos = ImVec2(text2 - text_size.x/2.0f, text1);

    draw_list->AddText(text_pos, text_color, text_cstr);
}



void CounterModule::generateNumbers() {
    while (!stopGeneration.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        counter.fetch_add(1);
    }
}

void CounterModule::saveLogToJson(const std::vector<int>& values) {
    std::lock_guard<std::mutex> lock(logMutex); // Protect access
    std::ofstream outFile("counter_log.json");
    if (outFile.is_open()) {
        outFile << "{\n  \"counter_values\": [";
        for (size_t i = 0; i < values.size(); ++i) {
            outFile << values[i];
            if (i != values.size() - 1) outFile << ", ";
        }
        outFile << "]\n}";
        outFile.close();
    }
}

std::string CounterModule::getName() const { return name; }
ImVec2 CounterModule::getSize() { return size; }
ImVec2 CounterModule::getPos() { return possition; }
void CounterModule::setPos(ImVec2 pos) { possition = pos; }
void CounterModule::setSize(ImVec2 size) { this->size = size; }

