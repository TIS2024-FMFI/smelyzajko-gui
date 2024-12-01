#include "Element.h"
#include <algorithm>

template<typename E> class Slider : public Element {
    static_assert(std::is_arithmetic<E>::value, "Slider can only work with numeric types (int, float, etc.)");
private:
    ImVec2 size;
    E value;
    E minValue;
    E maxValue;

public:
    // Constructor
    Slider(const std::string& lbl, const ImVec2& pos, const ImVec2& sz, E minVal, E maxVal, E initialValue)
            : Element(pos, lbl), size(sz), minValue(minVal), maxValue(maxVal), value(initialValue) {}

    E getValue() const;
    void setValue(E val);

    void draw(ImGuiIO& io) override;
    void handleClicks(ImGuiIO &io) override;
    ImRect getBoundingBox() const override;

    float getNormalizedValue() const;
};
