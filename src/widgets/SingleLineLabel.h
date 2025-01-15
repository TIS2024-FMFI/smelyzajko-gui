#include "Label.h"

class SingleLineLabel : public Label {
public:
    SingleLineLabel(const std::string& lbl = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), const ImVec2& sz = ImVec2(100.0f, 100.0f))
            : Label(lbl, pos, sz) {}

    void draw(ImGuiIO& io) override;

    void from_json(const nlohmann::json &j, ImVec2 resolution) override;
    void to_json(nlohmann::json &j) const override;
};