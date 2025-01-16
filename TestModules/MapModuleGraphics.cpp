#include "MapModuleGraphics.h"
#include <fstream>
#include <iostream>

MapModuleGraphics::MapModuleGraphics()
        : autoscrollEnabled(true),
          scrollbar(100.0f, 0.0f) { // Initialize scrollbar
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

    // Conditionally draw scrollbar if needed
    if (total_log_height > visible_height) {
        scrollbar.drawScrollbar(log_area_min, log_area_max, io);
        scrollbar.updateScrollOffset(io);
    }

    ImVec2 checkbox_position = ImVec2(log_area_max.x - 20.0f, log_area_min.y);
    ImGui::SetCursorScreenPos(checkbox_position);
    ImGui::Checkbox("##AutoscrollCheckbox3", &autoscrollEnabled);
    scrollbar.enableAutoscroll(autoscrollEnabled);
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
