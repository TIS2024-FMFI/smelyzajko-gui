#include "Rectangle.h"

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
    ImVec2 rect_min = ImVec2(position.x + ImGui::GetWindowPos().x,
                             position.y + ImGui::GetWindowPos().y);
    ImVec2 rect_max = ImVec2(position.x + size.x + ImGui::GetWindowPos().x,
                             position.y + size.y + ImGui::GetWindowPos().y);

    draw_list->AddRectFilled(rect_min, rect_max, IM_COL32(20, 20, 255, 255)); // White outline
    draw_list->AddRect(rect_min, rect_max, IM_COL32(255, 255, 255, 255)); // White outline

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

