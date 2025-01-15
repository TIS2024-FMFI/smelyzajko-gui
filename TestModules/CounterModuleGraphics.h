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
    void draw(ImGuiIO &io) override;
    void to_json(nlohmann::json& j) const override;
    void from_json(const nlohmann::json& j, ImVec2 resolution) override;

    void updateValueOfModule( std::string value) override;
    void updateValueOfModule( int value) override;
    void updateValueOfModule( std::vector<int> value) override;
    ImVec2 getPos() override { return position; }
    ImVec2 getSize() override { return size; }
    std::string getName() const override { return moduleName; }
    int getGraphicModuleID() override { return graphicModuleId; }
    void setPos(ImVec2 pos_) override { position = pos_; }
    void setSize(ImVec2 size_) override { size = size_; }
    void setGraphicModuleID(int id) override { graphicModuleId = id; }
//    void setModuleID(int id) { moduleId = id; }


    ImVec2 size;
    ImVec2 position;
    std::string moduleName = "Counter Graphic Element " ;  // Module name
    int graphicModuleId;
    int moduleId;

private:
    int counter;
    std::atomic<bool> stopGeneration;
    std::vector<int> logValues;
    std::mutex logMutex;
    std::thread generatorThread;

};


#endif //SMELYZAJKO_GUI_COUNTERMODULEGRAPHICS_H
