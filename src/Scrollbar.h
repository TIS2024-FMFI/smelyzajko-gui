#ifndef SMELYZAJKO_GUI_SCROLLBAR_H
#define SMELYZAJKO_GUI_SCROLLBAR_H

#include "imgui.h"
#include <algorithm>

/**
 * @class Scrollbar
 * @brief Scrollbar class represents a scrollbar element in the GUI.
 */

class Scrollbar {
public:
    Scrollbar(float visibleHeight, float totalHeight);

    void drawScrollbar(ImVec2 scrollAreaMin, ImVec2 scrollAreaMax, ImGuiIO& io);
    void updateScrollOffset(ImGuiIO& io);

    float getScrollOffset() const { return scrollOffset; }
    void updateTotalHeight(float newTotalHeight);
    void updateVisibleHeight(float newVisibleHeight);

    void enableAutoscroll(bool enable) { autoscrollEnabled = enable; }
    bool isAutoscrollEnabled() const { return autoscrollEnabled; }

private:
    float scrollOffset;
    float visibleHeight;
    float totalHeight;
    bool autoscrollEnabled;
};

#endif // SMELYZAJKO_GUI_SCROLLBAR_H
