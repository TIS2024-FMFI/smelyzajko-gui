#include "CounterModuleGraphics.h"
#include "fstream"

CounterModuleGraphics::CounterModuleGraphics()
        : counter(0) {
    setGraphicElementName("Counter Graphic Element");
}

void CounterModuleGraphics::draw(ImGuiIO &io) {
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();
    ImU32 text_color = IM_COL32(255, 255, 255, 255);

    // Draw module boundary
    ImVec2 rect_min = position;
    ImVec2 rect_max = ImVec2(position.x + size.x, position.y + size.y);
    draw_list->AddRect(rect_min, rect_max, text_color);

    // Display counter text
    std::string text = "Counter: " + std::to_string(counter);
    ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
    ImVec2 text_pos = ImVec2((rect_min.x + rect_max.x) / 2.0f - text_size.x / 2.0f,
                             (rect_min.y + rect_max.y) / 2.0f - text_size.y / 2.0f);
    draw_list->AddText(text_pos, text_color, text.c_str());

}

void CounterModuleGraphics::updateValueOfModule(int value) {
    counter = value;
}

void CounterModuleGraphics::logToJson() {
    if (!isGraphicsLogEnabled()) {
        return;
    }

    static auto lastLogTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastLogTime;

    float frequency = getGraphicsFrequency();
    if (frequency > 0) {
        float interval = 60.0f / frequency; // Convert frequency to interval in seconds
        if (elapsed.count() < interval) {
            return;
        }
    }

    lastLogTime = currentTime;

    std::lock_guard<std::mutex> lock(logMutex); // Protect writing

    std::string filename = logFileDirectory+"/counter_log.json";
    nlohmann::json j;

    // Load existing content
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> j;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << std::endl;
        }
        inFile.close();
    }

    // Initialize the file if empty
    if (!j.contains("counter_values")) {
        j["counter_values"] = nlohmann::json::array();
    }

    // Add new value
    j["counter_values"].push_back(counter);

    // Overwrite the file with updated content
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << std::setw(4) << j << std::endl;
        outFile.close();
    } else {
        std::cerr << "[ERROR] Could not open file for writing: " << filename << std::endl;
    }
}


