#include "UltrasonicModuleGraphics.h"
#include <cstdlib>
#include <cmath>

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
}

void UltrasonicModuleGraphics::updateDynamicSensors() {
    static int frameCounter = 0; // Počítadlo snímok

    frameCounter++;

    // Aktualizácia iba po určitom počte snímok
    if (frameCounter >= updateIntervalFrames) {
        frameCounter = 0; // Resetovať počítadlo

        for (auto& sensor : sensors) {
            // Náhodná variácia uhla a vzdialenosti
            sensor.angle += (std::rand() % 11 - 5); // -5° až +5°
            if (sensor.angle >= 360.0f) sensor.angle -= 360.0f;
            if (sensor.angle < 0.0f) sensor.angle += 360.0f;

            float distanceVariation = static_cast<float>(std::rand() % 21 - 10) / 10.0f; // -1.0 až +1.0
            sensor.distance += distanceVariation;
            sensor.distance = std::clamp(sensor.distance, 0.0f, 10.0f); // Orezanie na rozsah [0, 10]
        }
    }
}



void UltrasonicModuleGraphics::updateValueOfModule(std::string value) {
    std::cout << "Updating Ultrasonic Module with string value: " << value << std::endl;
}

void UltrasonicModuleGraphics::updateValueOfModule(int value) {
    updateIntervalFrames = value;
    std::cout << "Update interval set to: " << updateIntervalFrames << " frames." << std::endl;
}

void UltrasonicModuleGraphics::updateValueOfModule(std::vector<int> value) {
    if (value.size() == sensors.size()) {
        for (size_t i = 0; i < sensors.size(); ++i) {
            sensors[i].distance = value[i];
        }
    }
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





