#ifndef MAPMODULE_H
#define MAPMODULE_H

#include <vector>
#include <utility>
#include <string>
#include "imgui.h"
#include "../src/Module.h"
#include <thread>
#include <atomic>
#include <chrono>
#include "../src/ModuleManager.h"


class MapModule : public Module {


public:
    MapModule(ModuleManager* moduleManager);
//    MapModule(int id, const char* name) : Module(id, name) {}
    ~MapModule() ;
    void run() override;
    std::string getName() const override;
    void logToJson(const std::pair<int, int>& position);
    // Funkcia na generovanie priechodnej mapy, nájde aj počiatočnú pozíciu čo najďalej od cieľa
    void generatePassableMap();
    void saveMapToJson();
    // Funkcia na generovanie cesty cez mapu z náhodnej štartovacej pozície
    void generatePath();

    ModuleManager& moduleManager;

//    void drawButtons();

    const float cellSize = 40.0f;

    // Náhodná mapa (0 = cesta, 1 = múr)

    bool mapInitialized = false;
    std::pair<int, int> startPosition; // Náhodná počiatočná pozícia
    int rows = 20;
    int cols = 20;
    // Trasa pre guličku (sekvencia [riadok, stĺpec])
    std::vector<std::pair<int, int>> path;
    std::vector<std::vector<int>> map;



    std::string moduleName = "Map Module" ;  // Module name
    int graphicModuleId;


private:
    ImVec2 size;
    ImVec2 position ;


    // Graphics settings
    float graphicsFrequency{};
    bool graphicsLogEnabled{};

    // Text settings
    float textFrequency{};
    bool textLogEnabled{};


    std::thread mapThread;
    std::atomic<bool> running;
    float deltaTime;
    bool isStopped = false;
    int currentStep = 0;


};


#endif // MAPMODULE_H
