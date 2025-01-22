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
    void logFromJson() override;
    void logForward() override;
    void logBackwards(); // Navigate logs backward
    void updateValueOfModule(int value) override;

private:
    int counter;                              // Counter value
    std::vector<int> logData;                 // Loaded log data
    size_t currentLogIndex = 0;               // Current log index
    std::atomic<bool> stopGeneration;         // Flag for stopping threads
    std::mutex logMutex;                      // Mutex for log access
    std::thread generatorThread;              // Thread for asynchronous tasks
};

#endif // SMELYZAJKO_GUI_COUNTERMODULEGRAPHICS_H
