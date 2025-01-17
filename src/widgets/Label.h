#pragma once
#include "Element.h"

class Label : public Element {
protected:
    // the text is in the 'label' attribute of Element
    float font_size = 13.0f;

public:
    Label(const std::string& lbl, const ImVec2& pos, const ImVec2& sz)
            : Element(pos, lbl) {
        setSize(sz);
    }

    void setText(const std::string& newText);

    const std::string& getText() const;

    void createTextSizeButton();

    void draw(ImGuiIO& io) override = 0;
    void handleClicks(ImGuiIO &io) override;
    virtual ImRect getBoundingBox() const override;

    std::vector<Setting> getSettings() override;
    void from_json(const nlohmann::json &j, ImVec2 resolution) override;
};