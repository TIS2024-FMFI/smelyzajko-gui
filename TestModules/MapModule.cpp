#include "MapModule.h"
#include <vector>
#include <fstream>
#include <random>
#include <sstream>
#include <iostream>
#include <cmath> // Pre výpočet vzdialenosti

MapModule::MapModule(ModuleManager* moduleManager) : moduleManager(*moduleManager), running(false), deltaTime(0.0f){ // Initialize rows and cols
    moduleId = this->moduleManager.registerModule("Map Module",  this);
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

    void MapModule::generatePath() {
        path.clear();
        int row = startPosition.first;
        int col = startPosition.second;

        while (row < rows - 1 && col < cols - 1) {
            path.emplace_back(row, col);

            if (map[row + 1][col] == 0) {
                row++;
            } else if (map[row][col + 1] == 0) {
                col++;
            } else {
                break; // Ak sa nedá pohnúť, ukončí trasu
            }
        }
        if (row == rows - 2 && col == cols - 2) {
            path.emplace_back(row, col); // Pridaj cieľ
        } else {
            path.clear(); // Vyprázdni trasu, ak nedosiahne cieľ
        }
    }

    void MapModule::logToJson(const std::pair<int, int> &position) {
        std::ofstream outFile("map_log.json", std::ios::app);
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
}


    std::string MapModule::getName() const {
        return "Map Module";
    }


