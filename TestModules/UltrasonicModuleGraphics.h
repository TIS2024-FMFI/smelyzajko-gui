#ifndef SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H
#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"


struct UltrasonicSensorData {
    float angle;
    float distance;
};

class UltrasonicModuleGraphics: public GraphicModule {
public:

    //same in all modules
    void draw(ImGuiIO &io) override;
    void to_json(nlohmann::json& j) const override;
    void from_json(const nlohmann::json& j) override;
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
    ImVec2 size;
    ImVec2 position;
    std::string moduleName = "Ultrasonic Module" ;  // Module name
    int graphicModuleId;
    int moduleId;


    void updateRandomSensorData();
    std::vector<UltrasonicSensorData> sensors;

private:
    int frameCounter = 0;
    int updateIntervalFrames = 180;

};


#endif //SMELYZAJKO_GUI_ULTRASONICMODULEGRAPHICS_H