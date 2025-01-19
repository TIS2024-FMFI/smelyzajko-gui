#include "Button.h"
#include "iostream"

ImRect Button::getBoundingBox() const {
    return ImRect(position, ImVec2(position.x + size.x, position.y + size.y));
}

void Button::draw(ImGuiIO& io) {
    // Draw the button outline
    ImGui::SetCursorScreenPos(position);
    ImGui::SetNextItemAllowOverlap();
    if (ImGui::Button(label.c_str(), size)) {
        clicked = true;
    }

    // Draw the resize handle
    if (configurationMode) {
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImVec2 resize_handle_pos = ImVec2(position.x + size.x, position.y + size.y);
        ImVec2 handle_size = ImVec2(10.0f, 10.0f);
        ImVec2 handle_min = ImVec2(resize_handle_pos.x + ImGui::GetWindowPos().x - handle_size.x,
                                   resize_handle_pos.y + ImGui::GetWindowPos().y - handle_size.y);
        ImVec2 handle_max = ImVec2(resize_handle_pos.x + ImGui::GetWindowPos().x,
                                   resize_handle_pos.y + ImGui::GetWindowPos().y);
        draw_list->AddRectFilled(handle_min, handle_max, IM_COL32(255, 0, 0, 255)); // Red handle
    }
}

void Button::handleClicks(ImGuiIO &io) {
    ImVec2 resize_handle_pos = ImVec2(position.x + size.x, position.y + size.y);
    ImVec2 handle_size = ImVec2(10.0f, 10.0f);

    ImGui::SetCursorScreenPos(ImVec2(resize_handle_pos.x - handle_size.x, resize_handle_pos.y - handle_size.y));
    ImGui::InvisibleButton(("ResizeHandle" + label).c_str(), handle_size);
    bool resizing = false;
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        size.x += delta.x;
        size.y += delta.y;

        size.x = std::max(size.x, 50.0f); // Minimum width
        size.y = std::max(size.y, 20.0f); // Minimum height

        resizing = true;
    }

    if (!resizing) {
        ImGui::SetCursorScreenPos(position);
        ImGui::InvisibleButton(("Button" + label).c_str(), size);
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            ImVec2 delta = io.MouseDelta;
            position.x += delta.x;
            position.y += delta.y;
        }
    }
}

void Button::to_json(nlohmann::json &j) const {
    Element::to_json(j);
    j["type"] = "button";
}

void Button::from_json(const nlohmann::json &j, ImVec2 resolution) {
    if (j.contains("type") && j["type"] != "button") {
        throw std::invalid_argument("Invalid type for Checkbox: expected 'button'");
    }

    Element::from_json(j, resolution);

}

std::vector<Setting> Button::getSettings() {
    return {
            {"moduleName",moduleName, [this](const SettingValue& val) { moduleName = std::get<std::string>(val); }},
            {"label", label, [this](const SettingValue& val) { label = std::get<std::string>(val); }}
    };
}

std::optional<std::string> Button::getStringValue()  {
    if (clicked) {
        clicked = false;
        return label;
    }
    return std::nullopt;
}
