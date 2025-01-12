#include "MapModuleGraphics.h"
#include <fstream>
//#include <nlohmann/json.hpp>
#include <iostream>

MapModuleGraphics::MapModuleGraphics() {
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

void MapModuleGraphics::updateValueOfModule(std::vector<int> value) {
    if (value.size() == 2) {
        ballRow = value[0];
        ballCol = value[1];
    }
    else {
        std::cerr << "Invalid value for MapModuleGraphics." << std::endl;
    }
}

