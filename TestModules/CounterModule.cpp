#include "CounterModule.h"
#include "imgui.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream> // Na generovanie JSON formátu

void CounterModule::renderStandalone() {
    ImGui::Begin("Counter Modul");

    // Premenné
    static int counter = 0;
    static float timeAccumulator = 0.0f;
    static bool isStopped = true; // Počiatočný stav je zastavený
    static std::vector<int> logValues = {0}; // Na logovanie hodnôt

    // Aktualizácia času iba ak nie je zastavené
    if (!isStopped) {
        timeAccumulator += ImGui::GetIO().DeltaTime;

        if (timeAccumulator >= 0.3f) {
            timeAccumulator = 0.0f;
            counter++;
            logValues.push_back(counter); // Uloženie hodnoty do logu
            saveLogToJson(logValues); // Automaticky uložiť do JSON
        }
    }

    // Zobrazenie aktuálneho počítadla
    ImGui::Text("Aktuálna hodnota counteru: %d", counter);

    // Tlačidlo Start
    if (ImGui::Button("Start")) {
        isStopped = false;
    }
    ImGui::SameLine(); // Zarovná Stop tlačidlo na tú istú riadkovú pozíciu

    // Tlačidlo Stop
    if (ImGui::Button("Stop")) {
        isStopped = true;
    }
    ImGui::SameLine(); // Zarovná Replay tlačidlo na tú istú riadkovú pozíciu

    // Tlačidlo Replay
    if (ImGui::Button("Replay")) {
        if (!logValues.empty() && counter > 0) {
            counter = logValues.back(); // Vrátenie poslednej hodnoty
            logValues.pop_back(); // Odstránenie poslednej hodnoty
            saveLogToJson(logValues); // Aktualizácia logu v JSON
        }
    }

    ImGui::End();
}

void CounterModule::saveLogToJson(const std::vector<int>& values) {
    // Otvorenie súboru na zápis
    std::ofstream outFile("counter_log.json");
    if (outFile.is_open()) {
        outFile << "{\n  \"counter_values\": [";

        for (size_t i = 0; i < values.size(); ++i) {
            outFile << values[i];
            if (i != values.size() - 1) {
                outFile << ", ";
            }
        }

        outFile << "]\n}";
        outFile.close();
    }
}
