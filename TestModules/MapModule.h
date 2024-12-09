#ifndef MAPMODULE_H
#define MAPMODULE_H

#include <vector>
#include <utility>
#include <string>
#include "imgui.h"
#include "../src/Module.h"


class MapModule : public Module {
public:
    MapModule();
    // Funkcia na vykreslenie mapy
    void renderStandalone(ImGuiIO io, ImVec2 possition) override;
    void draw(ImGuiIO &io) override;
    // Funkcia na logovanie pozície guličky do JSON súboru
    void logToJson(const std::pair<int, int>& position);

    // Funkcia na generovanie priechodnej mapy, nájde aj počiatočnú pozíciu čo najďalej od cieľa
    void generatePassableMap();

    // Funkcia na generovanie cesty cez mapu z náhodnej štartovacej pozície
    void generatePath();

    std::string getName() const override;

    ImVec2 getSize() override;

    ImVec2 getPos() override;

    void setPos(ImVec2 pos) override;

    void setSize(ImVec2 size) override;

    void drawButtons();
    static const int rows = 10;
    static const int cols = 10;
    int map[rows][cols];
    const float cellSize = 40.0f;

    // Náhodná mapa (0 = cesta, 1 = múr)

    bool mapInitialized = false;
    std::pair<int, int> startPosition; // Náhodná počiatočná pozícia

    // Trasa pre guličku (sekvencia [riadok, stĺpec])
    std::vector<std::pair<int, int>> path;

    bool isStopped = false;
    int currentStep = 0;

private:
    std::string name = "Map Module";
    ImVec2 size;
    ImVec2 possition ;
    // Rozmery mapy a buniek




};


#endif // MAPMODULE_H
