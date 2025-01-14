#include "MapModule.h"
#include <vector>
#include <fstream>
#include <random>
#include <sstream>
#include <iostream>
#include <cmath> // Pre výpočet vzdialenosti

MapModule::MapModule(ModuleManager* moduleManager) : moduleManager(*moduleManager), running(false), deltaTime(0.0f){ // Initialize rows and cols
    moduleId = this->moduleManager.gegisterModule("Map Module",  this);
    graphicModuleId = this->moduleManager.registerGraphicModule("Map Module", moduleId);

    if (!mapInitialized) {
        generatePassableMap();

        mapInitialized = true;
    }
    if (path.empty()) {
        generatePath();
    }
    saveMapToJson();
    running = true;
    mapThread = std::thread(&MapModule::run, this);
}
MapModule:: ~MapModule() {
    running = false;
    if (mapThread.joinable()) {
        mapThread.join();
    }
}



void MapModule::run() {
    auto start = std::chrono::high_resolution_clock::now();
    while (running) {
        auto loopStart = std::chrono::high_resolution_clock::now();

        // Your existing run logic here...
        if (!path.empty() && !isStopped) {
            static float timeAccumulator = 0.0f;
            timeAccumulator += 0.009f; // Delta time

            if (timeAccumulator > 0.5f) {

                timeAccumulator = 0.0f;
                if (currentStep < path.size() - 1) {
                    currentStep++;
                    logToJson(path[currentStep]); // Logovanie aktuálnej pozície do JSON
                    int ballRow = path[currentStep].first;
                    int ballCol = path[currentStep].second;
                    moduleManager.updateValueOfModule(moduleId, std::vector<int>{ballRow, ballCol});
                }
            }
        }

        auto loopEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = loopEnd - loopStart;
        deltaTime = duration.count();
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Sleep to limit the loop to ~60 FPS
    }
}

    void MapModule::generatePassableMap() {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        map.resize(rows, std::vector<int>(cols, 0));
        while (true) {
            // Generovanie náhodnej mapy
            for (int row = 0; row < rows; ++row) {
                for (int col = 0; col < cols; ++col) {
                    if (row == 0 || row == rows - 1 || col == 0 || col == cols - 1) {
                        map[row][col] = 1; // Hranice sú vždy múry
                    } else {
                        map[row][col] = dis(gen);
                    }
                }
            }
            // Cieľová pozícia musí byť priechodná
            map[rows - 2][cols - 2] = 0;

            // Nájdeme priechodné políčko čo najďalej od cieľa
            int maxDistance = -1;
            std::pair<int, int> farthestPosition;

            for (int row = 1; row < rows - 1; ++row) {
                for (int col = 1; col < cols - 1; ++col) {
                    if (map[row][col] == 0) { // Priechodné políčko
                        int distance =
                                std::abs(row - (rows - 2)) + std::abs(col - (cols - 2)); // Manhattanova vzdialenosť
                        if (distance > maxDistance) {
                            maxDistance = distance;
                            farthestPosition = {row, col};
                        }
                    }
                }
            }

            // Ak existuje vzdialená pozícia, nastavíme ju ako počiatočnú
            if (maxDistance != -1) {
                startPosition = farthestPosition;

                // Overíme, že existuje cesta z počiatočnej pozície do cieľa
                generatePath();

                if (!path.empty()) {
                    break;
                }
            }
        }
    }

#include <queue>
#include <vector>
#include <iostream>

void MapModule::generatePath() {
    path.clear();
    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<int, int>>> parent(rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));

    // Štartovacia pozícia
    int startRow = startPosition.first;
    int startCol = startPosition.second;
    int targetRow = rows - 2;
    int targetCol = cols - 2;

    // Skontrolujeme, či je cieľové políčko priechodné
    if (map[targetRow][targetCol] != 0) {
        std::cerr << "[ERROR] Target square is not passable!" << std::endl;
        return;
    }

    // Inicializácia BFS
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;

    const std::vector<std::pair<int, int>> directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0} // Pravá, Dole, Ľavá, Hore
    };

    bool found = false;

    // BFS algoritmus
    while (!q.empty() && !found) {
        auto [currentRow, currentCol] = q.front();
        q.pop();

        for (const auto& [dRow, dCol] : directions) {
            int nextRow = currentRow + dRow;
            int nextCol = currentCol + dCol;

            // Kontrola hraníc mapy a priechodnosti
            if (nextRow >= 0 && nextRow < rows && nextCol >= 0 && nextCol < cols &&
                map[nextRow][nextCol] == 0 && !visited[nextRow][nextCol]) {
                visited[nextRow][nextCol] = true;
                parent[nextRow][nextCol] = {currentRow, currentCol};
                q.push({nextRow, nextCol});

                // Ak sme dosiahli cieľ, ukončíme hľadanie
                if (nextRow == targetRow && nextCol == targetCol) {
                    found = true;
                    break;
                }
            }
        }
    }

    // Ak sa nenašla cesta
    if (!found) {
        return;
    }

    // Rekonštruovanie trasy zo spätných ukazovateľov
    int row = targetRow;
    int col = targetCol;
    while (row != -1 && col != -1) {
        path.emplace_back(row, col);
        auto [prevRow, prevCol] = parent[row][col];
        row = prevRow;
        col = prevCol;
    }
    // Trasu otočíme, pretože sme ju konštruovali od cieľa po štart
    std::reverse(path.begin(), path.end());

    std::cout << std::endl;
}


void MapModule::logToJson(const std::pair<int, int> &position) {
        std::ofstream outFile("../TestModules/logs/map_log.json", std::ios::app);
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open logs/map.json for writing.\n";
            return;
        }
        if (outFile.is_open()) {
            outFile << "{ \"row\": " << position.first << ", \"col\": " << position.second << " }\n";
            outFile.close();
        }
    }

//
//    void MapModule::drawButtons() {
////    // Tlačidlá Start, Stop a Replay
////    if (ImGui::Button("Start")) {
////        isStopped = false;
////    }
////    ImGui::SameLine();
////    if (ImGui::Button("Stop")) {
////        isStopped = true;
////    }
////    ImGui::SameLine();
////    if (ImGui::Button("Replay")) {
////        if (currentStep > 0) {
////            currentStep--; // Vrátenie o jeden krok dozadu
////            isStopped = true;
////        }
////    }
//    }


void MapModule::saveMapToJson() {
    std::ofstream outFile("map.json");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open map.json for writing.\n";
        return;
    }
    if (outFile.is_open()) {
        nlohmann::json j;
        j["rows"] = rows;
        j["cols"] = cols;
        j["map"] = nlohmann::json::array();
        for (int row = 0; row < rows; ++row) {
            j["map"][row] = nlohmann::json::array();
            for (int col = 0; col < cols; ++col) {
                j["map"][row][col] = map[row][col];
            }
        }
        outFile << j.dump(4);
        outFile.close();
    }
    std::ofstream outFile2("../TestModules/logs/map.json");
    if (!outFile2.is_open()) {
        std::cerr << "Error: Could not open map.json for writing.\n";
        return;
    }
    if (outFile2.is_open()) {
        nlohmann::json j;
        j["rows"] = rows;
        j["cols"] = cols;
        j["map"] = nlohmann::json::array();
        for (int row = 0; row < rows; ++row) {
            j["map"][row] = nlohmann::json::array();
            for (int col = 0; col < cols; ++col) {
                j["map"][row][col] = map[row][col];
            }
        }
        outFile2 << j.dump(4);
        outFile2.close();
    }
}


    std::string MapModule::getName() const {
        return "Map Module";
    }


