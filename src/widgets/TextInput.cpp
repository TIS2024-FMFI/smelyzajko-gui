#include "TextInput.h"
#include <algorithm>
#include "iostream"

void TextInput::draw(ImGuiIO& io) {
    ImGui::SetCursorScreenPos(position);
    ImGui::SetNextItemAllowOverlap();
    ImGui::SetNextItemWidth(size.x);

    if (configurationMode) {
        ImGui::InputText(label.c_str(), "",0,ImGuiInputTextFlags_ReadOnly);
    } else {
        char buffer[256];
        strncpy(buffer, value.c_str(), sizeof(buffer));
        if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer))) {
            value = buffer;
        }
    }

    if (configurationMode) {
        ImRect bbox = getBoundingBox();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 resize_handle_pos = ImVec2(position.x + bbox.GetSize().x + 10.0f, position.y);
        ImVec2 handle_min = ImVec2(resize_handle_pos.x, resize_handle_pos.y);
        ImVec2 handle_max = ImVec2(resize_handle_pos.x + 10.0f, resize_handle_pos.y + 20.0f);
        draw_list->AddRectFilled(handle_min, handle_max, IM_COL32(255, 0, 0, 255)); // Red resize handle
    }
}

void TextInput::handleClicks(ImGuiIO& io) {
    ImRect bbox = getBoundingBox();
    ImVec2 resize_handle_pos = ImVec2(position.x + bbox.GetSize().x + 10.0f, position.y);
    ImVec2 handle_size = ImVec2(10.0f, 20.0f);

    ImGui::SetCursorScreenPos(resize_handle_pos);
    ImGui::InvisibleButton(("ResizeHandle" + label).c_str(), handle_size);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        size.x += delta.x;

        size.x = std::max(size.x, 100.0f); // Prevent shrinking too much
    }

    ImGui::SetCursorScreenPos(position);
    ImGui::InvisibleButton(("TextInput" + label).c_str(), bbox.GetSize());

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        position.x += delta.x;
        position.y += delta.y;
    }
}

ImRect TextInput::getBoundingBox() const {
    ImVec2 textSize = ImGui::CalcTextSize(label.c_str());

    float totalWidth = textSize.x + 10.0f; // Add some padding for the label
    float totalHeight = size.y;

    ImVec2 minPos = position;
    ImVec2 maxPos = ImVec2(position.x + totalWidth + size.x, position.y + totalHeight);
    return ImRect(minPos, maxPos);
}

void TextInput::to_json(nlohmann::json& j) const {
    Element::to_json(j);
    j["type"] = "text-input";
}

void TextInput::from_json(const nlohmann::json& j, ImVec2 resolution) {
    Element::from_json(j, resolution);
}

std::vector<Setting> TextInput::getSettings() {
    return {
            {"moduleName", moduleName, [this](const SettingValue& val) { moduleName = std::get<std::string>(val); }},
            {"label", label, [this](const SettingValue& val) { label = std::get<std::string>(val); }}
    };
}

std::optional<std::string> TextInput::getStringValue() {
    return value;
}
