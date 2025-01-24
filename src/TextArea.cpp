#include "TextArea.h"
#include "fstream"
#include "sstream"


TextArea::TextArea()
: scrollbar(0.0f, 0.0f) {
    setGraphicElementName("TextArea");
}

void TextArea::startLoggingThread() {
    if (!graphicsLogEnabled){
        return;
    }
    loggingThreadRunning = true;
    loggingThread = std::thread(&TextArea::loggingThreadFunction, this);

}
void TextArea::stopLoggingThread() {
    loggingThreadRunning = false;
    if (loggingThread.joinable()) {
        loggingThread.join();
    }
}


void TextArea::loggingThreadFunction() {


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

void TextArea::draw(ImGuiIO& io) {

    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    // Define text area boundaries
    float inner_padding = 5.0f;
    ImVec2 text_area_min = position;
    ImVec2 text_area_max = ImVec2(position.x + size.x, position.y + size.y); // Reduced padding

    // Draw text area boundary
    draw_list->AddRect(text_area_min, text_area_max, IM_COL32(255, 255, 255, 255));

    // Calculate total log height dynamically
    float total_log_height = 0.0f;
    std::vector<std::string> wrapped_logs;
    {
        for (const std::string& log : logs) {
            ImVec2 log_text_size = ImGui::CalcTextSize(log.c_str());
            if (log_text_size.x > size.x - 2 * inner_padding) {
                // Word wrapping
                std::istringstream iss(log);
                std::string word;
                std::string current_line;
                while (iss >> word) {
                    std::string test_line = current_line.empty() ? word : current_line + " " + word;
                    ImVec2 test_line_size = ImGui::CalcTextSize(test_line.c_str());
                    if (test_line_size.x > size.x - 2 * inner_padding) {
                        wrapped_logs.push_back(current_line);
                        current_line = word;
                    } else {
                        current_line = test_line;
                    }
                }
                if (!current_line.empty()) {
                    wrapped_logs.push_back(current_line);
                }
            } else {
                wrapped_logs.push_back(log);
            }
            total_log_height += log_text_size.y + inner_padding;
        }
    }

    // Update scrollbar dimensions
    float visible_height = size.y - 2 * inner_padding - 1.0f;
    scrollbar.updateTotalHeight(total_log_height);
    scrollbar.updateVisibleHeight(visible_height);

    // Render logs using the updated scrollOffset
    float log_y_offset = text_area_min.y - scrollbar.getScrollOffset();
    draw_list->PushClipRect(text_area_min, text_area_max, true);

    {
        for (const std::string& log : wrapped_logs) {
            ImVec2 log_text_size = ImGui::CalcTextSize(log.c_str());
            if (log_y_offset + log_text_size.y >= text_area_min.y &&
                log_y_offset <= text_area_max.y) {
                ImVec2 log_pos = ImVec2(text_area_min.x + inner_padding, log_y_offset);
                draw_list->AddText(log_pos, IM_COL32(255, 255, 255, 255), log.c_str());
            }
            log_y_offset += log_text_size.y + inner_padding;
        }
    }

    draw_list->PopClipRect();

    // Draw and update the scrollbar
    if (total_log_height > visible_height) {
        scrollbar.drawScrollbar(text_area_min, text_area_max, io);
        scrollbar.updateScrollOffset(io);
    }

    // Unique checkbox id using the text area's id
    std::string checkbox_id = "##AutoscrollCheckbox_" + moduleName;
    ImVec2 checkbox_position = ImVec2(text_area_max.x - 20.0f, text_area_min.y); // Adjust checkbox position
    ImGui::SetCursorScreenPos(checkbox_position);
    ImGui::Checkbox(checkbox_id.c_str(), &autoscrollEnabled);
    scrollbar.enableAutoscroll(autoscrollEnabled);
}

void TextArea::updateValueOfModule(std::string value) {
    logs.push_back(value);
    logToJson();
}
void TextArea::clearLogs() {

    logs.clear();
}

void TextArea::setAutoscrollEnabled(bool enabled) {
    autoscrollEnabled = enabled;
    scrollbar.enableAutoscroll(enabled);
}

bool TextArea::isAutoscrollEnabled() const {
    return autoscrollEnabled;
}

void TextArea::logToJson() {
    std::lock_guard<std::mutex> lock(logMutex); // Ensure thread-safe access to logs

    std::string filename = logFileDirectory + "/TextArea.json";
    nlohmann::json j;

    // Load existing content from the JSON file
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        try {
            inFile >> j;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << std::endl;
        }
        inFile.close();
    }

    // Ensure the structure is initialized
    if (!j.contains("textFrequency")) {
        j["textFrequency"] = getTextFrequency();
    }
    if (!j.contains("logs")) {
        j["logs"] = nlohmann::json::array();
    }

    // Check the last non-empty log from JSON and compare it with the last log in logs
    std::string lastJsonLog = "EMPTY";
    if (!j["logs"].empty()) {
        // Find the last non-empty log in the existing JSON
        for (auto it = j["logs"].rbegin(); it != j["logs"].rend(); ++it) {
            if (*it != "EMPTY") {
                lastJsonLog = *it;
                break;
            }
        }
    }

    // Get the most recent log from logs
    std::string currentLog = logs.empty() ? "EMPTY" : logs.back();

    // Compare the last non-empty log in JSON with the last log in logs
    if (lastJsonLog != currentLog) {
        // If they don't match, add the new log to JSON
        j["logs"].push_back(currentLog);
    }else{
        j["logs"].push_back("EMPTY");
    }

    // Overwrite the JSON file with updated content
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << std::setw(4) << j << std::endl;
        outFile.close();
    } else {
        std::cerr << "[ERROR] Could not open file for writing: " << filename << std::endl;
    }
}




void TextArea::logFromJson() {
    std::lock_guard<std::mutex> lock(logMutex);

    std::string filename = logFileDirectory + "/TextArea.json";
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << filename << std::endl;
        return;
    }

    try {
        nlohmann::json j;
        inFile >> j;
        inFile.close();

        // Load log frequency
        if (j.contains("textFrequency")) {
            textFrequency = j["textFrequency"];
        } else {
            std::cerr << "Error: No textFrequency found in JSON.\n";
        }

        // Load logs into loadedLogs instead of logs
        if (j.contains("logs") && j["logs"].is_array()) {
            loadedLogs.clear(); // Clear existing loaded logs
            for (const auto& logEntry : j["logs"]) {
                loadedLogs.push_back(logEntry);  // Add to loadedLogs
            }
            std::cout << "[INFO] Successfully loaded logs from JSON." << std::endl;
        } else {
            std::cerr << "[ERROR] Invalid JSON structure in file: " << filename << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << std::endl;
    }
}


void TextArea::logForward() {
    // Check if we are at the end of the loaded logs
    if (currentLogIndex >= loadedLogs.size()) {
        return;
    }

    // Check the current log at currentLogIndex
    if (loadedLogs[currentLogIndex] == "EMPTY") {

        ++currentLogIndex;
        return;
    }

    // Add the log to logs and increment the index

    logs.push_back(loadedLogs[currentLogIndex]);
    ++currentLogIndex;

}




void TextArea::logBackwards() {
    if (logs.empty()) {
        return;
    }

    // Check if the last entry in logs is "EMPTY"
    if (logs.back() == "EMPTY") {
        if (currentLogIndex > 0) {
            --currentLogIndex;
        }
        return;
    }

    // Remove the last log from logs
    std::string removedLog = logs.back();
    logs.pop_back();

    // Decrement the currentLogIndex only if it is greater than 0
    if (currentLogIndex > 0) {
        --currentLogIndex;
    }

    std::cout << "[INFO] Removed log: " << removedLog << std::endl;
}


