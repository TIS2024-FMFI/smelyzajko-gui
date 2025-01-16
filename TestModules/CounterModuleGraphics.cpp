#include "CounterModuleGraphics.h"

CounterModuleGraphics::CounterModuleGraphics()
        : counter(0), scrollOffset(0.0f), autoscrollEnabled(true) {}

void CounterModuleGraphics::draw(ImGuiIO& io) {
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

    float log_y_offset = scroll_area_min.y - scrollOffset;
    float visible_height = text_area_height - 2 * inner_padding;
    float total_log_height = 0.0f;

    draw_list->PushClipRect(scroll_area_min, scroll_area_max, true);
    for (const std::string& log : logValues) {
        ImVec2 log_text_size = ImGui::CalcTextSize(log.c_str());
        total_log_height += log_text_size.y + inner_padding;

        if (log_y_offset + log_text_size.y >= scroll_area_min.y &&
            log_y_offset <= scroll_area_max.y) {
            ImVec2 log_pos = ImVec2(scroll_area_min.x + inner_padding, log_y_offset);
            draw_list->AddText(log_pos, text_color, log.c_str());
        }
        log_y_offset += log_text_size.y + inner_padding;
    }
    draw_list->PopClipRect();

    // Render scrollbar only if the total log height exceeds the visible area
    if (total_log_height > visible_height) {
        float scrollbar_width = 20.0f;
        ImVec2 scrollbar_min = ImVec2(scroll_area_max.x + inner_padding, scroll_area_min.y);
        ImVec2 scrollbar_max = ImVec2(scroll_area_max.x + scrollbar_width, scroll_area_max.y);
        draw_list->AddRectFilled(scrollbar_min, scrollbar_max, IM_COL32(180, 180, 180, 255));

        float scrollbar_thumb_height = std::max((visible_height / total_log_height) * visible_height, 10.0f);
        scrollbar_thumb_height = std::min(scrollbar_thumb_height, visible_height);
        float thumb_offset = (scrollOffset / std::max(1.0f, total_log_height)) * visible_height;
        thumb_offset = std::clamp(thumb_offset, 0.0f, visible_height - scrollbar_thumb_height);

        ImVec2 thumb_min = ImVec2(scrollbar_min.x, scrollbar_min.y + thumb_offset);
        ImVec2 thumb_max = ImVec2(scrollbar_max.x, thumb_min.y + scrollbar_thumb_height);
        draw_list->AddRectFilled(thumb_min, thumb_max, IM_COL32(100, 100, 100, 255));

        // Handle manual scrollbar interaction
        if (!autoscrollEnabled && ImGui::IsMouseHoveringRect(scrollbar_min, scrollbar_max) &&
            ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            ImVec2 mouse_pos = io.MousePos;
            float mouse_scroll_position = mouse_pos.y - scrollbar_min.y - scrollbar_thumb_height / 2;
            scrollOffset = (mouse_scroll_position / visible_height) * total_log_height;
            scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, total_log_height - visible_height));
        }
    }

    // Handle autoscroll
    if (autoscrollEnabled && !logValues.empty()) {
        scrollOffset = std::max(0.0f, total_log_height - visible_height);
    }

    // Handle mouse wheel scrolling
    if (ImGui::IsMouseHoveringRect(scroll_area_min, scroll_area_max)) {
        scrollOffset -= io.MouseWheel * 20.0f;
        scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, total_log_height - visible_height));
    }

    // Autoscroll toggle checkbox in the top-right corner of the text area
    ImVec2 checkbox_position = ImVec2(scroll_area_max.x - 20.0f, scroll_area_min.y);
    ImGui::SetCursorScreenPos(checkbox_position);
    ImGui::Checkbox("##AutoscrollCheckbox2", &autoscrollEnabled); // Invisible label with a unique ID
}







void CounterModuleGraphics::updateValueOfModule(int value) {
    counter = value;
    std::lock_guard<std::mutex> lock(logMutex);
    logValues.push_back("Counter updated: " + std::to_string(value));
}


void CounterModuleGraphics::updateValueOfModule(std::string value) {
    std::cerr << "Invalid value for MapModuleGraphics." << std::endl;
}

void CounterModuleGraphics::updateValueOfModule(std::vector<int> value) {
    std::cerr << "Invalid value for MapModuleGraphics." << std::endl;

}







void CounterModuleGraphics::from_json(const nlohmann::json &j) {
    if (j.contains("graphicModuleId") && j["graphicModuleId"].is_number_integer()) {
        graphicModuleId = j["graphicModuleId"];
    }
    if (j.contains("moduleId") && j["moduleId"].is_number_integer()) {
        moduleId = j["moduleId"];
    }
    if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2) {
        position.x = j["position"][0];
        position.y = j["position"][1];
        setPos({j["position"][0], j["position"][1]});
    }
    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
        setSize({j["size"][0], j["size"][1]});
    }
    if (j.contains("graphicsFrequency") && j["graphicsFrequency"].is_number_float()) {
        graphicsFrequency = j["graphicsFrequency"];
    }
    if (j.contains("graphicsLogEnabled") && j["graphicsLogEnabled"].is_boolean()) {
        graphicsLogEnabled = j["graphicsLogEnabled"];
    }
    if (j.contains("textFrequency") && j["textFrequency"].is_number_float()) {
        textFrequency = j["textFrequency"];
    }
    if (j.contains("textLogEnabled") && j["textLogEnabled"].is_boolean()) {
        textLogEnabled = j["textLogEnabled"];
    }
}
void CounterModuleGraphics::to_json(nlohmann::json &j) const {
    j["graphicModuleId"] = graphicModuleId;
    j["moduleId"] = moduleId;
    j["name"] = moduleName;
    j["position"] = {position.x, position.y};
    j["size"] = {size.x, size.y};
    j["graphicsFrequency"] = graphicsFrequency;
    j["graphicsLogEnabled"] = graphicsLogEnabled;
    j["textFrequency"] = textFrequency;
    j["textLogEnabled"] = textLogEnabled;
}


