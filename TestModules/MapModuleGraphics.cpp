#include "MapModuleGraphics.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

MapModuleGraphics::MapModuleGraphics()
         { // Initialize TextArea with default dimensions
    setGraphicElementName("MapGraphicElement");

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

    draw_list->AddCircleFilled(
            ImVec2(position.x + ballCol * cellSize + cellSize / 2, position.y + ballRow * cellSize + cellSize / 2),
            cellSize / 4, IM_COL32(255, 0, 0, 255)
    );
}

void MapModuleGraphics::updateValueOfModule(std::vector<int> value) {
    if (value.size() == 3) {
        rows = value[0];
        cols = value[1];
    }else if (value.size() == 2) {
        ballRow = value[0];
        ballCol = value[1];
        //textArea.addLog("Moved to: (" + std::to_string(ballRow) + ", " + std::to_string(ballCol) + ")");
    } else {
        std::cerr << "Invalid value for MapModuleGraphics." << std::endl;
    }

}
void MapModuleGraphics::startLoggingThread() {
    if (!graphicsLogEnabled){
        return;
    }
    loggingThreadRunning = true;

    loggingThread = std::thread(&MapModuleGraphics::loggingThreadFunction, this);
}
void MapModuleGraphics::stopLoggingThread() {
    loggingThreadRunning = false;
    if (loggingThread.joinable()) {
        loggingThread.join();
    }
}
void MapModuleGraphics::loggingThreadFunction() {
    std::chrono::milliseconds interval;

    if (graphicsFrequency > 0) {
        interval = std::chrono::milliseconds(60000 / graphicsFrequency);
    } else {
        return;
    }
    while (loggingThreadRunning) {
        logToJson();
        std::this_thread::sleep_for(interval);
    }
}


void MapModuleGraphics::logToJson() {
    if (!graphicsLogEnabled){
        return;
    }
    std::lock_guard<std::mutex> lock(logMutex);

    std::string filename = logFileDirectory + "/MapGraphicElement.json";
    nlohmann::json j;

    // Load existing content
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> j;
        } catch (const std::exception& e) {
            std::cerr << "Error: Failed to parse JSON: " << e.what() << std::endl;
        }
        inFile.close();
    }

    if (!mapSaved) {
        if (!j.contains("graphicsFrequency")) {
            j["graphicsFrequency"] = getGraphicsFrequency();
        }
        j["rows"] = rows;
        j["cols"] = cols;
        j["map"] = nlohmann::json::array();
        for (int row = 0; row < rows; ++row) {
            j["map"][row] = nlohmann::json::array();
            for (int col = 0; col < cols; ++col) {
                j["map"][row][col] = map[row][col];
            }
        }
        mapSaved = true;
    }

    // Log the ball's position
    if (!j.contains("ball_positions")) {
        j["ball_positions"] = nlohmann::json::array();
    }
    j["ball_positions"].push_back({{"row", ballRow}, {"col", ballCol}});

    // Overwrite the file with updated content
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << std::setw(4) << j << std::endl;
        outFile.close();
    } else {
        std::cerr << "Error: Could not open map_logs.json for writing.\n";
    }
}

void MapModuleGraphics::updateValueOfModule(std::vector<std::vector<int>> value) {
    map = value;
}

void MapModuleGraphics::logFromJson() {
    std::string filename = logFileDirectory + "/MapGraphicElement.json";
    nlohmann::json j;

    // Load existing content
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> j;
        } catch (const std::exception& e) {
            std::cerr << "Error: Failed to parse JSON: " << e.what() << std::endl;
        }
        inFile.close();
    } else {
        std::cerr << "Error: Could not open " << filename << " for reading.\n";
        return;
    }

    // Load ball positions
    if (j.contains("ball_positions")) {
        ballPositionsFromLog.clear();
        for (const auto& pos : j["ball_positions"]) {
            ballPositionsFromLog.push_back({pos["row"], pos["col"]});
        }
    } else {
        std::cerr << "Error: No ball positions found in JSON.\n";
    }

    // Load map
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
        std::cerr << "Error: Invalid map format in JSON file.\n";
    }

    // Load log frequency
    if (j.contains("graphicsFrequency")) {
        graphicsFrequency = j["graphicsFrequency"];
    } else {
        std::cerr << "Error: No graphicsFrequency found in JSON.\n";
    }
}

void MapModuleGraphics::logForward() {
    if (ballPositionsFromLog.empty()) {
        std::cerr << "Error: No ball positions loaded.\n";
        return;
    }

    if (currentBallPositionIndexLog + 1 < ballPositionsFromLog.size()) {
        currentBallPositionIndexLog++;
        ballRow = ballPositionsFromLog[currentBallPositionIndexLog][0];
        ballCol = ballPositionsFromLog[currentBallPositionIndexLog][1];
    } else {
    }
}

void MapModuleGraphics::logBackwards() {

    if (ballPositionsFromLog.empty()) {
        std::cerr << "Error: No ball positions loaded.\n";
        return;
    }

    if (currentBallPositionIndexLog > 0) {
        currentBallPositionIndexLog--;
        ballRow = ballPositionsFromLog[currentBallPositionIndexLog][0];
        ballCol = ballPositionsFromLog[currentBallPositionIndexLog][1];
    } else {
    }
}