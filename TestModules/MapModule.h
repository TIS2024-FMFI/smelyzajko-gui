#ifndef MAPMODULE_H
#define MAPMODULE_H

#include <vector>
#include <utility>

class MapModule {
public:
    // Funkcia na vykreslenie mapy
    void renderStandalone();

    // Funkcia na logovanie pozície guličky do JSON súboru
    void logToJson(const std::pair<int, int>& position);

    // Funkcia na generovanie priechodnej mapy, nájde aj počiatočnú pozíciu čo najďalej od cieľa
    void generatePassableMap(int map[10][10], int rows, int cols, std::pair<int, int>& startPosition);

    // Funkcia na generovanie cesty cez mapu z náhodnej štartovacej pozície
    void generatePath(const int map[10][10], int rows, int cols, std::vector<std::pair<int, int>>& path, const std::pair<int, int>& startPosition);
};

#endif // MAPMODULE_H
