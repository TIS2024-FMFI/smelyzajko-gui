#include "CounterModuleGraphics.h"

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

//    // Dynamically update TextArea width
//    textArea.setWidth(size.x);
//
//    // Draw the text area
//    ImVec2 textAreaPosition = ImVec2(rect_min.x, rect_max.y + 10.0f);
//    textArea.drawTextArea(textAreaPosition, io);
}

void CounterModuleGraphics::updateValueOfModule(int value) {
    counter = value;
}
