#include "Button.h"
#include "iostream"

ImVec2 Button::getSize() const {
    return size;
}

void Button::setSize(const ImVec2& newSize) {
    size = newSize;
}

ImRect Button::getBoundingBox() const {
    return ImRect(position, ImVec2(position.x + size.x, position.y + size.y));
}

void Button::draw(ImGuiIO& io) {
    ImVec2 resize_handle_pos = ImVec2(position.x + size.x, position.y + size.y);
    ImVec2 handle_size = ImVec2(10.0f, 10.0f);

    // setStyles();

    // Draw the button outline
    ImGui::SetCursorScreenPos(position);
    ImGui::SetNextItemAllowOverlap();
    ImGui::Button(label.c_str(), size);

    // removeStyles();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // Draw the resize handle
    ImVec2 handle_min = ImVec2(resize_handle_pos.x + ImGui::GetWindowPos().x - handle_size.x, resize_handle_pos.y + ImGui::GetWindowPos().y - handle_size.y);
    ImVec2 handle_max = ImVec2(resize_handle_pos.x + ImGui::GetWindowPos().x, resize_handle_pos.y + ImGui::GetWindowPos().y);
    draw_list->AddRectFilled(handle_min, handle_max, IM_COL32(255, 0, 0, 255)); // Red handle
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
    j = nlohmann::json{
            {"type", "button"},
            {"label", label},
            {"position", {position.x, position.y}},
            {"size", {size.x, size.y}},
            {"moduleID", moduleID}
    };
}

void Button::from_json(const nlohmann::json &j, ImVec2 resolution) {
    if (j.contains("type") && j["type"] != "button") {
        throw std::invalid_argument("Invalid type for Checkbox: expected 'button'");
    }

    Element::from_json(j, resolution);

    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
    }
    if (j.contains("moduleID") && j["moduleID"].is_number_integer()) {
        moduleID = j["moduleID"];
    } else {
        moduleID = -1;
    }

    ImVec2 scale = Element::getScaleFactors(resolution);

    position = ImVec2(position.x * scale.x, position.y * scale.y);
    size = ImVec2(size.x * scale.x, size.y * scale.y);
}

void Button::setStyles() {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 120, 200, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(80, 150, 240, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(30, 100, 180, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 255));
}

void Button::removeStyles() {
    ImGui::PopStyleVar(1); // Restore the default border size
    ImGui::PopStyleColor(4); // Restore the previous colors (Button, Hovered, Active, Border)
}

std::vector<Setting> Button::getSettings() {
    return {
            {"moduleID",moduleID, [this](const SettingValue& val) { moduleID = std::get<int>(val); }},
            {"label", label, [this](const SettingValue& val) { label = std::get<std::string>(val); }}
    };
}