#ifndef SMELYZAJKO_GUI_GRAPHICMODULE_H
#define SMELYZAJKO_GUI_GRAPHICMODULE_H
#include "iostream"
#include "../libs/imgui/imgui.h"
#include "../libs/json.hpp"


class GraphicModule {
public:
    virtual void draw(ImGuiIO &io) = 0;

    // getters and setters
    virtual int getGraphicModuleID() = 0;
    virtual ImVec2 getSize() = 0;
    virtual ImVec2 getPos() = 0;
    virtual std::string getName() const = 0;
    virtual void setPos(ImVec2 pos_) =0;
    virtual void setSize(ImVec2 size_) =0 ;
    void setModuleID(int id) { moduleId = id; }
    virtual void setGraphicModuleID(int id) = 0;

    virtual void updateValueOfModule(std::string value) = 0;
    virtual void updateValueOfModule(int value) = 0;
    virtual void updateValueOfModule(std::vector<int> value) = 0;

    virtual void to_json(nlohmann::json& j) const = 0;
    virtual void from_json(const nlohmann::json& j, ImVec2 resolution) = 0;

    virtual void scaleFromResolution(ImVec2 templateResolution);

    // Graphics settings
    float graphicsFrequency;
    bool graphicsLogEnabled;

    // Text settings
    float textFrequency;
    bool textLogEnabled;

protected:
    int graphicModuleId;
    int moduleId;
    std::string moduleName;
    ImVec2 position;
    ImVec2 size;
};


#endif //SMELYZAJKO_GUI_GRAPHICMODULE_H
