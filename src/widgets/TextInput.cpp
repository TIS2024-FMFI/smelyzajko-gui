#include "TextInput.h"
#include <algorithm>

void TextInput::draw(ImGuiIO& io) {
    ImGui::SetCursorScreenPos(position);
    ImGui::SetNextItemAllowOverlap();
    ImGui::SetNextItemWidth(size.x);

    // Always disabled
    ImGui::InputText(label.c_str(), "", 0, ImGuiInputTextFlags_ReadOnly);

    ImRect bbox = getBoundingBox();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 resize_handle_pos = ImVec2(position.x + bbox.GetSize().x + 10.0f, position.y);
    ImVec2 handle_min = ImVec2(resize_handle_pos.x, resize_handle_pos.y);
    ImVec2 handle_max = ImVec2(resize_handle_pos.x + 10.0f, resize_handle_pos.y + size.y);
    draw_list->AddRectFilled(handle_min, handle_max, IM_COL32(255, 0, 0, 255)); // Red resize handle
}

void TextInput::handleClicks(ImGuiIO& io) {
    ImRect bbox = getBoundingBox();
    ImVec2 resize_handle_pos = ImVec2(position.x + bbox.GetSize().x + 10.0f, position.y);
    ImVec2 handle_size = ImVec2(10.0f, 10.0f);

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
    j = nlohmann::json{
            {"type", "text-input"},
            {"label", label},
            {"position", {position.x, position.y}},
            {"size", {size.x, size.y}},
            {"moduleId", moduleId}
    };
}

void TextInput::from_json(const nlohmann::json& j, ImVec2 resolution) {
    Element::from_json(j, resolution);

    if (j.contains("moduleId")) {
        moduleId = j["moduleId"].get<int>();
    }

    ImVec2 scale = Element::getScaleFactors(resolution);
    position = ImVec2(position.x * scale.x, position.y * scale.y);
    size = ImVec2(size.x * scale.x, size.y * scale.y);
}

std::vector<Setting> TextInput::getSettings() {
    return {
            {"moduleId", moduleId, [this](const SettingValue& val) { moduleId = std::get<int>(val); }},
            {"label", label, [this](const SettingValue& val) { label = std::get<std::string>(val); }}
    };
}
