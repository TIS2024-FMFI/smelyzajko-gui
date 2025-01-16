#include "CounterModuleGraphics.h"

CounterModuleGraphics::CounterModuleGraphics()
        : counter(0), autoscrollEnabled(true),
          scrollbar(100.0f, 0.0f) { // Initialize scrollbar with visibleHeight = 100.0f, totalHeight = 0.0f
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

    // Text area for logs
    float text_area_height = 100.0f;
    float inner_padding = 5.0f;
    ImVec2 scroll_area_min = ImVec2(rect_min.x + inner_padding, rect_max.y + 10.0f + inner_padding);
    ImVec2 scroll_area_max = ImVec2(rect_max.x - inner_padding, rect_max.y + 10.0f + text_area_height - inner_padding);

    draw_list->AddRect(scroll_area_min, scroll_area_max, text_color);

    // Calculate total log height dynamically
    float total_log_height = 0.0f;
    for (const std::string& log : logValues) {
        ImVec2 log_text_size = ImGui::CalcTextSize(log.c_str());
        total_log_height += log_text_size.y + inner_padding;
    }

    // Update scrollbar dimensions
    float visible_height = text_area_height - 2 * inner_padding;
    scrollbar.updateTotalHeight(total_log_height);
    scrollbar.updateVisibleHeight(visible_height);

    // Render logs using the updated scrollOffset
    float log_y_offset = scroll_area_min.y - scrollbar.getScrollOffset();
    draw_list->PushClipRect(scroll_area_min, scroll_area_max, true);
    for (const std::string& log : logValues) {
        ImVec2 log_text_size = ImGui::CalcTextSize(log.c_str());
        if (log_y_offset + log_text_size.y >= scroll_area_min.y &&
            log_y_offset <= scroll_area_max.y) {
            ImVec2 log_pos = ImVec2(scroll_area_min.x + inner_padding, log_y_offset);
            draw_list->AddText(log_pos, text_color, log.c_str());
        }
        log_y_offset += log_text_size.y + inner_padding;
    }
    draw_list->PopClipRect();

    // Conditionally draw scrollbar if needed
    if (total_log_height > visible_height) {
        scrollbar.drawScrollbar(scroll_area_min, scroll_area_max, io);
        scrollbar.updateScrollOffset(io);
    }

    // Autoscroll toggle checkbox
    ImVec2 checkbox_position = ImVec2(scroll_area_max.x - 20.0f, scroll_area_min.y);
    ImGui::SetCursorScreenPos(checkbox_position);
    ImGui::Checkbox("##AutoscrollCheckbox2", &autoscrollEnabled);
    scrollbar.enableAutoscroll(autoscrollEnabled);
}

void CounterModuleGraphics::updateValueOfModule(int value) {
    counter = value;
    std::lock_guard<std::mutex> lock(logMutex);
    logValues.push_back("Counter updated: " + std::to_string(value));
}
