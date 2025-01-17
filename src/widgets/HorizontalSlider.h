#include "Slider.h"


template<typename E>
class HorizontalSlider : public Slider<E> {
public:
    HorizontalSlider(const std::string& lbl = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), const ImVec2& sz = ImVec2(200.0f, 20.0f), E minVal = 0, E maxVal = 1, E initialValue = 0)
            : Slider<E>(lbl, pos, sz, minVal, maxVal, initialValue) {}

    void draw(ImGuiIO& io) override;
    void handleClicks(ImGuiIO &io) override;

    ImRect getBoundingBox() const override {
        return Slider<E>::getBoundingBox();
    }

    void from_json(const nlohmann::json &j, ImVec2 resolution) override;
    void to_json(nlohmann::json &j) const override;
};

