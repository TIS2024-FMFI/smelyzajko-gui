#include "UltrasonicModuleGraphics.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

UltrasonicModuleGraphics::UltrasonicModuleGraphics()
        : textArea(size.x, 100.0f, "UltrasonicModule") { // Initialize TextArea with default dimensions
    setGraphicElementName("Ultrasonic Module");

    // Initialize previous distances to match initial sensor values
    for (const auto& sensor : sensors) {
        previousDistances.push_back(sensor.distance);
    }
}

void UltrasonicModuleGraphics::draw(ImGuiIO& io) {
    ImVec2 center = ImVec2(position.x + size.x / 2, position.y + size.y / 2);
    float radius = std::min(size.x, size.y) / 3.0f;

    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    // Draw the bounding box and central circle
    ImVec2 topLeft = ImVec2(center.x - radius, center.y - radius);
    ImVec2 bottomRight = ImVec2(center.x + radius, center.y + radius);

    draw_list->AddRect(topLeft, bottomRight, IM_COL32(255, 255, 255, 255), 0.0f, 0, 2.0f);
    draw_list->AddCircle(center, radius, IM_COL32(255, 255, 255, 255), 64);

    // Update sensor values dynamically
    updateDynamicSensors();

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

    // Update TextArea width dynamically to match module width
    textArea.setWidth(size.x);

    // Draw the TextArea below the ultrasonic graphics
    ImVec2 textAreaPosition = ImVec2(position.x, position.y + size.y + 10.0f);
    textArea.drawTextArea(textAreaPosition, io);
}

void UltrasonicModuleGraphics::updateDynamicSensors() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);

    if (elapsed.count() < updateDelayMs) {
        return; // Skip update if delay has not passed
    }

    lastUpdateTime = now; // Reset the timer

    for (size_t i = 0; i < sensors.size(); ++i) {
        auto& sensor = sensors[i];
        float distanceVariation = static_cast<float>(std::rand() % 21 - 10) / 10.0f;
        sensor.distance = std::clamp(sensor.distance + distanceVariation, 0.0f, 10.0f);

        // Log only if the distance has changed
        if (sensor.distance != previousDistances[i]) {
            std::ostringstream logStream;
            logStream << "A: " << sensor.angle << ", D: " << std::fixed << std::setprecision(1) << sensor.distance;
            textArea.addLog(logStream.str()); // Add log to TextArea
            previousDistances[i] = sensor.distance;
        }
    }
}

void UltrasonicModuleGraphics::updateValueOfModule(std::vector<int> value) {
    if (value.size() == sensors.size()) {
        for (size_t i = 0; i < sensors.size(); ++i) {
            sensors[i].distance = value[i];
        }

        textArea.addLog("Sensor data updated.");
    }
}

void UltrasonicModuleGraphics::updateValueOfModule(int value) {
    updateIntervalFrames = value;
    std::ostringstream logStream;
    logStream << "Update interval set to: " << updateIntervalFrames << " frames.";
    textArea.addLog(logStream.str());
}
