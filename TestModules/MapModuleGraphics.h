#ifndef SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H

#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"
#include "src/TextArea.h" // Include TextArea class
#include <vector>
#include <string>
#include <mutex>

class MapModuleGraphics : public GraphicModule {
public:
    MapModuleGraphics();
    void draw(ImGuiIO &io) override;
    void logToJson() override;
    void updateValueOfModule(std::vector<int> value) override;
    void updateValueOfModule(std::vector<std::vector<int>> value) override;
    void saveMapToJson();

private:
    int ballRow = 0;
    int ballCol = 0;
    int rows = -1;
    int cols = -1;
    std::vector<std::vector<int>> map;
    float cellSize = 40.0f;
    std::mutex logMutex;



    // Private methods
    void loadMap(); // Load map data from a file
};

#endif // SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
