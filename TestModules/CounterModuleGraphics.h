#ifndef SMELYZAJKO_GUI_COUNTERMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_COUNTERMODULEGRAPHICS_H

#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"
#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>

class CounterModuleGraphics : public GraphicModule {
public:
    CounterModuleGraphics();
    void draw(ImGuiIO &io) override;                        // Draw the module graphics
    void to_json(nlohmann::json& j) const override;         // Serialize to JSON
    void from_json(const nlohmann::json& j) override;       // Deserialize from JSON

    void updateValueOfModule(std::string value) override;   // Update with a string value
    void updateValueOfModule(int value) override;           // Update with an integer value
    void updateValueOfModule(std::vector<int> value) override; // Update with a vector value

    // Module position and size accessors
    ImVec2 getPos() override { return position; }
    ImVec2 getSize() override { return size; }
    std::string getName() const override { return moduleName; }
    int getGraphicModuleID() override { return graphicModuleId; }
    void setPos(ImVec2 pos_) override { position = pos_; }
    void setSize(ImVec2 size_) override { size = size_; }
    void setGraphicModuleID(int id) override { graphicModuleId = id; }

private:
    // Module information
    std::string moduleName = "Counter"; // Module name
    int graphicModuleId;                // Graphics module ID
    int moduleId;                       // Module ID

    // Module properties
    ImVec2 size;                        // Module size
    ImVec2 position;                    // Module position

    // Counter and logs
    int counter;                        // Counter value
    std::vector<std::string> logValues; // Log entries for the text area
    std::mutex logMutex;                // Mutex for thread-safe log access

    // Graphics settings
    float graphicsFrequency;            // Graphics refresh frequency
    bool graphicsLogEnabled;            // Whether graphics logging is enabled

    // Text settings
    float textFrequency;                // Text refresh frequency
    bool textLogEnabled;                // Whether text logging is enabled

    // Scroll and autoscroll
    float scrollOffset;                 // Scroll position for the text area
    bool autoscrollEnabled;             // Toggle for autoscroll functionality
};

#endif // SMELYZAJKO_GUI_COUNTERMODULEGRAPHICS_H
