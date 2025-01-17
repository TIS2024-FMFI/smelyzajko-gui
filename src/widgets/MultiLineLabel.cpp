#include "MultiLineLabel.h"

void MultiLineLabel::draw(ImGuiIO &io) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImFont* defaultFont = io.Fonts->Fonts[0];
    ImVec2 text_size = ImGui::CalcTextSize(label.c_str());

    size = ImVec2(text_size.x * font_size / defaultFont->FontSize,
                  text_size.y * font_size / defaultFont->FontSize);
    ImVec2 current_pos = ImVec2(position.x + ImGui::GetWindowPos().x,
                                position.y + ImGui::GetWindowPos().y);

    // Split text into lines by '\n'
    size_t start = 0, end;
    while ((end = label.find('\n', start)) != std::string::npos) {
        std::string line = label.substr(start, end - start);
        draw_list->AddText(defaultFont, font_size, current_pos, IM_COL32(255, 255, 255, 255), line.c_str());
        ImVec2 line_size = ImGui::CalcTextSize(line.c_str());
        current_pos.y += line_size.y * font_size / defaultFont->FontSize; // Move to the next line (scaled)
        start = end + 1;
    }
    // Render the last line
    draw_list->AddText(defaultFont, font_size, current_pos, IM_COL32(255, 255, 255, 255), label.substr(start).c_str());

    size = ImVec2(text_size.x * font_size / defaultFont->FontSize,
                  text_size.y * font_size / defaultFont->FontSize);

    if (configurationMode) {
        createTextSizeButton();
    }
}

void MultiLineLabel::to_json(nlohmann::json& j) const {
    j = nlohmann::json{
            {"type", "multi-line-label"},
            {"label", label},
            {"position", {position.x, position.y}},
            {"size", {size.x, size.y}},
            {"font_size", font_size}
    };
}

void MultiLineLabel::from_json(const nlohmann::json& j, ImVec2 resolution) {
    if (j.contains("type") && j["type"] != "multi-line-label") {
        throw std::invalid_argument("Invalid type for Label: expected 'multi-line-label'");
    }

    Label::from_json(j, resolution);
}
