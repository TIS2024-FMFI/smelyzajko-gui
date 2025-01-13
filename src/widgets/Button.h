#include "Element.h"

class Button : public Element {
private:
    ImVec2 size;
public:
    Button(const std::string& lbl = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), const ImVec2& sz = ImVec2(100.0f, 25.0f))
            : Element(pos, lbl), size(sz) {}

    ImVec2 getSize() const;

    void setSize(const ImVec2& newSize);

    ImRect getBoundingBox() const override;
    void draw(ImGuiIO& io) override;
    void handleClicks(ImGuiIO &io) override;
    static void setStyles();
    static void removeStyles();

    void to_json(nlohmann::json &j) const override;
    void from_json(const nlohmann::json &j, ImVec2 resolution) override;
};
