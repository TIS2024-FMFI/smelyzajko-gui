#include "MapModule.h"
#include <vector>
#include <fstream>
#include <random>
#include <sstream>
#include <iostream>
#include <cmath> // Pre výpočet vzdialenosti
#include <queue>
#include <vector>
#include <iostream>

MapModule::MapModule(ModuleManager* moduleManager) : moduleManager(*moduleManager), running(false), deltaTime(0.0f){ // Initialize rows and cols
    setModuleName("MapModule") ;
    graphicModuleId.push_back(this->moduleManager.registerGraphicModule("MapGraphicElement",moduleName, moduleId));
    graphicModuleId.push_back(this->moduleManager.registerGraphicModule("TextArea",moduleName, moduleId));
    graphicModuleId.push_back(this->moduleManager.registerGraphicModule("CounterGraphicElement",moduleName, moduleId));

    if (!mapInitialized) {
        generatePassableMap();

        mapInitialized = true;
    }
    if (path.empty()) {
        generatePath();
    }

    running = true;
    mapThread = std::thread(&MapModule::run, this);
    std::vector<int> rowCol = {rows, cols,-1};
    moduleManager->updateValueOfModule(moduleId, graphicModuleId[0], rowCol);
    moduleManager->updateValueOfModule(moduleId, graphicModuleId[0], map);

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
            timeAccumulator += 0.009f * speed; // Delta time adjusted by speed

            if (timeAccumulator > 0.5f) {
                timeAccumulator = 0.0f;
                if (currentStep < path.size() - 1) {
                    currentStep++;
                    int ballRow = path[currentStep].first;
                    int ballCol = path[currentStep].second;
                    moduleManager.updateValueOfModule(moduleId, graphicModuleId[0], std::vector<int>{ballRow, ballCol});
                    std::string text = "Step: " + std::to_string(currentStep) + " / " + std::to_string(path.size() - 1);
                    moduleManager.updateValueOfModule(moduleId, graphicModuleId[1],text);
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
}

void MapModule::registerShortcuts(ShortcutsManager& shortcutsManager, ToastNotificationManager& toastManager) {
    shortcutsManager.registerShortcut("Ctrl+R", [this, &toastManager]() {
        resetMap();
        toastManager.addNotification(moduleName, "Map has been reset.");
    });

    shortcutsManager.registerShortcut("Ctrl+M", [this, &toastManager]() {
        isStopped = true;
        toastManager.addNotification(moduleName, "Map walk has been stopped.");
    });

    shortcutsManager.registerShortcut("Ctrl+N", [this, &toastManager]() {
        isStopped = false;
        toastManager.addNotification(moduleName, "Map walk has been started.");
    });
}




void MapModule::setValueFromInputElements(std::string elementName, std::string value) {
    if (elementName == "Reset") {
        resetMap();
    }
}

void MapModule::setValueFromInputElements(std::string elementName, bool value) {
    if (elementName == "Running") {
        isStopped = !value;
    }
}

void MapModule::setValueFromInputElements(std::string elementName, float value) {
    if (elementName == "Speed") {
        speed = value;
    }
}

void MapModule::resetMap() {
    currentStep = 0;
    if (!path.empty()) {
        int ballRow = path[currentStep].first;
        int ballCol = path[currentStep].second;
        moduleManager.updateValueOfModule(moduleId, graphicModuleId[0], std::vector<int>{ballRow, ballCol});
    }
}




