#include "Label.h"

class SingleLineLabel : public Label {
public:
    SingleLineLabel(const std::string& lbl, const ImVec2& pos, const ImVec2& sz)
            : Label(lbl, pos, sz) {}

    void draw(ImGuiIO& io) override;
};