#include "Rectangle.h"
#include <iostream>
ImVec2 Rectangle::getSize() const {
    return size;
}

float Rectangle::getWidth() const {
    return size.x;
}

float Rectangle::getHeight() const {
    return size.y;
}

void Rectangle::setSize(const ImVec2 &newSize) {
    size = newSize;
}

void Rectangle::setWidth(float newWidth) {
    size.x = newWidth;
}

void Rectangle::setHeight(float newHeight) {
    size.y = newHeight;
}

void Rectangle::draw(ImGuiIO& io) {
    ImVec2 resize_handle_pos = ImVec2(position.x + size.x, position.y + size.y);
    ImVec2 handle_size = ImVec2(10, 10);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 rect_min = ImVec2(position.x, position.y);
    ImVec2 rect_max = ImVec2(position.x + size.x, position.y + size.y);

    draw_list->AddRectFilled(rect_min, rect_max, IM_COL32(20, 20, 255, 255));
    draw_list->AddRect(rect_min, rect_max, IM_COL32(255, 255, 255, 255)); // White outline
    if (configurationMode) {
        // Draw the resizing handle
        ImVec2 handle_screen_pos_min = ImVec2(resize_handle_pos.x + ImGui::GetWindowPos().x - handle_size.x,
                                              resize_handle_pos.y + ImGui::GetWindowPos().y - handle_size.y);
        ImVec2 handle_screen_pos_max = ImVec2(resize_handle_pos.x + ImGui::GetWindowPos().x,
                                              resize_handle_pos.y + ImGui::GetWindowPos().y);
        draw_list->AddRectFilled(handle_screen_pos_min, handle_screen_pos_max, IM_COL32(255, 0, 0, 255)); // Red handle

        ImVec2 text_size = ImGui::CalcTextSize(label.c_str());
        ImVec2 text_pos = ImVec2(position.x + (size.x - text_size.x) / 2,
                                 position.y + (size.y - text_size.y) / 2); // Centering the text
        draw_list->AddText(ImVec2(text_pos.x + ImGui::GetWindowPos().x, text_pos.y + ImGui::GetWindowPos().y), IM_COL32(255, 255, 255, 255), label.c_str());
    }
}
ImRect Rectangle::getBoundingBox() const {
    return ImRect(position, ImVec2(position.x + size.x, position.y + size.y));
}

void Rectangle::handleClicks(ImGuiIO &io) {
    ImVec2 resize_handle_pos = ImVec2(position.x + size.x, position.y + size.y);
    ImVec2 handle_size = ImVec2(10, 10);

    ImGui::SetCursorScreenPos(ImVec2(resize_handle_pos.x - handle_size.x, resize_handle_pos.y - handle_size.y));
    ImGui::InvisibleButton(("ResizeHandle" + label).c_str(), handle_size);
    bool resizing = false;
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        size.x += delta.x;
        size.y += delta.y;

        size.x = std::max(size.x, 20.0f); // Minimum width
        size.y = std::max(size.y, 20.0f); // Minimum height

        resizing = true;
    }

    // Only allow moving the rectangle if resizing is NOT active
    if (!resizing) {
        ImGui::SetCursorPos(position);
        ImGui::InvisibleButton(("Rect" + label).c_str(), size);

        // Moving the rectangle
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            ImVec2 delta = io.MouseDelta;
            position.x += delta.x;
            position.y += delta.y;
        }
    }
}

void Rectangle::to_json(nlohmann::json& j) const {
    j = nlohmann::json{
            {"type", "rectangle"},
            {"label", label},
            {"position", {position.x, position.y}},
            {"size", {size.x, size.y}},
            {"moduleId", moduleId},
            {"graphicModuleId", graphicModuleId},
            {"graphicsFrequency", graphicsFrequency},
            {"graphicsLogEnabled", graphicsLogEnabled},
            {"textFrequency", textFrequency},
            {"textLogEnabled", textLogEnabled},
            {"name",label}
    };
}

void Rectangle::from_json(const nlohmann::json& j) {
    if (j.contains("type") && j["type"] != "rectangle") {
        throw std::invalid_argument("Invalid type for Rectangle: expected 'rectangle'");
    }

    Element::from_json(j);
    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
    } else {
        size = ImVec2(100.0f, 50.0f);
    }
    if (j.contains("moduleId") && j["moduleId"].is_number_integer()) {
        moduleId = j["moduleId"];
    } else {
        moduleId = -1;
    }
    if (j.contains("graphicModuleId") && j["graphicModuleId"].is_number_integer()) {
        graphicModuleId = j["graphicModuleId"];
    } else {
        graphicModuleId = -1;
    }
    if (j.contains("graphicsFrequency") && j["graphicsFrequency"].is_number_integer()) {
        graphicsFrequency = j["graphicsFrequency"];
    } else {
        graphicsFrequency = 0;
    }
    if (j.contains("graphicsLogEnabled") && j["graphicsLogEnabled"].is_boolean()) {
        graphicsLogEnabled = j["graphicsLogEnabled"];
    } else {
        graphicsLogEnabled = false;
    }
    if (j.contains("textFrequency") && j["textFrequency"].is_number_integer()) {
        textFrequency = j["textFrequency"];
    } else {
        textFrequency = 0;
    }
    if (j.contains("textLogEnabled") && j["textLogEnabled"].is_boolean()) {
        textLogEnabled = j["textLogEnabled"];
    } else {
        textLogEnabled = false;
    }
    if (j.contains("name") && j["name"].is_string() && !j["name"].empty() && label.empty()) {
        label = j["name"];
    }

}
std::vector<Setting> Rectangle::getSettings() {

    if (label.find("Rectangle") != std::string::npos) {
        return {
                {"label",label, [this](const SettingValue &val) { label = std::get<std::string>(val); }}
        };
    }
    else {
        return {
                {"moduleId",moduleId, [this](const SettingValue &val) { moduleId = std::get<int>(val); }},
                {"graphicModuleId",graphicModuleId, [this](const SettingValue &val) { graphicModuleId = std::get<int>(val); }},
                {"graphicsFrequency",graphicsFrequency, [this](const SettingValue &val) { graphicsFrequency = std::get<int>(val); }},
                {"graphicsLogEnabled",graphicsLogEnabled, [this](const SettingValue &val) { graphicsLogEnabled = std::get<bool>(val); }},
                {"textFrequency",textFrequency, [this](const SettingValue &val) { textFrequency = std::get<int>(val); }},
                {"textLogEnabled",textLogEnabled, [this](const SettingValue &val) { textLogEnabled = std::get<bool>(val); }}
        };
        }
    }

