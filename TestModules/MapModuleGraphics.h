#ifndef SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H

#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"
#include <vector>
#include <string>
#include <mutex>
#include "Scrollbar.h"

class MapModuleGraphics : public GraphicModule {
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

    // Log scrolling
    std::vector<std::string> logValues;  // Logs for the text area
    float scrollOffset;                  // Current scroll offset
    bool autoscrollEnabled;              // Enable or disable autoscroll
    std::mutex logMutex;                 // Mutex for protecting log access during updates
    Scrollbar scrollbar;

    // Private methods
    void loadMap();                      // Load map data from a file
    void initializeBallPosition(const std::pair<int, int>& start); // Initialize ball position
};

#endif // SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
