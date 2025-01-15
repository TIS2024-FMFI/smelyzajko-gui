#ifndef COUNTERMODULE_H
#define COUNTERMODULE_H

#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include "imgui.h"
#include "../src/Module.h"
#include "../src/ModuleManager.h"


class CounterModule : public Module {
public:
    ImFont* largeFont;
    CounterModule(ModuleManager* moduleManager);
    ~CounterModule() ;
    void run() override;
    std::string getName() const override;
    int getModuleID() const override ;

    void saveLogToJson(const std::vector<int> &values);
    ModuleManager& moduleManager;
    int graphicModuleId;
    int moduleId;
    std::vector<std::string> getPossibleGraphicsElement() override ;



private:
    std::string moduleName = "Counter Module";
    ImVec2 size;
    ImVec2 position;

    std::atomic<int> counter;
    std::atomic<bool> stopGeneration;
    std::vector<int> logValues;
    std::mutex logMutex;
    std::thread generatorThread;

};

#endif // COUNTERMODULE_H
