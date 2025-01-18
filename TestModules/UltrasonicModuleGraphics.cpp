#include "UltrasonicModuleGraphics.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <fstream>


UltrasonicModuleGraphics::UltrasonicModuleGraphics(){ // Initialize TextArea with default dimensions
    setGraphicElementName("Ultrasonic Graphic Element");

}

void UltrasonicModuleGraphics::draw(ImGuiIO& io) {
    ImVec2 center = ImVec2(position.x + size.x / 2, position.y + size.y / 2);
    float radius = std::min(size.x, size.y) / 2.0f; // Use half of the smaller dimension

    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    // Draw the bounding box and central circle
    ImVec2 topLeft = ImVec2(center.x - radius, center.y - radius);
    ImVec2 bottomRight = ImVec2(center.x + radius, center.y + radius);

    draw_list->AddRect(topLeft, bottomRight, IM_COL32(255, 255, 255, 255), 0.0f, 0, 2.0f);
    draw_list->AddCircle(center, radius, IM_COL32(255, 255, 255, 255), 64);

    // Draw lines for each sensor
    for (const auto& sensor : sensors) {
        float angle_rad = sensor.angle * 3.14159265f / 180.0f; // Convert angle to radians
        float normalizedDistance = sensor.distance / 10.0f;    // Normalize distance
        normalizedDistance = std::clamp(normalizedDistance, 0.0f, 1.0f); // Clamp to [0, 1]

        // Compute line endpoint
        float x = center.x + radius * normalizedDistance * cos(angle_rad);
        float y = center.y - radius * normalizedDistance * sin(angle_rad);

        // Draw line and endpoint
        draw_list->AddLine(center, ImVec2(x, y), IM_COL32(0, 255, 0, 255), 2.0f); // Green line
        draw_list->AddCircleFilled(ImVec2(x, y), 4.0f, IM_COL32(0, 255, 0, 255)); // Green dot
        logToJson();
    }
}



void UltrasonicModuleGraphics::updateValueOfModule(std::vector<float> value) {
    sensors[i].distance = value[0];
    sensors[i].angle = value[1];
}

void UltrasonicModuleGraphics::updateValueOfModule(int value) {
    i = value;
}

void UltrasonicModuleGraphics::logToJson() {
    if (!isGraphicsLogEnabled()) {
        return;
    }

    static auto lastLogTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastLogTime;

    float frequency = getGraphicsFrequency();
    if (frequency > 0) {
        float interval = 60.0f / frequency;
        if (elapsed.count() < interval) {
            return;
        }
    }

    lastLogTime = currentTime;

    std::lock_guard<std::mutex> lock(logMutex);

    std::string filename = logFileDirectory+"/"+ moduleName+ "/ultrasonic_module_log.json";
    nlohmann::json j;
    std::ifstream inFile(filename);

    if (inFile.is_open()) {
        try {
            inFile >> j;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << std::endl;
        }
        inFile.close();
    }

    if (!j.contains("sensor_data") || !j["sensor_data"].is_array()) {
        j["sensor_data"] = nlohmann::json::array();
    }

    for (const auto& sensor : sensors) {
        nlohmann::json sensorEntry;
        sensorEntry["angle"] = sensor.angle;
        sensorEntry["distance"] = sensor.distance;
        j["sensor_data"].push_back(sensorEntry);
    }

    std::ofstream outFile(filename, std::ios::trunc);
    if (outFile.is_open()) {
        outFile << std::setw(4) << j << std::endl;
        outFile.close();
    } else {
        std::cerr << "[ERROR] Could not open file for writing: " << filename << std::endl;
    }
}

