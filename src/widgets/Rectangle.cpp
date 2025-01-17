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

            {"graphicElementId",graphicElementId},
            {"graphicElementName", graphicElementName},
            {"moduleId", moduleId},
            {"moduleName", moduleName},
            {"position", {roundToOneDecimal(position.x), roundToOneDecimal(position.y)}},
            {"size", {roundToOneDecimal(size.x), roundToOneDecimal(size.y)}},
            {"graphicsFrequency", graphicsFrequency},
            {"graphicsLogEnabled", graphicsLogEnabled},
            {"textFrequency", textFrequency},
            {"textLogEnabled", textLogEnabled},
            {"type", "rectangle"}

    };
}

void Rectangle::from_json(const nlohmann::json& j, ImVec2 resolution) {
    Element::from_json(j, resolution);
    if (j.contains("graphicElementId") && j["graphicElementId"].is_number_integer()) {
        graphicElementId = j["graphicElementId"];
    }
    if (j.contains("graphicElementName") && j["graphicElementName"].is_string()) {
        graphicElementName = j["graphicElementName"];
        label = graphicElementName ;
    }
    if (j.contains("moduleId") && j["moduleId"].is_number_integer()) {
        moduleId = j["moduleId"];
    }
    if (j.contains("moduleName") && j["moduleName"].is_string()) {
        moduleName = j["moduleName"];
        label += "\n" + moduleName;
    }
    if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2) {
        position.x = j["position"][0];
        position.y = j["position"][1];
    }
    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
        setSize({j["size"][0], j["size"][1]});
    }
    if (j.contains("graphicsFrequency") && j["graphicsFrequency"].is_number_float()) {
        graphicsFrequency = j["graphicsFrequency"];
    }
    if (j.contains("graphicsLogEnabled") && j["graphicsLogEnabled"].is_boolean()) {
        graphicsLogEnabled = j["graphicsLogEnabled"];
    }
    if (j.contains("textFrequency") && j["textFrequency"].is_number_float()) {
        textFrequency = j["textFrequency"];
    }
    if (j.contains("textLogEnabled") && j["textLogEnabled"].is_boolean()) {
        textLogEnabled = j["textLogEnabled"];
    }

    ImVec2 scale = Element::getScaleFactors(resolution);
    position = ImVec2(position.x * scale.x, position.y * scale.y);
    size = ImVec2(size.x * scale.x, size.y * scale.y);
}


std::vector<Setting> Rectangle::getSettings() {
    return {
            {"graphicsFrequency",  graphicsFrequency,  [this](
                    const SettingValue &val) { graphicsFrequency = std::get<int>(val); }},
            {"graphicsLogEnabled", graphicsLogEnabled, [this](
                    const SettingValue &val) { graphicsLogEnabled = std::get<bool>(val); }},
            {"textFrequency",      textFrequency,      [this](const SettingValue &val) {
                textFrequency = std::get<int>(val);
            }},
            {"textLogEnabled",     textLogEnabled,     [this](
                    const SettingValue &val) { textLogEnabled = std::get<bool>(val); }}
    };
}


void Rectangle::setModuleID(int id) {
    moduleId = id;
}

void Rectangle::setGraphicElementId(int id) {
    graphicElementId = id;
}
void Rectangle::setModuleName(std::string name) {
    moduleName = name;
    label = graphicElementName + "\n" + moduleName;
}
void Rectangle::setGraphicElementName(std::string name) {
    graphicElementName = name;
    label = graphicElementName+ "\n" + moduleName;
}

