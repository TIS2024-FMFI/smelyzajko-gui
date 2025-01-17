#include "CounterModule.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

CounterModule::CounterModule(ModuleManager* moduleManager)
        : moduleManager(*moduleManager), counter(0), stopGeneration(false) {
    setModuleName("Counter Module");
    moduleId = this->moduleManager.registerModule(moduleName, this);
    std::cout<<moduleId<<std::endl;
    graphicModuleId = this->moduleManager.registerGraphicModule("Counter Graphic Element",moduleName, moduleId);
    generatorThread = std::thread(&CounterModule::run, this);
}

CounterModule::~CounterModule() {
    stopGeneration.store(true);
    if (generatorThread.joinable()) {
        generatorThread.join();
    }
}

void CounterModule::run() {
    while (!stopGeneration.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        counter.fetch_add(1);
        saveLogToJson({counter.load()});
        moduleManager.updateValueOfModule(moduleId,graphicModuleId, counter.load());
    }
}




void CounterModule::saveLogToJson(const std::vector<int>& values) {
    static bool initialized = false; // Indikátor, či sme už inicializovali súbor

    std::lock_guard<std::mutex> lock(logMutex); // Chráni zápis

    std::string filename = "../TestModules/logs/counter_log.json";
    nlohmann::json j;

    // Inicializácia súboru pri prvom volaní
    if (!initialized) {
        j["counter_values"] = nlohmann::json::array(); // Prázdny zoznam
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << std::setw(4) << j << std::endl; // Zápis prázdneho zoznamu
            outFile.close();
        } else {
            std::cerr << "[ERROR] Could not open file for initialization: " << filename << std::endl;
            return;
        }
        initialized = true; // Nastaviť ako inicializované
    }

    // Pridať nové hodnoty do súboru
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> j; // Načítať existujúci obsah
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << std::endl;
        }
        inFile.close();
    }

    // Pridať nové hodnoty
    for (const int& value : values) {
        j["counter_values"].push_back(value);
    }

    // Prepísať súbor s aktualizovaným obsahom
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << std::setw(4) << j << std::endl; // Zápis s indentáciou
        outFile.close();
    } else {
        std::cerr << "[ERROR] Could not open file for writing: " << filename << std::endl;
    }
}

void CounterModule::setValueFromInputElements(std::string elementName, std::string value) {
    if (elementName == "Button1") {
        counter.store(0);
    }
    else if (elementName == "Text1") {
        if (value == "stop") {
            if (!stopGeneration.load()) {
                stopGeneration.store(true);
                if (generatorThread.joinable()) {
                    generatorThread.join();
                }
            }
        }
    }
}
void CounterModule::setValueFromInputElements(std::string elementName, int value) {
    //std::cout<<elementName<<std::endl;
    if (elementName == "Slider-int1" && checkbox) {
        counter.store(value);
    }
    //counter.store(value);
}

void CounterModule::setValueFromInputElements(std::string elementName, bool value) {

    if (elementName == "Checkbox1") {
        checkbox = value;
    }
}


