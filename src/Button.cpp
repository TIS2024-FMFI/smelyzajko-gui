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

    setStyles();

    // Draw the button outline
    ImGui::SetCursorScreenPos(position);
    ImGui::SetNextItemAllowOverlap();
    ImGui::Button(label.c_str(), size);

    removeStyles();

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
        if (ImGui::InvisibleButton(("Button" + label).c_str(), size)) {
            std::cout << "Button clicked: " << label << std::endl;
        }
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            ImVec2 delta = io.MouseDelta;
            position.x += delta.x;
            position.y += delta.y;
        }
    }
}

void Button::setStyles() {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 120, 200, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(80, 150, 240, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(30, 100, 180, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 255, 255, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
}

void Button::removeStyles() {
    ImGui::PopStyleVar(2); // Restore the default border size
    ImGui::PopStyleColor(4); // Restore the previous colors (Button, Hovered, Active, Border)
}