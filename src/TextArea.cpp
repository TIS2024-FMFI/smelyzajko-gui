#include "TextArea.h"

TextArea::TextArea(float width, float height, const std::string& id)
        : width(width),          // Initialize the width of the text area
          height(height),        // Initialize the height of the text area
          id(id),                // Set the unique identifier for the text area
          scrollbar(height, 0.0f), // Initialize the scrollbar with height
          autoscrollEnabled(true) // Enable autoscroll by default
{
}

void TextArea::setWidth(float newWidth) {
    width = newWidth; // Dynamically adjust the width
}

void TextArea::drawTextArea(ImVec2 position, ImGuiIO& io) {
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    // Define text area boundaries
    float inner_padding = 5.0f;
    ImVec2 text_area_min = position;
    ImVec2 text_area_max = ImVec2(position.x + width, position.y + height - 15.0f); // Reduced padding

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
    float visible_height = height - 2 * inner_padding - 1.0f;
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
    std::string checkbox_id = "##AutoscrollCheckbox_" + id;
    ImVec2 checkbox_position = ImVec2(text_area_max.x - 20.0f, text_area_min.y); // Adjust checkbox position
    ImGui::SetCursorScreenPos(checkbox_position);
    ImGui::Checkbox(checkbox_id.c_str(), &autoscrollEnabled);
    scrollbar.enableAutoscroll(autoscrollEnabled);
}

void TextArea::addLog(const std::string& log) {
    std::lock_guard<std::mutex> lock(logMutex);
    logs.push_back(log);
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
