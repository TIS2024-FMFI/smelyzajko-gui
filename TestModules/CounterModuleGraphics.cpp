#include "CounterModuleGraphics.h"
#include "fstream"

CounterModuleGraphics::CounterModuleGraphics()
        : counter(0) {
    setGraphicElementName("CounterGraphicElement");
    startLoggingThread();

}

void CounterModuleGraphics::draw(ImGuiIO &io) {
    logToJson();
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



    std::lock_guard<std::mutex> lock(logMutex); // Protect writing

    std::string filename = logFileDirectory + "/CounterGraphicElement.json";
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
    if (!j.contains("graphicsFrequency")) {
        j["graphicsFrequency"] = getGraphicsFrequency();
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


void CounterModuleGraphics::startLoggingThread() {
    loggingThread = std::thread(&CounterModuleGraphics::loggingThreadFunction, this);
}
void CounterModuleGraphics::stopLoggingThread() {
    loggingThreadRunning = false;
    if (loggingThread.joinable()) {
        loggingThread.join();
    }
}
void CounterModuleGraphics::loggingThreadFunction() {
    std::chrono::milliseconds interval;

    if (graphicsFrequency > 0) {
        interval = std::chrono::milliseconds(60000 / graphicsFrequency);
    } else {
        return;
    }
    while (loggingThreadRunning) {
        logToJson();
        std::this_thread::sleep_for(interval);
    }
}

void CounterModuleGraphics::logFromJson() {

    std::string filename = logFileDirectory + "/CounterGraphicElement.json";
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << filename << std::endl;
        return;
    }

    try {
        nlohmann::json j;
        inFile >> j;
        inFile.close();
        if (j.contains("graphicsFrequency")) {
            graphicsFrequency = j["graphicsFrequency"];
        }
        if (j.contains("counter_values") && j["counter_values"].is_array()) {
            logData.clear();
            for (const auto& value : j["counter_values"]) {
                logData.push_back(value);
            }
        } else {
            std::cerr << "[ERROR] Invalid JSON structure in file: " << filename << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << std::endl;
    }


}

void CounterModuleGraphics::logForward() {
    if (logData.empty()) {
        std::cerr << "[ERROR] No log data loaded." << std::endl;
        return;
    }

    if (currentLogIndex + 1 < logData.size()) {
        ++currentLogIndex;
        counter = logData[currentLogIndex];
    }
}

void CounterModuleGraphics::logBackwards() {

    if (logData.empty()) {
        std::cerr << "[ERROR] No log data loaded." << std::endl;
        return;
    }

    if (currentLogIndex > 0) {
        --currentLogIndex;
        counter = logData[currentLogIndex]; // Update counter to the previous value
    } else {
        std::cerr << "[INFO] Already at the beginning of the logs." << std::endl;
    }
}



