#include "MapModule.h"
#include "imgui.h"
#include <vector>
#include <fstream>
#include <random>
#include <sstream>
#include <cmath> // Pre výpočet vzdialenosti

void MapModule::renderStandalone() {
    ImGui::Begin("Mapa");

    // Rozmery mapy a buniek
    const int rows = 10;
    const int cols = 10;
    const float cellSize = 40.0f;

    // Náhodná mapa (0 = cesta, 1 = múr)
    static int map[rows][cols];
    static bool mapInitialized = false;
    static std::pair<int, int> startPosition; // Náhodná počiatočná pozícia

    if (!mapInitialized) {
        generatePassableMap(map, rows, cols, startPosition);
        mapInitialized = true;
    }

    // Trasa pre guličku (sekvencia [riadok, stĺpec])
    static std::vector<std::pair<int, int>> path;
    static bool isStopped = true;
    static int currentStep = 0;

    // Ak sa mapa zmení, aktualizuj cestu
    if (path.empty()) {
        generatePath(map, rows, cols, path, startPosition);
    }

    // Tlačidlá Start, Stop a Replay
    if (ImGui::Button("Start")) {
        isStopped = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        isStopped = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Replay")) {
        if (currentStep > 0) {
            currentStep--; // Vrátenie o jeden krok dozadu
            isStopped = true;
        }
    }

    // Pohyb guličky každých 0.5 sekundy
    if (!path.empty() && !isStopped) {
        static float timeAccumulator = 0.0f;
        timeAccumulator += ImGui::GetIO().DeltaTime;

        if (timeAccumulator > 0.5f) {
            timeAccumulator = 0.0f;
            if (currentStep < path.size() - 1) {
                currentStep++;
                logToJson(path[currentStep]); // Logovanie aktuálnej pozície do JSON
            }
        }
    }

    // Základné vykreslenie mapy
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();

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
                    ImVec2(p.x + col * cellSize, p.y + row * cellSize),
                    ImVec2(p.x + (col + 1) * cellSize, p.y + (row + 1) * cellSize),
                    color
            );
        }
    }

    // Vykreslenie guličky
    if (!path.empty()) {
        int ballRow = path[currentStep].first;
        int ballCol = path[currentStep].second;
        draw_list->AddCircleFilled(
                ImVec2(p.x + ballCol * cellSize + cellSize / 2, p.y + ballRow * cellSize + cellSize / 2),
                cellSize / 4, IM_COL32(255, 0, 0, 255)
        );
    }

    ImGui::End();
}

void MapModule::generatePassableMap(int map[10][10], int rows, int cols, std::pair<int, int>& startPosition) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

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
                    int distance = std::abs(row - (rows - 2)) + std::abs(col - (cols - 2)); // Manhattanova vzdialenosť
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
            std::vector<std::pair<int, int>> testPath;
            generatePath(map, rows, cols, testPath, startPosition);
            if (!testPath.empty()) {
                break;
            }
        }
    }
}

void MapModule::generatePath(const int map[10][10], int rows, int cols, std::vector<std::pair<int, int>>& path, const std::pair<int, int>& startPosition) {
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

void MapModule::logToJson(const std::pair<int, int>& position) {
    std::ofstream outFile("map_log.json", std::ios::app);
    if (outFile.is_open()) {
        outFile << "{ \"row\": " << position.first << ", \"col\": " << position.second << " }\n";
        outFile.close();
    }
}
