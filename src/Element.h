#pragma once

#include <utility>

#include "imgui.h"
#include "string"
#include "imgui_internal.h"

class Element {
protected:
    ImVec2 position;
    std::string label;
    bool pendingDelete;
    ImVec2 deletePopupPosition;
    int zIndex = 0;

public:
    Element(const ImVec2& pos = ImVec2(0.0f, 0.0f), std::string lbl = "")
            : position(pos), label(std::move(lbl)), pendingDelete(false) {}

    virtual ~Element() = default;

    // Getters
    ImVec2 getPosition() const;
    std::string getLabel() const;
    bool getPendingDelete() const;
    ImVec2 getDeletePopupPosition();
    int getZIndex() const;

    // Setters
    void setPosition(const ImVec2& newPos);
    void setLabel(const std::string& newLabel);
    void setPendingDelete(bool newBool);
    void setDeletePopupPosition(ImVec2 newPopupPosition);
    void setZIndex(int z);


    // Utility
    void move(const ImVec2& delta);
    virtual void draw(ImGuiIO& io) = 0;
    void detectRightClickDelete();
    virtual ImRect getBoundingBox() const = 0;

    virtual void handleClicks(ImGuiIO& io) = 0;
};

