#include "SingleLineLabel.h"

void SingleLineLabel::draw(ImGuiIO &io) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImFont* defaultFont = io.Fonts->Fonts[0];

    ImVec2 text_size = ImGui::CalcTextSize(label.c_str());
    size = ImVec2(text_size.x * font_size / defaultFont->FontSize,
                  text_size.y * font_size / defaultFont->FontSize);

    draw_list->AddText(defaultFont, font_size, position, IM_COL32(255, 255, 255, 255), label.c_str());

    if (configurationMode) {
        createTextSizeButton();
    }
}

void SingleLineLabel::to_json(nlohmann::json& j) const {
    j = nlohmann::json{
            {"type", "single-line-label"},
            {"label", label},
            {"position", {position.x, position.y}},
            {"size", {size.x, size.y}},
            {"font_size", font_size}
    };
}

void SingleLineLabel::from_json(const nlohmann::json& j, ImVec2 resolution) {
    if (j.contains("type") && j["type"] != "single-line-label") {
        throw std::invalid_argument("Invalid type for Label: expected 'single-line-label'");
    }

    Label::from_json(j, resolution);

    ImVec2 scale = Element::getScaleFactors(resolution);

    position = ImVec2(position.x * scale.x, position.y * scale.y);
}
