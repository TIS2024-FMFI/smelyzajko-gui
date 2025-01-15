#include "MapModuleGraphics.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

MapModuleGraphics::MapModuleGraphics()
        : scrollOffset(0.0f), autoscrollEnabled(true) {
    loadMap();
}

void MapModuleGraphics::loadMap() {
    std::ifstream inFile("map.json");
    if (inFile.is_open()) {
        nlohmann::json j;
        inFile >> j;
        inFile.close();

        if (j.contains("rows") && j.contains("cols") && j.contains("map")) {
            rows = j["rows"];
            cols = j["cols"];
            map.resize(rows, std::vector<int>(cols, 0));

            for (int row = 0; row < rows; ++row) {
                for (int col = 0; col < cols; ++col) {
                    map[row][col] = j["map"][row][col];
                }
            }
        } else {
            std::cerr << "Invalid map format in JSON file." << std::endl;
        }
    } else {
        throw std::invalid_argument("Failed to open 'map.json'.");
    }
}

void MapModuleGraphics::draw(ImGuiIO& io) {
    loadMap();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Draw map
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            ImU32 color;

            if (row == rows - 2 && col == cols - 2) {
                color = IM_COL32(0, 255, 0, 255); // Goal
            } else if (map[row][col] == 1) {
                color = IM_COL32(50, 50, 50, 255); // Walls
            } else {
                color = IM_COL32(200, 200, 200, 255); // Paths
            }

            draw_list->AddRectFilled(
                    ImVec2(position.x + col * cellSize, position.y + row * cellSize),
                    ImVec2(position.x + (col + 1) * cellSize, position.y + (row + 1) * cellSize),
                    color
            );
        }
    }

    // Draw ball
    draw_list->AddCircleFilled(
            ImVec2(position.x + ballCol * cellSize + cellSize / 2, position.y + ballRow * cellSize + cellSize / 2),
            cellSize / 4, IM_COL32(255, 0, 0, 255)
    );

    // Log area
    float log_area_height = 100.0f;
    float inner_padding = 5.0f;
    ImVec2 log_area_min = ImVec2(position.x, position.y + rows * cellSize + 10.0f);
    ImVec2 log_area_max = ImVec2(position.x + cols * cellSize, position.y + rows * cellSize + log_area_height);

    draw_list->AddRect(log_area_min, log_area_max, IM_COL32(255, 255, 255, 255));

    // Scroll logs
    float visible_height = log_area_height - 2 * inner_padding;
    float total_log_height = 0.0f;
    float log_y_offset = log_area_min.y - scrollOffset;

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
    ImVec2 scrollbar_min = ImVec2(log_area_max.x + 10.0f, log_area_min.y); // Positioned 10px to the right
    ImVec2 scrollbar_max = ImVec2(scrollbar_min.x + scrollbar_width, log_area_max.y);
    draw_list->AddRectFilled(scrollbar_min, scrollbar_max, IM_COL32(180, 180, 180, 255));

    // Calculate the scrollbar thumb height and position
    float scrollbar_thumb_height = std::max((visible_height / total_log_height) * visible_height, 10.0f);
    scrollbar_thumb_height = std::min(scrollbar_thumb_height, visible_height); // Cap thumb height to visible area
    float thumb_offset = (scrollOffset / std::max(1.0f, total_log_height)) * visible_height;
    thumb_offset = std::clamp(thumb_offset, 0.0f, visible_height - scrollbar_thumb_height);

    ImVec2 thumb_min = ImVec2(scrollbar_min.x, scrollbar_min.y + thumb_offset);
    ImVec2 thumb_max = ImVec2(scrollbar_max.x, thumb_min.y + scrollbar_thumb_height);
    draw_list->AddRectFilled(thumb_min, thumb_max, IM_COL32(100, 100, 100, 255));

    // Handle manual scrolling
    if (ImGui::IsMouseHoveringRect(log_area_min, log_area_max)) {
        scrollOffset -= io.MouseWheel * 20.0f;
    }

    // Manual scrollbar interaction
    if (ImGui::IsMouseHoveringRect(scrollbar_min, scrollbar_max) &&
        ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        ImVec2 mouse_pos = io.MousePos;
        float mouse_scroll_position = mouse_pos.y - scrollbar_min.y - scrollbar_thumb_height / 2;
        scrollOffset = (mouse_scroll_position / visible_height) * total_log_height;
    }

    // Clamp scroll offset
    scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, total_log_height - visible_height));

    // Autoscroll logic
    if (autoscrollEnabled && !logValues.empty()) {
        scrollOffset = std::max(0.0f, total_log_height - visible_height);
    }

    // Checkbox for autoscroll
    ImGui::SetCursorScreenPos(ImVec2(log_area_min.x, log_area_max.y + 10.0f));
    ImGui::Checkbox("Enable Map Autoscroll ", &autoscrollEnabled);
}




void MapModuleGraphics::updateValueOfModule(std::vector<int> value) {
    if (value.size() == 2) {
        ballRow = value[0];
        ballCol = value[1];
        std::lock_guard<std::mutex> lock(logMutex);
        logValues.push_back("Moved to: (" + std::to_string(ballRow) + ", " + std::to_string(ballCol) + ")");
        if (logValues.size() > 100) {
            logValues.erase(logValues.begin());
        }
    } else {
        std::cerr << "Invalid value for MapModuleGraphics." << std::endl;
    }
}




void MapModuleGraphics::from_json(const nlohmann::json &j) {
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

void MapModuleGraphics::to_json(nlohmann::json &j) const {
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

void MapModuleGraphics::updateValueOfModule(int value) {
    std::cerr << "Invalid value for MapModuleGraphics." << std::endl;
}

void MapModuleGraphics::updateValueOfModule(std::string value) {
    std::cerr << "Invalid value for MapModuleGraphics." << std::endl;
}


