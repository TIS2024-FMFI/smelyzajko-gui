#pragma once

#include "Element.h"
#include <algorithm>

template<typename E> class Slider : public Element {
    static_assert(std::is_arithmetic<E>::value, "Slider can only work with numeric types (int, float, etc.)");
protected:
    ImVec2 size;
    E value;
    E minValue;
    E maxValue;
    int moduleId = -1;

public:
    Slider(const std::string& lbl = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), const ImVec2& sz = ImVec2(200.0f, 20.0f), E minVal = 0, E maxVal = 1, E initialValue = 0)
            : Element(pos, lbl), size(sz), minValue(minVal), maxValue(maxVal), value(initialValue) {}

    E getValue() const;
    void setValue(E val);

    virtual void draw(ImGuiIO& io) override = 0;
    virtual void handleClicks(ImGuiIO &io) override = 0;
    ImRect getBoundingBox() const override;

    float getNormalizedValue() const;

    virtual void from_json(const nlohmann::json &j, ImVec2 resolution) override;
    virtual void to_json(nlohmann::json &j) const override = 0;

    std::vector<Setting> getSettings() override;
};
