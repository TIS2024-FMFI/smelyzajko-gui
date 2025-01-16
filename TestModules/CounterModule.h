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


    void saveLogToJson(const std::vector<int> &values);
    ModuleManager& moduleManager;
    int graphicModuleId;
    std::vector<std::string> getPossibleGraphicsElement() override ;



private:
    std::atomic<int> counter;
    std::atomic<bool> stopGeneration;
    std::vector<int> logValues;
    std::mutex logMutex;
    std::thread generatorThread;

};

#endif // COUNTERMODULE_H
