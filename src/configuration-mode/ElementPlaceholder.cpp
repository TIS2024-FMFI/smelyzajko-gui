#include "ElementPlaceholder.h"

void ElementPlaceholder::draw() {
    ImGui::SetNextWindowPos(position, ImGuiCond_Once);
    ImGui::SetNextWindowSize(size, ImGuiCond_Once);

    if (ImGui::Begin(label.c_str(), nullptr, ImGuiWindowFlags_NoCollapse)) {
        ImVec2 newPos = ImGui::GetWindowPos();
        ImVec2 newSize = ImGui::GetWindowSize();

        ImVec2 displaySize = ImGui::GetIO().DisplaySize;

        ImVec2 topLeft = newPos;
        ImVec2 bottomRight = {newPos.x + newSize.x, newPos.y + newSize.y};

        if (topLeft.x < 0.0f) {
            newPos.x = 0;
        }
        if (topLeft.y < 0.0f) {
            newPos.y = 0;
        }
        if (bottomRight.x > displaySize.x) {
            newPos.x -= (bottomRight.x - displaySize.x);
        }
        if (bottomRight.y > displaySize.y) {
            newPos.y -= (bottomRight.y - displaySize.y);
        }

        ImGui::SetWindowPos(newPos);

        position = newPos;
        size = newSize;

        detectRightClickDelete();

        ImGui::Text("Position: (%.1f, %.1f)", position.x, position.y);
        ImGui::Text("Size: (%.1f x %.1f)", size.x, size.y);
    }
    ImGui::End();
}

void ElementPlaceholder::detectRightClickDelete() {
    if (ImGui::IsWindowHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        deletePopupPosition = ImGui::GetMousePos();
        pendingDelete = true;
    }
}


