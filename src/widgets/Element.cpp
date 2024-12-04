#include "Element.h"

ImVec2 Element::getPosition() const {
    return position;
}

std::string Element::getLabel() const {
    return label;
}

void Element::setPosition(const ImVec2 &newPos) {
    position = newPos;
}

void Element::setLabel(const std::string &newLabel) {
    label = newLabel;
}

void Element::move(const ImVec2& delta) {
    position.x += delta.x;
    position.y += delta.y;
}

bool Element::getPendingDelete() const {
    return pendingDelete;
}

void Element::setPendingDelete(bool newPendingDelete) {
    pendingDelete = newPendingDelete;
}

ImVec2 Element::getDeletePopupPosition() {
    return deletePopupPosition;
}

void Element::setDeletePopupPosition(ImVec2 newPopupPosition) {
    deletePopupPosition = newPopupPosition;
}

void Element::detectRightClickDelete() {
    deletePopupPosition = ImGui::GetMousePos();
    pendingDelete = true; // Mark that the element is pending deletion
}

int Element::getZIndex() const {
    return zIndex;
}

void Element::setZIndex(int z) {
    zIndex = z;
}




