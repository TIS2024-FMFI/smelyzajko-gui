#include "imgui.h"
#include "imgui_internal.h"
#include "string"

enum ElementPlaceholderType {
    TEXT, GRAPHIC, OTHER
};

class ElementPlaceholder {

public:
    ElementPlaceholder(std::string label, ImVec2 position, ImVec2 size)
            : label(std::move(label)), position(position), size(size), pendingDelete(false) {}

    void draw();
    void detectRightClickDelete();

    bool pendingDelete;
    ImVec2 deletePopupPosition;

private:
    std::string label;
    ImVec2 position;
    ImVec2 size;
};


