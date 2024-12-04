#include "SingleLineLabel.h"

void SingleLineLabel::draw(ImGuiIO &io) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImFont* defaultFont = io.Fonts->Fonts[0];

    ImVec2 text_size = ImGui::CalcTextSize(label.c_str());
    size = ImVec2(text_size.x * font_size / defaultFont->FontSize,
                  text_size.y * font_size / defaultFont->FontSize);

    draw_list->AddText(defaultFont, font_size, position, IM_COL32(255, 255, 255, 255), label.c_str());

    createTextSizeButton();
}
