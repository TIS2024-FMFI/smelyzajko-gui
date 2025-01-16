// File: TestModules/TestGraphicModule.h
#ifndef TESTGRAPHICMODULE_H
#define TESTGRAPHICMODULE_H
#include "imgui.h"
#include <string>
#include "../src/Module.h"
#include "../src/GraphicModule.h"
class TestGraphicModule : public Module {
public:

    void run() override;
    bool isModuleActive = false;


};

#endif // TESTGRAPHICMODULE_H