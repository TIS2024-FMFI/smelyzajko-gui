#include "UltrasonicModuleGraphics.h"
#include <iostream>
#include <iomanip>
#include <cmath>

UltrasonicModuleGraphics::UltrasonicModuleGraphics()
        : autoscrollEnabled(true),
          scrollbar(100.0f, 0.0f) { // Initialize Scrollbar
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

    // Log area
    float log_area_height = 100.0f;
    float inner_padding = 5.0f;
    ImVec2 log_area_min = ImVec2(position.x, position.y + size.y + 10.0f);
    ImVec2 log_area_max = ImVec2(position.x + size.x, position.y + size.y + log_area_height);

    draw_list->AddRect(log_area_min, log_area_max, IM_COL32(255, 255, 255, 255));

    // Calculate total log height dynamically
    float total_log_height = 0.0f;
    for (const std::string& log : logValues) {
        ImVec2 log_text_size = ImGui::CalcTextSize(log.c_str());
        total_log_height += log_text_size.y + inner_padding;
    }

    // Update scrollbar dimensions
    float visible_height = log_area_height - 2 * inner_padding;
    scrollbar.updateTotalHeight(total_log_height);
    scrollbar.updateVisibleHeight(visible_height);

    // Render logs using the updated scrollOffset
    float log_y_offset = log_area_min.y - scrollbar.getScrollOffset();
    draw_list->PushClipRect(log_area_min, log_area_max, true);
    for (const std::string& log : logValues) {
        ImVec2 log_text_size = ImGui::CalcTextSize(log.c_str());
        if (log_y_offset + log_text_size.y >= log_area_min.y &&
            log_y_offset <= log_area_max.y) {
            ImVec2 log_pos = ImVec2(log_area_min.x + inner_padding, log_y_offset);
            draw_list->AddText(log_pos, IM_COL32(255, 255, 255, 255), log.c_str());
        }
        log_y_offset += log_text_size.y + inner_padding;
    }
    draw_list->PopClipRect();

    // Draw and update the scrollbar
    if (total_log_height > visible_height) {
        scrollbar.drawScrollbar(log_area_min, log_area_max, io);
        scrollbar.updateScrollOffset(io);
    }

    // Checkbox for autoscroll
    ImVec2 checkbox_position = ImVec2(log_area_max.x - 20.0f, log_area_min.y + 5.0f);
    ImGui::SetCursorScreenPos(checkbox_position);
    ImGui::Checkbox("##AutoscrollCheckbox", &autoscrollEnabled);
    scrollbar.enableAutoscroll(autoscrollEnabled);
}





void UltrasonicModuleGraphics::updateDynamicSensors() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);

    if (elapsed.count() < updateDelayMs) {
        return; // Skip update if delay has not passed
    }

    lastUpdateTime = now; // Reset the timer

    std::lock_guard<std::mutex> lock(logMutex); // Ensure thread safety

    for (size_t i = 0; i < sensors.size(); ++i) {
        auto& sensor = sensors[i];
        float distanceVariation = static_cast<float>(std::rand() % 21 - 10) / 10.0f;
        sensor.distance = std::clamp(sensor.distance + distanceVariation, 0.0f, 10.0f);

        // Log only if the distance has changed
        if (sensor.distance != previousDistances[i]) {
            std::ostringstream logStream;
            logStream << "A: " << sensor.angle << ", D: " << std::fixed << std::setprecision(1) << sensor.distance;
            logValues.push_back(logStream.str());
            previousDistances[i] = sensor.distance;
        }
    }
}


void UltrasonicModuleGraphics::updateValueOfModule(std::vector<int> value) {
    if (value.size() == sensors.size()) {
        for (size_t i = 0; i < sensors.size(); ++i) {
            sensors[i].distance = value[i];
        }

        std::lock_guard<std::mutex> lock(logMutex);
        logValues.push_back("Sensor data updated.");
    }
}


void UltrasonicModuleGraphics::updateValueOfModule(int value) {
    updateIntervalFrames = value;
    std::cout << "Update interval set to: " << updateIntervalFrames << " frames." << std::endl;
}

