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
    void updateValueOfModule( int value) override;

private:
    int counter;
    std::atomic<bool> stopGeneration;
    std::vector<std::string> logValues;
    std::mutex logMutex;
    std::thread generatorThread;
    bool autoscrollEnabled;
    float scrollOffset;


};


#endif //SMELYZAJKO_GUI_COUNTERMODULEGRAPHICS_H
