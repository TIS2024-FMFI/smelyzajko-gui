#include "Label.h"

void Label::setText(const std::string &newText) {
    label = newText;
}

const std::string &Label::getText() const {
    return label;
}

ImRect Label::getBoundingBox() const {
    return ImRect(position, ImVec2(position.x + size.x, position.y + size.y));
}

void Label::handleClicks(ImGuiIO &io) {
    ImGui::SetCursorPos(position);
    ImGui::InvisibleButton(("Label" + label).c_str(), size);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        position.x += delta.x;
        position.y += delta.y;
    }

}

void Label::createTextSizeButton() {
    float button_width = 30.0f;
    float button_spacing = 5.0f;
    float button_height = 20;
    ImVec2 button_start_pos = ImVec2(position.x + size.x + 10.0f, position.y + (size.y - button_height * 2 - button_spacing) / 2);

    ImGui::SetCursorScreenPos(button_start_pos);
    if (ImGui::Button("+", ImVec2(button_width, button_height))) {
        font_size = std::min(font_size + 1.0f, 40.0f);
    }

    ImGui::SetCursorScreenPos(ImVec2(button_start_pos.x, button_start_pos.y + button_height + button_spacing));
    if (ImGui::Button("-", ImVec2(button_width, button_height))) {
        font_size = std::max(font_size - 1.0f, 8.0f);
    }
}

void Label::from_json(const nlohmann::json &j) {
    Element::from_json(j);

    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
    } else {
        size = ImVec2(100.0f, 25.0f);
    }

    if (j.contains("font_size") && j["font_size"].is_number()) {
        font_size = j["font_size"];
    } else {
        font_size = 13.0f;
    }
}

