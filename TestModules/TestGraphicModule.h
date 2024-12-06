// File: TestModules/TestGraphicModule.h
#ifndef TESTGRAPHICMODULE_H
#define TESTGRAPHICMODULE_H
#include "imgui.h"
#include <string>
#include "../src/Module.h"
class TestGraphicModule : public Module {
public:
    void renderStandalone(ImGuiIO io, ImVec2 possition) override;

    ImVec2 getSize() override;

    ImVec2 getPos() override;

    void setPos(ImVec2 pos) override;

    void setSize(ImVec2 size) override;

    std::string getName() const override;
    void draw(ImGuiIO &io) override;

private:
    std::string name = "Sinusoida";
    ImVec2 size ;
    ImVec2 possition;

};

#endif // TESTGRAPHICMODULE_H