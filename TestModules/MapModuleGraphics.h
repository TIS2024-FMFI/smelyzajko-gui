#ifndef SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H

#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"
#include <vector>
#include <string>
#include <mutex>

class MapModuleGraphics : public GraphicModule {
public:
    MapModuleGraphics();
    void draw(ImGuiIO &io) override;                        // Render the module
    void to_json(nlohmann::json& j) const override;         // Serialize module properties to JSON
    void from_json(const nlohmann::json& j) override;       // Deserialize module properties from JSON

    void updateValueOfModule(std::string value) override;   // Handle string updates (not used here)
    void updateValueOfModule(int value) override;           // Handle integer updates (not used here)
    void updateValueOfModule(std::vector<int> value) override; // Update ball position

    ImVec2 getPos() override { return position; }           // Get the module position
    ImVec2 getSize() override { return size; }              // Get the module size
    std::string getName() const override { return moduleName; } // Get the module name
    int getGraphicModuleID() override { return graphicModuleId; } // Get the graphics module ID
    void setPos(ImVec2 pos_) override { position = pos_; }  // Set the module position
    void setSize(ImVec2 size_) override {                   // Set the module size and adjust cell size
        size = size_;
        cellSize = std::min(size.x / cols, size.y / rows);
    }
    void setGraphicModuleID(int id) override { graphicModuleId = id; } // Set the graphics module ID

private:
    // Map properties
    int ballRow = 0;                     // Current row position of the ball
    int ballCol = 0;                     // Current column position of the ball
    int rows = 10;                       // Number of rows in the map
    int cols = 10;                       // Number of columns in the map
    std::vector<std::vector<int>> map;   // Map data
    float cellSize = 40.0f;              // Size of each cell in the map

    // Graphics settings
    float graphicsFrequency;             // Frequency for graphics refresh
    bool graphicsLogEnabled;             // Enable or disable graphics logging

    // Text settings
    float textFrequency;                 // Frequency for text refresh
    bool textLogEnabled;                 // Enable or disable text logging

    // Log scrolling
    std::vector<std::string> logValues;  // Logs for the text area
    float scrollOffset;                  // Current scroll offset
    bool autoscrollEnabled;              // Enable or disable autoscroll
    std::mutex logMutex;                 // Mutex for protecting log access during updates

    // Private methods
    void loadMap();                      // Load map data from a file
    void initializeBallPosition(const std::pair<int, int>& start); // Initialize ball position
};

#endif // SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
