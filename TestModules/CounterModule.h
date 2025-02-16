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
    void registerShortcuts(ShortcutsManager &shortcutsManager, ToastNotificationManager &toastNotificationManager) override;


    ModuleManager& moduleManager;
    std::vector<int> graphicModuleIds;
    float hodnota1 = 5;
    float hodnota2 = 0;
    int speed = 200;

    std::vector<std::string> getPossibleGraphicsElement() override {
        return {"CounterGraphicElement","TextArea","UltrasonicGraphicElement"};
    }

    std::unordered_map<std::string,std::vector<std::string>> getPossibleInputElements() override {
        return {
            {"button", {"Stop", "Start"}},
            {"horizontal-slider-int", {"Hodnota 1", "Hodnota 2"}},
            {"vertical-slider-int", {"Speed", }},

        };
    }
    void setValueFromInputElements(std::string elementName, std::string value) override ;
    void setValueFromInputElements(std::string elementName, int value) override ;

    void stopCounter();
    void startCounter();

private:
    std::atomic<int> counter;
    std::atomic<bool> stopGeneration;
    std::vector<int> logValues;
    std::mutex logMutex;
    std::thread generatorThread;

    bool checkbox = false;
};

#endif // COUNTERMODULE_H
