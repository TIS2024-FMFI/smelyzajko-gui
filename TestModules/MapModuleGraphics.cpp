#include "MapModuleGraphics.h"
#include <fstream>
#include <iostream>

MapModuleGraphics::MapModuleGraphics()
        : scrollOffset(0.0f), autoscrollEnabled(true) {
    setGraphicElementName("Map Graphic Element");
    loadMap();
}

void MapModuleGraphics::loadMap() {
    std::ifstream inFile("../TestModules/map.json");
    if (inFile.is_open()) {
        nlohmann::json j;
        inFile >> j;
        inFile.close();

        if (j.contains("rows") && j.contains("cols") && j.contains("map")) {

            rows = j["rows"];
            cols = j["cols"];
            map.resize(rows, std::vector<int>(cols, 0)); // Initialize new cells to 0

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

void MapModuleGraphics::draw(ImGuiIO &io) {
    loadMap();
    cellSize = std::min(size.x / cols, size.y / rows);

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

    // Render scrollbar only if the total log height exceeds the visible area
    if (total_log_height > visible_height) {
        float scrollbar_width = 20.0f;
        ImVec2 scrollbar_min = ImVec2(log_area_max.x + inner_padding, log_area_min.y);
        ImVec2 scrollbar_max = ImVec2(scrollbar_min.x + scrollbar_width, log_area_max.y);
        draw_list->AddRectFilled(scrollbar_min, scrollbar_max, IM_COL32(180, 180, 180, 255));

        float scrollbar_thumb_height = std::max((visible_height / total_log_height) * visible_height, 10.0f);
        scrollbar_thumb_height = std::min(scrollbar_thumb_height, visible_height); // Cap thumb height to visible area
        float thumb_offset = (scrollOffset / std::max(1.0f, total_log_height)) * visible_height;
        thumb_offset = std::clamp(thumb_offset, 0.0f, visible_height - scrollbar_thumb_height);

        ImVec2 thumb_min = ImVec2(scrollbar_min.x, scrollbar_min.y + thumb_offset);
        ImVec2 thumb_max = ImVec2(scrollbar_max.x, thumb_min.y + scrollbar_thumb_height);
        draw_list->AddRectFilled(thumb_min, thumb_max, IM_COL32(100, 100, 100, 255));

        // Manual scrollbar interaction
        if (!autoscrollEnabled && ImGui::IsMouseHoveringRect(scrollbar_min, scrollbar_max) &&
            ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            ImVec2 mouse_pos = io.MousePos;
            float mouse_scroll_position = mouse_pos.y - scrollbar_min.y - scrollbar_thumb_height / 2;
            scrollOffset = (mouse_scroll_position / visible_height) * total_log_height;
            scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, total_log_height - visible_height));
        }
    }

    // Handle mouse wheel scrolling
    if (ImGui::IsMouseHoveringRect(log_area_min, log_area_max)) {
        scrollOffset -= io.MouseWheel * 20.0f;
        scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, total_log_height - visible_height));
    }

    // Autoscroll logic
    if (autoscrollEnabled && !logValues.empty()) {
        scrollOffset = std::max(0.0f, total_log_height - visible_height);
    }

    // Autoscroll toggle checkbox in the top-right corner of the log area
    ImVec2 checkbox_position = ImVec2(log_area_max.x - 20.0f, log_area_min.y + 5.0f);
    ImGui::SetCursorScreenPos(checkbox_position);
    ImGui::Checkbox("##AutoscrollCheckbox3", &autoscrollEnabled); // Invisible label with a unique ID
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





