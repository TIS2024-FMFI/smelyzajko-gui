#include "MapModule.h"
#include "imgui.h"
#include <vector>

void MapModule::renderStandalone() {
    ImGui::Begin("Mapa");

    // Rozmery mapy a buniek
    const int rows = 10;
    const int cols = 10;
    const float cellSize = 40.0f;

    // Definícia mapy: 0 = cesta, 1 = múr
    const int map[rows][cols] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
            {1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
            {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
            {1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
            {1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
            {1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    // Cesta pre guličku (sekvencia [riadok, stĺpec])
    static std::vector<std::pair<int, int>> path = {
            {1, 1}, {1, 2}, {1, 3}, {2, 3}, {3, 3}, {3, 4}, {3, 5}, {3, 6},
            {4, 6}, {5, 6}, {6, 6}, {7, 6}, {7, 5}, {7, 4}, {7, 3}, {7, 2}, {7, 1}
    };
    static int currentStep = 0;

    // Aktualizácia pozície guličky
    if (!path.empty()) {
        static float timeAccumulator = 0.0f;
        timeAccumulator += ImGui::GetIO().DeltaTime;

        // Pohyb guličky každých 0.5 sekundy
        if (timeAccumulator > 0.5f) {
            timeAccumulator = 0.0f;
            currentStep = (currentStep + 1) % path.size();  // Loop cez trasu
        }
    }

    // Základné vykreslenie mapy
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            ImU32 color = (map[row][col] == 1) ? IM_COL32(50, 50, 50, 255) : IM_COL32(200, 200, 200, 255);
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
