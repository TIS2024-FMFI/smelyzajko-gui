#include "TextArea.h"
#include "fstream"

TextArea::TextArea()
        : scrollbar(0.0f, 0.0f) {
    setGraphicElementName("Text Area");
}

void TextArea::draw(ImGuiIO& io) {
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    // Define text area boundaries
    float inner_padding = 5.0f;
    ImVec2 text_area_min = position;
    ImVec2 text_area_max = ImVec2(position.x + size.x, position.y + size.y ); // Reduced padding

    // Draw text area boundary
    draw_list->AddRect(text_area_min, text_area_max, IM_COL32(255, 255, 255, 255));

    // Calculate total log height dynamically
    float total_log_height = 0.0f;
    {
        std::lock_guard<std::mutex> lock(logMutex);
        for (const std::string& log : logs) {
            ImVec2 log_text_size = ImGui::CalcTextSize(log.c_str());
            total_log_height += log_text_size.y + inner_padding;
        }
    }

    // Update scrollbar dimensions
    float visible_height = size.x - 2 * inner_padding - 1.0f;
    scrollbar.updateTotalHeight(total_log_height);
    scrollbar.updateVisibleHeight(visible_height);

    // Render logs using the updated scrollOffset
    float log_y_offset = text_area_min.y - scrollbar.getScrollOffset();
    draw_list->PushClipRect(text_area_min, text_area_max, true);

    {
        std::lock_guard<std::mutex> lock(logMutex);
        for (const std::string& log : logs) {
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
    std::lock_guard<std::mutex> lock(logMutex);
    logs.push_back(value);
}
void TextArea::clearLogs() {
    std::lock_guard<std::mutex> lock(logMutex);
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
    if (!isTextLogEnabled()) {
        return;
    }

    static auto lastLogTime = std::chrono::steady_clock::now();
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastLogTime;

    float frequency = getTextFrequency();
    if (frequency > 0) {
        float interval = 60.0f / frequency; // Convert frequency to interval in seconds
        if (elapsed.count() < interval) {
            return;
        }
    }

    lastLogTime = currentTime;

    std::lock_guard<std::mutex> lock(logMutex); // Protect writing

    std::string filename = logFileDirectory+"text_area_"+moduleName+"_log.json";
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
    if (!j.contains("logs")) {
        j["logs"] = nlohmann::json::array();
    }

    // Add new logs
    for (const std::string& log : logs) {
        j["logs"].push_back(log);
    }

    // Overwrite the file with updated content
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << std::setw(4) << j << std::endl;
        outFile.close();
    } else {
        std::cerr << "[ERROR] Could not open file for writing: " << filename << std::endl;
    }
}

