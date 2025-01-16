#include "MapModuleGraphics.h"
#include <fstream>
#include <iostream>

MapModuleGraphics::MapModuleGraphics() {
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
    }
    else {
        throw std::invalid_argument("Failed to open 'map.json'.");
    }

}

void MapModuleGraphics::draw(ImGuiIO &io) {
    loadMap();
    cellSize = std::min(size.x / cols, size.y / rows);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            ImU32 color;

            // Zvýraznenie cieľového štvorčeka
            if (row == rows - 2 && col == cols - 2) {
                color = IM_COL32(0, 255, 0, 255); // Zelená farba pre cieľ
            } else if (map[row][col] == 1) {
                color = IM_COL32(50, 50, 50, 255); // Múry
            } else {
                color = IM_COL32(200, 200, 200, 255); // Cesty
            }

            draw_list->AddRectFilled(
                ImVec2(position.x + col * cellSize, position.y + row * cellSize),
                ImVec2(position.x + (col + 1) * cellSize, position.y + (row + 1) * cellSize),
                color
            );
        }
    }

    draw_list->AddCircleFilled(
        ImVec2(position.x + ballCol * cellSize + cellSize / 2, position.y + ballRow * cellSize + cellSize / 2),
        cellSize / 4, IM_COL32(255, 0, 0, 255)
    );
}



void MapModuleGraphics::updateValueOfModule(std::vector<int> value) {
    if (value.size() == 2) {
        ballRow = value[0];
        ballCol = value[1];
    }
    else {
        std::cerr << "Invalid value for MapModuleGraphics." << std::endl;
    }
}

