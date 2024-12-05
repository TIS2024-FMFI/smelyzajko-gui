#ifndef MAPMODULE_H
#define MAPMODULE_H

#include <vector>
#include <utility>
#include <string>
#include "imgui.h"
#include "../src/Module.h"


class MapModule : public Module {
public:
    // Funkcia na vykreslenie mapy
    void renderStandalone() override;

    // Funkcia na logovanie pozície guličky do JSON súboru
    void logToJson(const std::pair<int, int>& position);

    // Funkcia na generovanie priechodnej mapy, nájde aj počiatočnú pozíciu čo najďalej od cieľa
    void generatePassableMap(int map[10][10], int rows, int cols, std::pair<int, int>& startPosition);

    // Funkcia na generovanie cesty cez mapu z náhodnej štartovacej pozície
    void generatePath(const int map[10][10], int rows, int cols, std::vector<std::pair<int, int>>& path, const std::pair<int, int>& startPosition);

    std::string getName() const override;

    ImVec2 getSize() override;

    ImVec2 getPos() override;

    void setPos(ImVec2 pos) override;

    void setSize(ImVec2 size) override;


private:
    std::string name = "Map Module";
    ImVec2 size;
    ImVec2 pos;
};


#endif // MAPMODULE_H
