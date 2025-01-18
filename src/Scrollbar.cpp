#include "Scrollbar.h"

Scrollbar::Scrollbar(float visibleHeight, float totalHeight)
        : scrollOffset(0.0f), visibleHeight(visibleHeight), totalHeight(totalHeight), autoscrollEnabled(true) {}

void Scrollbar::drawScrollbar(ImVec2 scrollAreaMin, ImVec2 scrollAreaMax, ImGuiIO& io) {
    float scrollbarWidth = 20.0f; // Width of the scrollbar
    float checkboxHeight = 20.0f; // Space reserved for the checkbox

    // Adjust scrollbar's vertical position to start below the checkbox
    ImVec2 scrollbarMin = ImVec2(scrollAreaMax.x - scrollbarWidth, scrollAreaMin.y + checkboxHeight);
    ImVec2 scrollbarMax = ImVec2(scrollAreaMax.x, scrollAreaMax.y);

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled(scrollbarMin, scrollbarMax, IM_COL32(255, 255, 255, 255)); // White color for scrollbar

    // Calculate thumb size and position
    float thumbHeight = std::max((visibleHeight / totalHeight) * (scrollbarMax.y - scrollbarMin.y), 10.0f);
    float thumbOffset = (scrollOffset / std::max(1.0f, totalHeight)) * (scrollbarMax.y - scrollbarMin.y);
    thumbOffset = std::clamp(thumbOffset, 0.0f, (scrollbarMax.y - scrollbarMin.y) - thumbHeight);

    ImVec2 thumbMin = ImVec2(scrollbarMin.x, scrollbarMin.y + thumbOffset);
    ImVec2 thumbMax = ImVec2(scrollbarMax.x, thumbMin.y + thumbHeight);
    drawList->AddRectFilled(thumbMin, thumbMax, IM_COL32(0, 0, 0, 255)); // Black color for thumb

    // Handle scrollbar dragging
    if (ImGui::IsMouseHoveringRect(scrollbarMin, scrollbarMax) && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        ImVec2 mousePos = io.MousePos;
        float mouseScrollPos = mousePos.y - scrollbarMin.y - (thumbHeight / 2);
        scrollOffset = (mouseScrollPos / (scrollbarMax.y - scrollbarMin.y)) * totalHeight;
        scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, totalHeight - visibleHeight));
    }
}

void Scrollbar::updateScrollOffset(ImGuiIO& io) {
    if (ImGui::IsMouseHoveringRect(ImVec2(0, 0), ImVec2(visibleHeight, totalHeight))) {
        scrollOffset -= io.MouseWheel * 20.0f;
        scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, totalHeight - visibleHeight));
    }

    if (autoscrollEnabled) {
        scrollOffset = std::max(0.0f, totalHeight - visibleHeight);
    }
}

void Scrollbar::updateTotalHeight(float newTotalHeight) {
    totalHeight = newTotalHeight;
    scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, totalHeight - visibleHeight));
}

void Scrollbar::updateVisibleHeight(float newVisibleHeight) {
    visibleHeight = newVisibleHeight;
    scrollOffset = std::clamp(scrollOffset, 0.0f, std::max(0.0f, totalHeight - visibleHeight));
}
