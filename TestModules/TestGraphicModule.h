// File: TestModules/TestGraphicModule.h
#ifndef TESTGRAPHICMODULE_H
#define TESTGRAPHICMODULE_H
#include "imgui.h"
#include <string>
#include "../src/Module.h"
#include "../src/GraphicModule.h"
class TestGraphicModule : public Module {
public:
    void renderStandalone(ImGuiIO io, ImVec2 possition) ;

    void run() override;
    ImVec2 getSize() ;

    ImVec2 getPos() ;

    void setPos(ImVec2 pos) ;

    void setSize(ImVec2 size) ;

    std::string getName() const override;
    void draw(ImGuiIO &io) ;
    bool isModuleActive = false;

    int moduleId;            // Module ID for identification
    std::string moduleName = "Sinusoida";  // Module name
    void to_json(nlohmann::json& j) const ;
    void from_json(const nlohmann::json& j) ;

private:
    ImVec2 size ;
    ImVec2 position;

    // Graphics settings
    float graphicsFrequency;
    bool graphicsLogEnabled;

    // Text settings
    float textFrequency;
    bool textLogEnabled;

};

#endif // TESTGRAPHICMODULE_H