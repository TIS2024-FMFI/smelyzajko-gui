#include "Element.h"
#include <algorithm>

template<typename E> class Slider : public Element {
    static_assert(std::is_arithmetic<E>::value, "Slider can only work with numeric types (int, float, etc.)");
private:
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

    void draw(ImGuiIO& io) override;
    void handleClicks(ImGuiIO &io) override;
    ImRect getBoundingBox() const override;

    float getNormalizedValue() const;

    void from_json(const nlohmann::json &j) override;
    void to_json(nlohmann::json &j) const override;
    std::vector<Setting> getSettings() override;
};
