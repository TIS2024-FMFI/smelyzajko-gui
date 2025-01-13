#ifndef SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
#define SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
#include "imgui.h"
#include "../src/Module.h"
#include "../src/GraphicModule.h"

class MapModuleGraphics : public GraphicModule  {

public:
    MapModuleGraphics();
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
    void setSize(ImVec2 size_) override { size = size_; cellSize=  std::min(size.x/ cols, size.y / rows);; }
    void setGraphicModuleID(int id) override { graphicModuleId = id; }
//    void setModuleID(int id) { moduleId = id; }


    ImVec2 size;
    ImVec2 position;
    std::string moduleName = "Counter Module" ;  // Module name
    int graphicModuleId;
    int moduleId;

private:
    int ballRow = 0;
    int ballCol = 0;
    int rows = 10;
    int cols =10;
    std::vector<std::vector<int>> map;
    float cellSize = 40.0f;
    // Graphics settings
    float graphicsFrequency;
    bool graphicsLogEnabled;

    // Text settings
    float textFrequency;
    bool textLogEnabled;


    void loadMap();


};


#endif //SMELYZAJKO_GUI_MAPMODULEGRAPHICS_H
