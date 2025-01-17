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
    std::vector<std::string> getPossibleGraphicsElement() override{
        return {"Counter Graphic Element"};
    }
    std::unordered_map<std::string,std::vector<std::string>> getPossibleInputElements() override {
        return {
            {"button", {"Button1", "Button2"}},
            {"slider-int", {"Slider-int1", "Slider-int2"}}
        };
    }
    void setValueFromInputElements(std::string elementName, std::string value) override ;
    void setValueFromInputElements(std::string elementName, int value) override ;




private:
    std::atomic<int> counter;
    std::atomic<bool> stopGeneration;
    std::vector<int> logValues;
    std::mutex logMutex;
    std::thread generatorThread;

};

#endif // COUNTERMODULE_H
