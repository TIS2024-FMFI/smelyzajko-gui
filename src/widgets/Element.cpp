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

void Element::setWasDragged(bool value) {
    wasDragged = value;
}

bool Element::getWasDragged() const {
    return wasDragged;
}

void Element::to_json(nlohmann::json &j) const {
    j["label"] = label;
    j["position"] = {position.x, position.y};
}

void Element::from_json(const nlohmann::json &j, ImVec2 resolution) {
    if (j.contains("label") && j["label"].is_string()) {
        label = j["label"];
    }

    if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2) {
        position.x = j["position"][0];
        position.y = j["position"][1];
    } else {
        position = ImVec2(0.0f, 0.0f);
    }
}

ImVec2 Element::getScaleFactors(ImVec2 templateResolution) {
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

    int monitorWidth = videoMode->width;
    int monitorHeight = videoMode->height;

    float scaleX = monitorWidth / templateResolution.x;
    float scaleY = monitorHeight / templateResolution.y;

    return ImVec2(scaleX, scaleY);
}