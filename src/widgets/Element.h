#pragma once

#include <utility>
#include "optional"
#include "imgui.h"
#include "string"
#include "imgui_internal.h"
#include <GLFW/glfw3.h>
#include "variant"
#include "libs/json.hpp"

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
    // Position and size of the element
    ImVec2 position;
    ImVec2 size;

    std::string moduleName;
    int moduleID;
    std::string label;
    bool pendingChooseWhatToDo = false;
    bool pendingEdit = false;
    bool pendingDelete = false;
    ImVec2 popupPosition;
    int zIndex = 0;
    bool configurationMode;
    bool wasDragged = false;

public:
    Element(std::string lbl = "", std::string moduleName = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), const ImVec2& size = ImVec2(0.0f, 0.0f))
            : label(std::move(lbl)), moduleName(std::move(moduleName)), position(pos), size(size) {}

    virtual ~Element() = default;

// Getters
    ImVec2 getPosition() const;
    std::string getLabel() const;
    bool getPendingDelete() const;
    bool getPendingEdit() const;
    bool getPendingChooseWhatToDo() const;
    ImVec2 getPopupPosition();
    int getZIndex() const;
    bool getWasDragged() const;
    std::string getModuleName() const;
    ImVec2 getSize() const;
// Setters
    void setPosition(const ImVec2& newPos);
    void setLabel(const std::string& newLabel);
    void setPendingDelete(bool newBool);
    void setPendingEdit(bool newBool);
    void setPendingChooseWhatToDo(bool newBool);
    void setPopupPosition(ImVec2 newPopupPosition);
    void setZIndex(int z);
    void setConfigurationMode(bool newBool);
    void setWasDragged(bool value);
    virtual void setModuleName(const std::string& name);
    void setModuleID(int id);
    void setSize(const ImVec2& newSize);

    virtual std::optional<int> getIntValue()  { return std::nullopt; }
    virtual std::optional<float> getFloatValue()  { return std::nullopt; }
    virtual std::optional<bool> getBoolValue()  { return std::nullopt; }
    virtual std::optional<std::string> getStringValue()  { return std::nullopt; }
    // Utility
    void move(const ImVec2& delta);
    virtual void draw(ImGuiIO& io) = 0;
    void detectRightClick();
    virtual ImRect getBoundingBox() const = 0;

    virtual void handleClicks(ImGuiIO& io) = 0;

    virtual void to_json(nlohmann::json& j) const;
    virtual void from_json(const nlohmann::json& j, ImVec2 resolution);

    static ImVec2 getScaleFactors(ImVec2 templateResolution);
    virtual std::vector<Setting> getSettings()  = 0;
};

