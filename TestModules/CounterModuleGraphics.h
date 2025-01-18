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
    void logToJson() override;
    void updateValueOfModule(int value) override;

private:
    int counter;
    std::atomic<bool> stopGeneration;
    std::mutex logMutex;
    std::thread generatorThread;

};

#endif //SMELYZAJKO_GUI_COUNTERMODULEGRAPHICS_H
