#pragma once
#include "Element.h"

class Label : public Element {
protected:
    // the text is in the 'label' attribute of Element
    ImVec2 size;
    float font_size = 13.0f;

public:
    Label(const std::string& lbl, const ImVec2& pos, const ImVec2& sz)
            : Element(pos, lbl), size(sz) {}

    void setText(const std::string& newText);

    const std::string& getText() const;

    void createTextSizeButton();

    void draw(ImGuiIO& io) override = 0;
    void handleClicks(ImGuiIO &io) override;
    virtual ImRect getBoundingBox() const override;

    void from_json(const nlohmann::json &j) override;
    std::vector<Setting> getSettings() override;
};