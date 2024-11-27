#include "CounterModule.h"
#include "imgui.h"

void CounterModule::renderStandalone() {
    ImGui::Begin("Counter Modul");

    static int counter = 0;
    static float timeAccumulator = 0.0f;

    timeAccumulator += ImGui::GetIO().DeltaTime;

    if (timeAccumulator >= 0.3f) {
        timeAccumulator = 0.0f;
        counter++;
    }

    // Zobrazenie aktuálneho počítadla
    ImGui::Text("Aktuálna hodnota counteru: %d", counter);

    ImGui::End();
}
