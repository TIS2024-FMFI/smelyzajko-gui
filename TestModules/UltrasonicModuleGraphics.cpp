#include "UltrasonicModuleGraphics.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <fstream>


UltrasonicModuleGraphics::UltrasonicModuleGraphics(){ // Initialize TextArea with default dimensions
    setGraphicElementName("UltrasonicGraphicElement");

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
    }
}



void UltrasonicModuleGraphics::updateValueOfModule(std::vector<float> value) {
    if (value.size() == 2) {
        sensors[i].distance = value[0];
        sensors[i].angle = value[1];
        logToJson();
    } else {
        std::cerr << "[ERROR] Invalid input for updateValueOfModule (vector<float>)." << std::endl;
    }
}

void UltrasonicModuleGraphics::updateValueOfModule(int value) {
    if (value >= 0 && value < sensors.size()) {
        i = value;
        logToJson();
    } else {
        std::cerr << "[ERROR] Invalid index for updateValueOfModule (int)." << std::endl;
    }
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

    std::string filename = logFileDirectory + "/UltrasonicGraphicElement.json";



    nlohmann::json j;

    if (!j.contains("graphicsFrequency")) {
        j["graphicsFrequency"] = getGraphicsFrequency();
    }
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

void UltrasonicModuleGraphics::logFromJson() {
    std::cout << "logFromJson " << logFileDirectory << std::endl;
    std::string filename = logFileDirectory + "/UltrasonicGraphicElement.json";
    nlohmann::json j;

    // Load existing content
    std::ifstream inFile(filename);

    // Check if the file is open
    if (inFile.is_open()) {
        try {
            // Parse the JSON file
            inFile >> j;

            // Clear existing data from sensorsFromLog
            sensorsFromLog.clear();

            // Load sensor data in chunks of 8
            if (j.contains("sensor_data") && j["sensor_data"].is_array()) {
                std::vector<std::pair<float, float>> chunk;
                for (const auto& sensorEntry : j["sensor_data"]) {
                    if (sensorEntry.contains("angle") && sensorEntry.contains("distance")) {
                        chunk.push_back({sensorEntry["angle"], sensorEntry["distance"]});
                    }

                    // Every 8 sensors, push the chunk to the vector and reset the chunk
                    if (chunk.size() == 8) {
                        sensorsFromLog.push_back(chunk);
                        chunk.clear();
                    }
                }

                // Handle case where the last chunk has less than 8 elements
                if (!chunk.empty()) {
                    sensorsFromLog.push_back(chunk);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << std::endl;
        }
        inFile.close();
    } else {
        std::cerr << "[ERROR] Could not open file for reading: UltrasonicGraphicElement.json" << std::endl;
    }

    // Load log frequency
    if (j.contains("graphicsFrequency")) {
        graphicsFrequency = j["graphicsFrequency"];
    } else {
        std::cerr << "Error: No graphicsFrequency found in JSON.\n";
    }
}




void UltrasonicModuleGraphics::logForward() {
    // Check if we have any log data
    if (sensorsFromLog.empty()) {
        std::cerr << "No sensor data to log forward." << std::endl;
        return;
    }

    // Check if we've reached the end of the log
    if (currentSensorIndexLog >= sensorsFromLog.size()) {
        std::cerr << "All sensor data has been logged. Stopping." << std::endl;
        return;  // Stop if we've processed all chunks
    }

    // Get the current chunk of 8 sensors
    const auto& currentChunk = sensorsFromLog[currentSensorIndexLog];

    if (currentChunk.size() != 8) {
        std::cerr << "Unexpected chunk size. Expected 8, but got " << currentChunk.size() << std::endl;
        return;
    }


    // Update the sensor data (in this case, updating each sensor's angle and distance)
    for (size_t i = 0; i < currentChunk.size(); ++i) {
        sensors[i].angle = currentChunk[i].first;
        sensors[i].distance = currentChunk[i].second;
    }

    // Move to the next chunk of 8 sensors
    currentSensorIndexLog++;

    // Optional: Log the current data to JSON after advancing
    logToJson();
}


void UltrasonicModuleGraphics::logBackwards() {
    // Check if we have any log data
    if (sensorsFromLog.empty()) {
        std::cerr << "No sensor data to log backwards." << std::endl;
        return;
    }

    // Check if we've already processed all chunks (we're at the first chunk)
    if (currentSensorIndexLog == 0) {
        std::cerr << "Already at the beginning of sensor data. Stopping." << std::endl;
        return;  // Stop if we're already at the first chunk
    }

    // Get the previous chunk of 8 sensors
    const auto& currentChunk = sensorsFromLog[currentSensorIndexLog - 1];

    // Ensure the chunk size is exactly 8, as expected
    if (currentChunk.size() != 8) {
        std::cerr << "Unexpected chunk size. Expected 8, but got " << currentChunk.size() << std::endl;
        return;
    }

    // Update the sensor data (in this case, updating each sensor's angle and distance)
    for (size_t i = 0; i < currentChunk.size(); ++i) {
        sensors[i].angle = currentChunk[i].first;
        sensors[i].distance = currentChunk[i].second;
    }

    // Move to the previous chunk of 8 sensors
    currentSensorIndexLog--;

    // Optional: Log the current data to JSON after moving backwards
    logToJson();
}

void UltrasonicModuleGraphics::logBackwards() {

}