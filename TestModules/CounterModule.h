#ifndef COUNTERMODULE_H
#define COUNTERMODULE_H

#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include "imgui.h"
#include "../src/Module.h"

class CounterModule : public Module {
public:
    ImFont* largeFont;
    CounterModule();
    ~CounterModule();

    void renderStandalone(ImGuiIO io, ImVec2 possition) override;
    void draw(ImGuiIO &io) override;
    void saveLogToJson(const std::vector<int> &values);

    std::string getName() const override;
    ImVec2 getSize() override;
    ImVec2 getPos() override;

    void setPos(ImVec2 pos) override;
    void setSize(ImVec2 size) override;

private:
    std::string name = "Counter Module";
    ImVec2 size;
    ImVec2 possition;

    std::atomic<int> counter;
    std::atomic<bool> stopGeneration;
    std::vector<int> logValues;
    std::mutex logMutex;
    std::thread generatorThread;

    void generateNumbers();
};

#endif // COUNTERMODULE_H
