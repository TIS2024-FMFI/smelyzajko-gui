#include "UltrasonicModuleGraphics.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>

UltrasonicModuleGraphics::UltrasonicModuleGraphics()
        : scrollOffset(0.0f), autoscrollEnabled(true) {
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

    // Scrollable log for sensor data
    float log_area_height = 100.0f;
    float inner_padding = 5.0f;
    ImVec2 log_area_min = ImVec2(position.x, position.y + size.y + 10.0f);
    ImVec2 log_area_max = ImVec2(position.x + size.x - 25.0f, position.y + size.y + log_area_height); // Adjusted for scrollbar space

    draw_list->AddRect(log_area_min, log_area_max, IM_COL32(255, 255, 255, 255));

    float visible_height = log_area_height - 2 * inner_padding;
    float total_log_height = 0.0f;
    float log_y_offset = log_area_min.y - scrollOffset;

    // Enable scissor/clipping for log area
    draw_list->PushClipRect(log_area_min, log_area_max, true);

    for (const std::string& log : logValues) {
        ImVec2 log_text_size = ImGui::CalcTextSize(log.c_str());
        total_log_height += log_text_size.y + inner_padding;

        if (log_y_offset + log_text_size.y >= log_area_min.y &&
            log_y_offset <= log_area_max.y) {
            ImVec2 log_pos = ImVec2(log_area_min.x + inner_padding, log_y_offset);
            draw_list->AddText(log_pos, IM_COL32(255, 255, 255, 255), log.c_str());
        }
        log_y_offset += log_text_size.y + inner_padding;
    }

    draw_list->PopClipRect();

    // Scrollbar
    float scrollbar_width = 20.0f;
    ImVec2 scrollbar_min = ImVec2(log_area_max.x + 5.0f, log_area_min.y); // Positioned slightly to the side
    ImVec2 scrollbar_max = ImVec2(log_area_max.x + 5.0f + scrollbar_width, log_area_max.y);
    draw_list->AddRectFilled(scrollbar_min, scrollbar_max, IM_COL32(180, 180, 180, 255));

    // Calculate the scrollbar thumb height and position
    float scrollbar_thumb_height = std::max((visible_height / total_log_height) * visible_height, 10.0f);
    float thumb_offset = (scrollOffset / total_log_height) * visible_height;
    thumb_offset = std::clamp(thumb_offset, 0.0f, visible_height - scrollbar_thumb_height);

    ImVec2 thumb_min = ImVec2(scrollbar_min.x, scrollbar_min.y + thumb_offset);
    ImVec2 thumb_max = ImVec2(scrollbar_max.x, thumb_min.y + scrollbar_thumb_height);
    draw_list->AddRectFilled(thumb_min, thumb_max, IM_COL32(100, 100, 100, 255));

    // Handle interactions
    if (ImGui::IsMouseHoveringRect(scrollbar_min, scrollbar_max) && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        ImVec2 mouse_pos = io.MousePos;
        float mouse_scroll_position = mouse_pos.y - scrollbar_min.y - scrollbar_thumb_height / 2;
        scrollOffset = (mouse_scroll_position / visible_height) * total_log_height;
        scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, total_log_height - visible_height));
    }

    if (ImGui::IsMouseHoveringRect(log_area_min, log_area_max)) {
        scrollOffset -= io.MouseWheel * 20.0f;
        scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, total_log_height - visible_height));
    }

    // Autoscroll logic
    if (autoscrollEnabled && !logValues.empty()) {
        scrollOffset = std::max(0.0f, total_log_height - visible_height);
    }

    // Checkbox for autoscroll (top-right corner)
    ImGui::SetCursorScreenPos(ImVec2(log_area_max.x - 20.0f, log_area_min.y + 5.0f));
    ImGui::Checkbox("##AutoscrollCheckbox", &autoscrollEnabled); // Invisible label with a unique ID
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
            logStream << "Angle: " << sensor.angle << ", Dist: " << std::fixed << std::setprecision(1) << sensor.distance;
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

void UltrasonicModuleGraphics::updateValueOfModule(std::string value) {
    std::cout << "Updating Ultrasonic Module with string value: " << value << std::endl;
}

void UltrasonicModuleGraphics::updateValueOfModule(int value) {
    updateIntervalFrames = value;
    std::cout << "Update interval set to: " << updateIntervalFrames << " frames." << std::endl;
}


void UltrasonicModuleGraphics::from_json(const nlohmann::json &j) {
    if (j.contains("graphicModuleId") && j["graphicModuleId"].is_number_integer()) {
        graphicModuleId = j["graphicModuleId"];
    }
    if (j.contains("moduleId") && j["moduleId"].is_number_integer()) {
        moduleId = j["moduleId"];
    }
    if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2) {
        position.x = j["position"][0];
        position.y = j["position"][1];
        setPos({j["position"][0], j["position"][1]});
    }
    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
        setSize({j["size"][0], j["size"][1]});
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

void UltrasonicModuleGraphics::to_json(nlohmann::json &j) const {
    j["graphicModuleId"] = graphicModuleId;
    j["moduleId"] = moduleId;
    j["name"] = moduleName;
    j["position"] = {position.x, position.y};
    j["size"] = {size.x, size.y};
    j["graphicsFrequency"] = graphicsFrequency;
    j["graphicsLogEnabled"] = graphicsLogEnabled;
    j["textFrequency"] = textFrequency;
    j["textLogEnabled"] = textLogEnabled;
}





