#include "CounterModule.h"
#include "imgui.h"
#include <iostream>
#include <fstream> // Add this line to include the fstream header


void CounterModule::renderStandalone(ImGuiIO io, ImVec2 possition) {
    if (counter == 1){
        ImGui::SetNextWindowPos(getPos()); // Set the position of the window
        ImGui::SetNextWindowSize(getSize())  ; // Set the size of the window
        ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar |    // Remove title bar
                                            ImGuiWindowFlags_NoCollapse |   // Prevent collapsing
                                            ImGuiWindowFlags_NoResize |     // Disable resizing
                                            ImGuiWindowFlags_NoBringToFrontOnFocus | // Prevent window focus changes
                                            ImGuiWindowFlags_NoScrollbar |   // Disable scrollbar (optional)
                                            ImGuiWindowFlags_NoBackground); // Disable resizing

    }
    else {
        ImGui::Begin(name.c_str());
        setPos(ImGui::GetWindowPos());
        setSize(ImGui::GetWindowSize());
    }

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
    // Print the size of the window
//    ImVec2 windowSize = ImGui::GetWindowSize();
//    std::cout << "Window size: (" << windowSize.x << ", " << windowSize.y << ")" << std::endl;

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
std::string CounterModule::getName() const {
    return name;
}

ImVec2 CounterModule::getSize() {
    return size;
}

ImVec2 CounterModule::getPos() {
    return possition;
}

void CounterModule::setPos(ImVec2 pos) {
    CounterModule::possition = pos;
}

void CounterModule::setSize(ImVec2 size) {
    CounterModule::size = size;
}

void CounterModule::draw(ImGuiIO &io) {

}
