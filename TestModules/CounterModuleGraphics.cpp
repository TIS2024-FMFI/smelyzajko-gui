#include "CounterModuleGraphics.h"


CounterModuleGraphics::CounterModuleGraphics() {
    setGraphicElementName("Counter Graphic Element");
}


void CounterModuleGraphics::draw(ImGuiIO &io) {

        ImDrawList* draw_list = ImGui::GetForegroundDrawList();

        // Text content
        std::string text = "Counter: " + std::to_string(counter);

        // Define the color of the text (white) and border
        ImU32 text_color = IM_COL32(255, 255, 255, 255);
        ImU32 border_color = IM_COL32(255, 255, 255, 255); // White border

        const char* text_cstr = text.c_str();
        ImVec2 text_size = ImGui::CalcTextSize(text_cstr);

        ImVec2 rect_min = position;
        ImVec2 rect_max = ImVec2(position.x + size.x, position.y + size.y);
        const float border_thickness = 2.0f;

        draw_list->AddRect(rect_min, rect_max, border_color, 0.0f, 0, border_thickness);
        ImVec2 text_pos = ImVec2((rect_min.x + rect_max.x) / 2.0f - text_size.x / 2.0f, (rect_min.y + rect_max.y) / 2.0f - text_size.y / 2.0f);

        draw_list->AddText(text_pos, text_color, text_cstr);
}





void CounterModuleGraphics::updateValueOfModule(int value) {
    counter = value;
}





