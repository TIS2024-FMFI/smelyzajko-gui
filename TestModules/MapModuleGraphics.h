#ifndef SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"

class MapModuleGraphics : public GraphicModule  {

public:
    MapModuleGraphics();
    void draw(ImGuiIO &io) override;
    void updateValueOfModule( std::vector<int> value) override;

private:
    int ballRow = 0;
    int ballCol = 0;
    int rows = 10;
    int cols =10;
    std::vector<std::vector<int>> map;
    float cellSize = 40.0f;

    void loadMap();

};


#endif //SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
