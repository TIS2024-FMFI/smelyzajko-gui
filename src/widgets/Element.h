#pragma once

#include <utility>
#include "../../json.hpp"
#include "imgui.h"
#include "string"
#include "imgui_internal.h"
#include <GLFW/glfw3.h>
// Typ alias pre variant
using SettingValue = std::variant<bool, int, float, std::string>;
// Struct pre nastavenie
struct Setting {
    std::string name;
    SettingValue value;
    std::function<void(const SettingValue&)> setter;
};


class Element {
protected:
    ImVec2 position;
    std::string label;
    bool pendingChooseWhatToDo;
    bool pendingEdit;
    bool pendingDelete;
    ImVec2 deletePopupPosition;
    int zIndex = 0;
    bool configurationMode;
    bool wasDragged = false;

public:
    Element(const ImVec2& pos = ImVec2(0.0f, 0.0f), std::string lbl = "")
            : position(pos), label(std::move(lbl)), pendingDelete(false) {}

    virtual ~Element() = default;

// Getters
    ImVec2 getPosition() const;
    std::string getLabel() const;
    bool getPendingDelete() const;
    bool getPendingEdit() const;
    bool getPendingChooseWhatToDo() const;
    ImVec2 getDeletePopupPosition();
    int getZIndex() const;
    bool getWasDragged() const;

// Setters
    void setPosition(const ImVec2& newPos);
    void setLabel(const std::string& newLabel);
    void setPendingDelete(bool newBool);
    void setPendingEdit(bool newBool);
    void setPendingChooseWhatToDo(bool newBool);
    void setDeletePopupPosition(ImVec2 newPopupPosition);
    void setZIndex(int z);
    void setConfigurationMode(bool newBool);
    void setWasDragged(bool value);


    // Utility
    void move(const ImVec2& delta);
    virtual void draw(ImGuiIO& io) = 0;
    void detectRightClickDelete();
    virtual ImRect getBoundingBox() const = 0;

    virtual void handleClicks(ImGuiIO& io) = 0;

    virtual void to_json(nlohmann::json& j) const;
    virtual void from_json(const nlohmann::json& j, ImVec2 resolution);

    static ImVec2 getScaleFactors(ImVec2 templateResolution);
    virtual std::vector<Setting> getSettings()  = 0;
};